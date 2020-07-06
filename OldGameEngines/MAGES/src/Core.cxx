/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#include "MAGES/Globals.hxx"

#include <iostream>

#include <Poco/ConsoleChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Runnable.h>
#include <Poco/RunnableAdapter.h>
#include <Poco/Thread.h>
#include <Poco/ThreadPool.h>
using namespace Poco;

#include "MAGES/Core.hxx"
using namespace MAGES;

Core::Core(String confFile, String logFile, FileFormat confFormat):
	_log(Logger::get("MAGES.Core")) {

	Mutex::ScopedLock updLock(_mUpdates);
	Mutex::ScopedLock cmdLock(_mCommands);
	Channel* dest;
	Formatter* format;
	Id i;

#if defined DEBUG || not defined NDEBUG
	Logger::root().setLevel("trace");
	_log.setLevel("trace");
	format = new PatternFormatter("%b %e, %H:%M:%S.%i %s:%p(%U:%u): %t");
#else
	Logger::root().setLevel("warning");
	_log.setLevel("warning");
	format = new PatternFormatter("%b %e, %H:%M:%S.%i %s:%p: %t");
#endif

	if(logFile == "") {
		dest = new ColorConsoleChannel();
		dest->setProperty("noticeColor", "green");
		std::cout << "No log file specified; logging to default console channel." << std::endl;
	} else {
		try {
			dest = new SimpleFileChannel(logFile);
		} catch(...) {
			dest = new ColorConsoleChannel();
			dest->setProperty("noticeColor", "green");
			std::cerr << "Could not open log file; logging to default console channel." << std::endl;
		}
	}

	_logChannel = new FormattingChannel(format, dest);
	Logger::root().setChannel(_logChannel);
	_log.setChannel(_logChannel);

	_db = new PropertyDB(DEFAULT_CHAR_SIZE);
	if(confFile != "") {
		_initialized = _db->unserializeFromFile(confFile, confFormat);
	} else _initialized = true;

	_commands = new std::vector<Subsystem*>* [ID_MAX + 1];
	for(i = 0; i < ID_MAX; ++i) _commands[i] = NULL;
}


Core::~Core() {
	shutdown();
}


bool Core::isInitialized() {
	return _initialized;
}


void Core::shutdown() {
	Id i;

	_mUpdates.lock();
	if(_initialized) {
		poco_notice(_log, "Core shutting down.");
		std::vector<Subsystem*>::iterator it;
		for(it = _subsystems.begin() ; it != _subsystems.end(); it++) {
			//Normally we'd want to lock a mutex in there,
			// but we'll use an atomic bool for now, just for fun.
			(*it)->_coreReady = false; //C++11 feature
		}

		_initialized = false;
		_mUpdates.unlock();
		ThreadPool::defaultPool().joinAll();
		_mUpdates.lock();

		_manualSubsystems.clear();
		for(it = _subsystems.begin() ; it != _subsystems.end(); it++) {
			(*it)->destroy();
			delete(*it);
		}
		_subsystems.clear();

		_mCommands.lock();
		for(i = 0; i <= ID_MAX; ++i) delete _commands[i];
		delete _commands;
		_commands = NULL;
		_mCommands.unlock();

		delete _db;
		_db = NULL;

		//Can't delete because destructor is protected
		//delete _logChannel;
		_logChannel = NULL;
	}

	_mUpdates.unlock();
}


PropertyDB& Core::getDatabase() {
	return *_db;
}

/*
Poco::Channel& Core::getLogChannel() {
    return *_logChannel;
}
*/

bool Core::registerSubsystem(Subsystem* sub, bool createThread, UInt32 procReqs) {
	Mutex::ScopedLock lock(_mUpdates);
	std::vector<Id>::iterator idIter;

	if(_db != NULL) {
		if(sub != NULL) {
			///@todo Figure out whether to add back the inherited mutex, under the
			///      assumption that the implementation will actually use / respect it...
			//Mutex::ScopedLock(sub->_mUpdates);
			sub->_core = this;
			sub->_db = _db;
			sub->_coreReady = _initialized;

			poco_notice_f1(_log, "Initializing subsystem '%s'...", String(sub->name()));
			if(sub->initialize()) {
				poco_notice(_log, "Subsystem initialized, now registering commands...");
				_mCommands.lock();
				for(idIter = sub->_commands.begin(); idIter != sub->_commands.end(); ++idIter) {
					if(_commands[*idIter] == NULL) _commands[*idIter] = new std::vector<Subsystem*>;
					_commands[*idIter]->push_back(sub);
				}
				_mCommands.unlock();

				poco_notice(_log, "Commands registered, now scheduling update loop.");
				if(createThread) {
					///@todo Support auto-sleep based on procReqs and/or update backlog.
					//Either that or remove procReqs and just assume threads will manage their own yielding wisely.
					//Or implement a mix of the two in virtual run() function?
					ThreadPool::defaultPool().start(*sub);
				} else {
					_manualSubsystems.push_back(sub);
				}
				_subsystems.push_back(sub);
				return true;
			}

			poco_error_f1(_log, "Initialization of subsystem '%s' failed.", String(sub->name()));
		} else poco_error(_log, "Initialization of unknown subsystem failed.");
	} else poco_critical(_log, "Attempted to initialize a subsystem without having a valid PropertyDB!");

	return false;
}


void Core::sendCommand(Id command, String property) {
	//Eventually it might be better to just add things to a central command queue and dispatch
	// from there during update() cycles, but considering that most subsystems will be calling this
	// from an update cycle, I think this will work fine for now (unless we start hitting deadlocks).
	std::vector<Subsystem*>::iterator it;

	_mCommands.lock();
	if(_commands[command] != NULL) {
		for(it = _commands[command]->begin(); it != _commands[command]->end(); ++it) {
			(*it)->dispatchCommand(command, property);
		}
	}
	_mCommands.unlock();
}


bool Core::update() {
	std::vector<Subsystem*>::iterator it;

	_mUpdates.lock();
	it = _manualSubsystems.begin();
	_mUpdates.unlock();

	while(true) {
		_mUpdates.lock();
		if(it == _manualSubsystems.end()) break;
//		(*it)->_mUpdates.lock();

		if(!(*it)->update()) {
//			(*it)->_mUpdates.unlock();
			_mUpdates.unlock();
			return false;
		}

//		(*it)->_mUpdates.unlock();
		_mUpdates.unlock();
	}

	_mUpdates.unlock();
	return true;
}

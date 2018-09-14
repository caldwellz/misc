/*
 *	Overmind: A 3D RPG game.
 *	This file is part of Overmind.
 *
 *	Copyright (C) 2012 by authors.
 *
 * Overmind is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 * Overmind is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *	along with Overmind.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OvermindEngine.hpp"
using namespace OvermindEngine;

namespace OvermindEngine {
	//Define global vars here so they're only in one compilation unit
	NotificationCenter nfCenter;
	Logger *logOvermind;
	bool overmindReady;
}

void OvermindEngine::init() {
//Set up logging
	AutoPtr<ConsoleChannel> logChannel(new ConsoleChannel);
	AutoPtr<PatternFormatter> logFormat(new PatternFormatter);
	#ifdef _DEBUG
		logFormat->setProperty("pattern", "%H:%M:%S.%i [%l-%p][%s][%P, %I:%T][%U:%u]: '%t'");
	#else
		logFormat->setProperty("pattern", "%H:%M:%S.%i [%p][%s]: '%t'");
	#endif

	AutoPtr<FormattingChannel> logFormatChannel(new FormattingChannel(logFormat, logChannel));
	Logger::root().setChannel(logFormatChannel);
	logOvermind = &Logger::get("Overmind");

	/// \bug Setting the logOvermind logging level doesn't seem to work
	#ifdef _DEBUG
		logOvermind->setLevel("trace");
	#else
		logOvermind->setLevel("warning");
	#endif
	logOvermind->notice("Logging initialized");
//

	//Create mManager instances
	new OgreManager();
	new LuaManager();
	new OISManager();
	new CEGUIManager();
	new OgreAudioManager();

	//Alert that we're ready to start initializing everything.
	//Since postNotification blocks until all recieving observers finish processing,
	//this shouldn't return until all mManagers have finished initializing.
	nfCenter.postNotification(new nfManagerStatus(M_MAIN, MSI_BEGIN_INITIALIZE));

	//Since everything should be working now, let's alert that we're done initializing
	nfCenter.postNotification(new nfManagerStatus(M_MAIN, MSI_END_INITIALIZE));

	overmindReady = (OgreManager::getSingleton().isReady() & LuaManager::getSingletonPtr()->isReady() & OISManager::getSingleton().isReady() & CEGUIManager::getSingleton().isReady() & OgreAudioManager::getSingleton().isReady());
}

void OvermindEngine::run() {
	if(OgreManager::getSingleton().isReady() && LuaManager::getSingletonPtr()->isReady() && OISManager::getSingleton().isReady() && CEGUIManager::getSingleton().isReady() && OgreAudioManager::getSingleton().isReady()) { //Make sure everything's ready so we're not trying to run when we're supposed to be deinitializing
		//Alert that we're starting a run cycle.
		//Since postNotification blocks until all recieving observers finish processing,
		//this shouldn't return until all mManagers have finished a run cycle.
		nfCenter.postNotification(new nfManagerStatus(M_MAIN, MSI_BEGIN_RUN));

		//Since everything should have been run, let's alert that we finished a cycle
		nfCenter.postNotification(new nfManagerStatus(M_MAIN, MSI_END_RUN));
	} else
		overmindReady = false;
}

void OvermindEngine::deinit() {
	//Alert that we're ready to start deinitializing everything.
	//Since postNotification blocks until all recieving observers finish processing,
	//this shouldn't return until all mManagers have finished deinitializing.
	nfCenter.postNotification(new nfManagerStatus(M_MAIN, MSI_BEGIN_DEINITIALIZE));

	//Dunno if there would still be any observers at this point (except maybe the host program),
	//but we post this anyway, for the sake of completeness.
	nfCenter.postNotification(new nfManagerStatus(M_MAIN, MSI_END_DEINITIALIZE));

	overmindReady = false;
}

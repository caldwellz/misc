/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include <Poco/Mutex.h>
#include <Poco/Logger.h>
#include <Poco/ThreadPool.h>
#include <Poco/Timestamp.h>
using namespace Poco;

#include "MAGES/MAGES.hxx"
using namespace MAGES;

static int i, n;
static Mutex m;
class TestSystem: public MAGES::Subsystem {
	public:
		const char* name() {
			return "TestSystem";
		}

		bool initialize() {
			_commands.push_back(STDCMD_TEST);
			return true;
		}

		bool update() {
			Mutex::ScopedLock lock(m);

			while(!_cmdQueue.empty()) {
				cout << "TestSystem: Received command " << _cmdQueue.front().first << " with parameter '" << _cmdQueue.front().second << "'" << endl;
				_cmdQueue.pop();
			}

			if(i <= 5000) {
				if(n <= 100) {
					_db->setProperty<UInt32>(String("Test") + to_string(i), i * 1.5, true); //Uses C++11 feature
					++n;
					return true;
				} else {
					n = 1;
					++i;
					return true;
				}
			}

			++i;
			return false;
		}
};

int main() {
	try {
		string pause;
		stringstream msg;
		Timestamp ts;
		Core c;
		TestSystem t;
		Logger& log = Logger::get("TCore");

		//We use the Warning message priority just so that we get output in both Debug and Release mode
		poco_warning(log, "- Starting test: Setting 5000 properties 100 times each...");
		i = 0;
		n = 101;
		c.registerSubsystem(&t, true);
		c.sendCommand(STDCMD_TEST, "TestData");
		c.getDatabase().setProperty<String>("TestString", "This is a test.", true);
		ThreadPool::defaultPool().joinAll();
		msg << "Set time: " << (ts.elapsed() / 1000000.0f) << " seconds.";
		poco_warning(log, msg.str());
		msg.str("");

		poco_warning(log, "- Writing properties to TCore.jsonz...");
		ts.update();
		c.getDatabase().serializeToFile("TCore.jsonz");
		msg << "Write time: " << (ts.elapsed() / 1000000.0f) << " seconds.";
		poco_warning(log, msg.str());
		msg.str("");

		poco_warning(log, "- Clearing database...");
		ts.update();
		c.getDatabase().clear();
		msg << "Clear time: " << (ts.elapsed() / 1000000.0f) << " seconds.";
		poco_warning(log, msg.str());
		msg.str("");

		poco_warning(log, "- Reading properties back from TCore.jsonz...");
		ts.update();
		c.getDatabase().unserializeFromFile("TCore.jsonz");
		msg << "Read time: " << (ts.elapsed() / 1000000.0f) << " seconds.";
		poco_warning(log, msg.str());
		msg.str("");

		msg << "Test1000 (should be 1500): " << c.getDatabase().getProperty<UInt32>("Test1000");
		poco_warning(log, msg.str());
		msg.str("");
		msg << "TestString (should be \'This is a test.\'): " << c.getDatabase().getProperty<String>("TestString");
		poco_warning(log, msg.str());
		msg.str("");

		poco_warning(log, "- \'get\'-ing all properties 100 times each...");
		ts.update();

		for(i = 1; i <= 5000; ++i) {
			for(n = 1; n <= 100; ++n) {
				m.lock();
				c.getDatabase().getProperty<UInt32>(String("Test") + to_string(i)); //Uses C++11 feature
				m.unlock();
			}
		}

		msg << "Get time: " << (ts.elapsed() / 1000000.0f) << " seconds.";
		poco_warning(log, msg.str());
		msg.str("");

		
		cout << "- Pausing before cleanup for the sake of external mem/state analysis...]" << endl;
		cin >> pause;

		ts.update();
		c.shutdown();
		msg << "Cleanup time: " << (ts.elapsed() / 1000000.0f) << " seconds." << endl;
		poco_warning(log, msg.str());
		msg.str("");

	} catch(exception& e) {
		cout << "Encountered exception: " << e.what() << endl << "Aborting..." << endl;
		return 1;
	} catch(...) {
		cout << "Encountered unknown exception. Aborting..." << endl;
		return 1;
	}

	return 0;
}

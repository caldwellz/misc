/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MAGES_CORE_HXX_
#define _MAGES_CORE_HXX_

#include <vector>

#include <Poco/Channel.h>
#include <Poco/Mutex.h>

#include "Globals.hxx"
#include "Subsystem.hxx"
#include "PropertyDB.hxx"

namespace MAGES {

	/** The engine core.
	*   This is where you register subsystems and where the property database is stored.
	*   It is possible to have multiple instances with different subsystem sets if needed;
	*   however it would be your responsibility to ensure that they don't conflict. */
	class Core {
		public:
			/** Default constructor
			* \param logFile Where to output the log. Logs to console if not specified.
			* \param confFile An initial file to be loaded into the PropertyDB.
			* \param confFormat The config file format. \see FileFormat. */
			Core(String confFile = String("MAGES.json"), String logFile = "", FileFormat confFormat = FMT_AUTO);

			/** Default destructor
			*   Calls shutdown() automatically. */
			~Core();

			/** Check whether the Core has been initialized.
			*   Currently it just indicates that the constructor was
			*   successful, and that no one has called shutdown() since. */
			bool isInitialized();

			/** Shut down the core.
			*   Tells all subsystems to stop, waits for them, destroys them, then
			*   clears the PropertyDB. Called automatically by the destructor. */
			void shutdown();

			/// Get a reference to the internal PropertyDB.
			PropertyDB& getDatabase();

			/** Register a subsystem.
			*   The Core will take care of calling its initialize() method.
			* \param sub Pointer to a created (usually uninitialized) subsystem.
			* \param createThread True to start the sub's run() function in a new
			*        thread, False to just call its update() function every frame.
			* \param procReqs Processing requirements when run in a seperate thread. Unused for now. */
			bool registerSubsystem(Subsystem* sub, bool createThread = false, UInt32 procReqs = PROC_CPU_LOW);

			/** Send a command to all subsystems that are registered to receive it.
			* \param command The command identifier; 'standardized' ones will eventually be created.
			* \param property Can be the key for something in the PropertyDB, or just a straight string. */
			void sendCommand(Id command, String property);

			/** Process next frame / update subsystems.
			* Just calls update() on all registered subsystems that aren't running in their own thread. */
			bool update();

		protected:
			Poco::Mutex _mUpdates, _mCommands;
			Poco::Channel* _logChannel;
			Logger& _log;
			bool _initialized;
			PropertyDB* _db;
			std::vector<Subsystem*> _subsystems, _manualSubsystems, ** _commands;
	};
}
#endif //_MAGES_CORE_HXX_

/***************************************************
* MAGES: Modular Application / Game Engine System  *
*       Copyright (C) 2016 the MAGES Project       *
****************************************************
* This Source Code Form is subject to the terms of *
* the Mozilla Public License, v. 2.0. If a copy of *
* the MPL was not distributed with this file, You  *
* can obtain one at http://mozilla.org/MPL/2.0/.   *
***************************************************/

#ifndef _MAGES_SUBSYSTEM_HXX_
#define _MAGES_SUBSYSTEM_HXX_

#include <queue>
#include <utility>
#include <vector>
#include <atomic> //C++11 feature

#include <Poco/Mutex.h>
#include <Poco/Runnable.h>

#include "Globals.hxx"

namespace MAGES {
	class Core;
	class PropertyDB;

	/** Basic Subsystem interface.
	*   Custom subsystems should inherit this class. */
	class Subsystem: public Poco::Runnable {
		public:
			friend class Core;

			///Default constructor.
			Subsystem();

			/** Get the subsystem's name.
			*   Must be overriden by subclasses. */
			virtual const char* name() = 0;

			/** Initialize the subsystem.
			*   The default implementation always returns True.
			* \returns True on success, False on failure. */
			virtual bool initialize();

			/** Get a list of the subsystem's supported commands.
			*   This will probably primarily be used by the Core for setting up
			*   dispatching, however, it currently just accesses the vector directly.
			*   The default implementation just returns the instance's vector in whatever
			*   state it's in, which is useful if you set it up during initialize().
			* \returns A vector containing (hopefully) the subsystem's supported commands. */
			//virtual const std::vector<Id>& getCommands();

			/** Dispatch a command to the subsystem.
			*   The default, intended use just adds the command to a queue. In really fast and
			*   infrequently-used subsystems, however, it may make more sense to just do all
			*   the processing here in a case switch, although that would the block the calling
			*   dispatcher. This is normally only called by the Core command dispatcher.
			* \param command The command Id. */
			virtual void dispatchCommand(Id command, String property);

			/** Process next frame / update the subsystem.
			*   Ideally, most subsystems will use this instead of doing their
			*   processing in dispatchCommand(), to avoid blocking the Core command
			*   dispatcher (updates are done in a seperate thread).
			* \returns True if updating was successful (and/or the subsystem wants to KEEP being updated), False otherwise. */
			virtual bool update() = 0;

			/** Run the subsystem.
			*   Used by Core when it is told to create a new subsystem thread.
			*   The default implementation just calls update() in a loop until it returns False. */
			virtual void run();

			/** Destroy the subsystem.
			*   Shut down functionality and clean up resources. The default
			*   implementation simply returns. (You really should override it ;) */
			virtual void destroy();

		protected:
			MAGES::Core* _core;
			MAGES::PropertyDB* _db;
			Poco::Mutex _m;
			std::atomic<bool> _coreReady; //C++11 feature
			std::vector<Id> _commands;
			std::queue<std::pair<Id, String> > _cmdQueue;
	};
}
#endif //_MAGES_SUBSYSTEM_HXX_

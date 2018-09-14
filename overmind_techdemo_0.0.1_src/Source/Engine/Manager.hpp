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

#ifndef _Manager_hpp_
#define _Manager_hpp_

#include <Poco/ConsoleChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/NotificationCenter.h>
#include <Poco/Notification.h>
#include <Poco/NObserver.h>
using namespace Poco;

#include "Singleton.hpp"

namespace OvermindEngine {
	extern NotificationCenter nfCenter;

	/// \brief Managers enumeration
	enum Managers {
		/// \brief Overmind engine
		/// \remarks Used to signify the Overmind engine as a whole;
				/// not technically a manager, but it lets us use nfManagerStatus
				/// instead of having to implement a whole seperate notification.
		M_MAIN,
		/// \brief OgreAudioManager
		M_OGREAUDIO,
		/// \brief CEGUIManager
		M_CEGUI,
		/// \brief OISManager
		M_OIS,
		/// \brief LuaManager
		M_LUA,
		/// \brief OgreManager
		M_OGRE,
	};

	/// \brief Manager mManagerStatusIndicator indicators
	enum ManagerStatusIndicators {
		/// \brief Beginning initialization
		MSI_BEGIN_INITIALIZE,
		/// \brief Finished initialization
		MSI_END_INITIALIZE,
		/// \brief Beginning reconfiguration
		MSI_BEGIN_RECONFIGURE,
		/// \brief Finished reconfiguration
		MSI_END_RECONFIGURE,
		/// \brief Beginning run cycle
		MSI_BEGIN_RUN,
		/// \brief Finished run cycle
		MSI_END_RUN,
		/// \brief Beginning deinitialization
		MSI_BEGIN_DEINITIALIZE,
		/// \brief Finished deinitialization
		MSI_END_DEINITIALIZE,
	};

	/// \brief Manager status notification
	class nfManagerStatus: public Notification {
		public:
			const Managers mManager;
			const ManagerStatusIndicators mManagerStatusIndicator;

			/// \brief Constructor
			/// \param manager The manager indicating the status
			/// \param status The manager status that you wish to notify of
			nfManagerStatus(Managers manager, ManagerStatusIndicators status):
				mManager(manager),
				mManagerStatusIndicator(status)
			{ }
	};

	/// \brief Manager error notification. Dunno if it will ever actually be used, but we'll see.
	class nfManagerError: public Notification {
		public:
			const Managers mManager;
			const int mError;

			nfManagerError(Managers mManager, int error):
				mManager(mManager),
				mError(error)
			{ }
	};

	/// \brief The base Manager class, inherited by all manager implementations.
	class Manager {
		public:
			/// \brief Manager status notification handler
			virtual void hManagerStatus(const AutoPtr<nfManagerStatus>& stat) = 0;

			/// \brief Get mManager ready mManagerStatusIndicator
			/// \return True if the mManager is initialized and ready to run, false otherwise
			virtual bool isReady() const { return mManagerReady; }

			/// \brief Constructor
			Manager():
				oManagerStatusIndicators(*this, &Manager::hManagerStatus),
				logger(NULL),
				mManagerReady(false)
			{ nfCenter.addObserver(oManagerStatusIndicators); }

		protected:
			NObserver<Manager, nfManagerStatus> oManagerStatusIndicators;
			Logger *logger;
			bool mManagerReady;
	};
} //namespace OvermindEngine

#endif //_Manager_hpp_

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

#ifndef _OgreAudioManager_hpp_
#define _OgreAudioManager_hpp_

#include "Manager.hpp"

#include <OgreOggSound.h>
using namespace OgreOggSound;

#include <OGRE/OgreString.h>

namespace OvermindEngine {
	/// \brief Class used to store an audio configuration
	class AudioConfig {
		public:
			Ogre::String mDeviceName;
			unsigned int mMaxSources;
			unsigned int mQueueListSize;

			/// \brief Constructor
			/// \param deviceName The name of the audio device to open
			/// \param maxSources The max number of sources to allocate
			/// \param queueListSize Desired queue list size
			AudioConfig(std::string deviceName = "", unsigned int maxSources = 100, unsigned int queueListSize = 64):
				mDeviceName(deviceName),
				mMaxSources(maxSources),
				mQueueListSize(queueListSize)
			{ }
	};

	/// \brief OgreOggSound library manager
	class OgreAudioManager: public OvermindEngine::Singleton<OgreAudioManager>, public Manager {
		public:
			void hManagerStatus(const AutoPtr<nfManagerStatus>& stat);


			/// \brief Get current audio config
			/// \return Const reference to the current AudioConfig in use by the mManager.
			virtual const AudioConfig& getAudioConfig() { return config; }

			/// \brief Reconfigure the audio device
			/// \remarks Reconfigure the audio device using an initialized AudioConfig instance.
			/// \param conf The new AudioConfig to use
			virtual void setAudioConfig(AudioConfig conf);

			/// \brief Main OgreOggSoundManager pointer
			OgreOggSoundManager *mSoundManager;

		protected:
			void init();
			void run();
			void deinit();

			AudioConfig config;
	};
} //namespace OvermindEngine

#endif //_OgreAudioManager_hpp_

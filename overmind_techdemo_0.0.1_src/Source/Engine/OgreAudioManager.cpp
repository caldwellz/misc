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

template<> OgreAudioManager* OvermindEngine::Singleton<OgreAudioManager>::msSingleton = 0;

void OgreAudioManager::hManagerStatus(const AutoPtr<nfManagerStatus>& stat) {
	if(stat->mManager == M_OGRE) {
		if(stat->mManagerStatusIndicator == MSI_END_INITIALIZE)
			init();
		if(stat->mManagerStatusIndicator == MSI_BEGIN_RUN)
			run();
		if(stat->mManagerStatusIndicator == MSI_BEGIN_DEINITIALIZE)
			deinit();
	}
}

void OgreAudioManager::init() {
	if(!logger) logger = &logOvermind->get("OgreAudioManager");
	nfCenter.postNotification(new nfManagerStatus(M_OGREAUDIO, MSI_BEGIN_INITIALIZE));
	logger->information("OgreAudioManager initializing...");

	OgreManager::getSingleton().loadOgrePlugin("OgreOggSound");
	mSoundManager = OgreOggSoundManager::getSingletonPtr();
	if(!mSoundManager->init(config.mDeviceName, config.mMaxSources, config.mQueueListSize)) {
		logger->error("mSoundManager creation failed!");
		return;
	}

	mManagerReady = true;
	logger->notice("OgreAudioManager initialized");
	nfCenter.postNotification(new nfManagerStatus(M_OGREAUDIO, MSI_END_INITIALIZE));
}

void OgreAudioManager::run() {
	nfCenter.postNotification(new nfManagerStatus(M_OGREAUDIO, MSI_BEGIN_RUN));

	mSoundManager->update(OgreManager::getSingleton().mTimeDelta);

	nfCenter.postNotification(new nfManagerStatus(M_OGREAUDIO, MSI_END_RUN));
}

void OgreAudioManager::deinit() {
	nfCenter.postNotification(new nfManagerStatus(M_OGREAUDIO, MSI_BEGIN_DEINITIALIZE));
	logger->information("OgreAudioManager deinitializing...");

	OgreManager::getSingleton().unloadOgrePlugin("OgreOggSound");
	mSoundManager = NULL;

	mManagerReady = false;
	logger->information("OgreAudioManager deinitialized");
	nfCenter.postNotification(new nfManagerStatus(M_OGREAUDIO, MSI_END_DEINITIALIZE));
}

void OgreAudioManager::setAudioConfig(AudioConfig conf) {
	nfCenter.postNotification(new nfManagerStatus(M_OGREAUDIO, MSI_BEGIN_RECONFIGURE));
	deinit();
	config = conf;
	init();
	nfCenter.postNotification(new nfManagerStatus(M_OGREAUDIO, MSI_END_RECONFIGURE));
}

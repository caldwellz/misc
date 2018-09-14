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

#include <sstream>

#include "OvermindEngine.hpp"
using namespace OvermindEngine;

template<> OISManager* OvermindEngine::Singleton<OISManager>::msSingleton = 0;

void OISManager::hManagerStatus(const AutoPtr<nfManagerStatus>& stat) {
	if(stat->mManager == M_OGRE) {
		if(stat->mManagerStatusIndicator == MSI_END_INITIALIZE)
			init();
		if(stat->mManagerStatusIndicator == MSI_BEGIN_DEINITIALIZE)
			deinit();
	} else if(stat->mManager == M_MAIN) {
		if(stat->mManagerStatusIndicator == MSI_BEGIN_RUN)
			run();
	}
}

void OISManager::init() {
	if(!logger) logger = &logOvermind->get("OISManager");
	nfCenter.postNotification(new nfManagerStatus(M_OIS, MSI_BEGIN_INITIALIZE));
	logger->information("OISManager initializing...");

	//Initialize the OIS OISManager
	OIS::ParamList pl;
	size_t mRenderWindowHnd = 0;
	std::ostringstream mRenderWindowHndStr;
	OgreManager::getSingleton().mRenderWindow->getCustomAttribute("WINDOW", &mRenderWindowHnd);
	mRenderWindowHndStr << mRenderWindowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), mRenderWindowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);

	//Initialize mMouse object
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
	//Register as MouseListener
	mMouse->setEventCallback(this);
	//Set mMouse input boundaries to the size of the mRenderWindow
	mMouse->getMouseState().width = OgreManager::getSingleton().mRenderWindow->getWidth();
	mMouse->getMouseState().height = OgreManager::getSingleton().mRenderWindow->getHeight();

	//Initialize mKeyboard object
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	//Register as KeyListener
	mKeyboard->setEventCallback(this);

	mManagerReady = true;
	logger->notice("OISManager initialized");
	nfCenter.postNotification(new nfManagerStatus(M_OIS, MSI_END_INITIALIZE));
}

void OISManager::run() {
	nfCenter.postNotification(new nfManagerStatus(M_OIS, MSI_BEGIN_RUN));

	//Capture mMouse and mKeyboard input
	mMouse->capture();
	mKeyboard->capture();

	nfCenter.postNotification(new nfManagerStatus(M_OIS, MSI_END_RUN));
}

void OISManager::deinit() {
	nfCenter.postNotification(new nfManagerStatus(M_OIS, MSI_BEGIN_DEINITIALIZE));
	logger->information("OISManager deinitializing...");

	OIS::InputManager::destroyInputSystem(mInputManager);

	mManagerReady = false;
	logger->information("OISManager deinitialized");
	nfCenter.postNotification(new nfManagerStatus(M_OIS, MSI_END_DEINITIALIZE));
}

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

#include <exception>
#include <string>

#include "OvermindEngine.hpp"
using namespace OvermindEngine;

template<> OgreManager* OvermindEngine::Singleton<OgreManager>::msSingleton = 0;

void OgreManager::hManagerStatus(const AutoPtr<nfManagerStatus>& stat) {
	if(stat->mManager == M_MAIN) {
		if(stat->mManagerStatusIndicator == MSI_BEGIN_INITIALIZE)
			init();
		if(stat->mManagerStatusIndicator == MSI_BEGIN_RUN)
			run();
		if(stat->mManagerStatusIndicator == MSI_BEGIN_DEINITIALIZE)
			deinit();
	}
}

void OgreManager::init() {
	if(!logger) logger = &logOvermind->get("OgreManager");
	nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_BEGIN_INITIALIZE));
	logger->information("OgreManager initializing...");

	try {
		//Create mRoot
		mRoot = std::auto_ptr<Ogre::Root>(new Root("", "", "Ogre.log"));
		//Load OpenGL render system: we need one in order to initialize and this is the most logical choice at the moment
		loadOgrePlugin("RenderSystem_GL");
		//If we still don't have a usable render system, bail out
		if(mRoot->getAvailableRenderers().size() == 0) throw Ogre::Exception(1, "No available mCEGUIRenderers", "OgreManager::Init");
		//Set the first available render system
		mRoot->setRenderSystem(mRoot->getAvailableRenderers()[0]);
		//Initialize
		mRoot->initialise(false, "", "");
		//Create initial render mRenderWindow
		mRenderWindow = mRoot->createRenderWindow("OGRE window", 640, 480, false);
		//Initialize mOldTime for mTimeDelta
		mOldTime = mRoot->getTimer()->getMilliseconds();
		//Set mRenderWindow as active
		mRenderWindow->setActive(true);
		//Initialize mRootScene and mRootCamera: temp objects used until Areas take over
		mRootScene = mRoot->createSceneManager(ST_GENERIC, "mRootScene");
		mRootCamera = mRootScene->createCamera("mRootCamera");
		//mRootScene->getRootSceneNode()->createChildSceneNode("mRootCameraNode")->attachObject(camera);
		//Create initial mViewport with mRootCamera, using entire mRenderWindow
		mViewport = mRenderWindow->addViewport(mRootCamera);
		mViewport->setAutoUpdated(true);
	}
	catch(Ogre::Exception &e) {
		std::string err("Ogre exception: ");
		err.append(e.what());
		logger->error(err);
		return;
	}
	catch(std::exception &e) {
		std::string err("std exception: ");
		err.append(e.what());
		logger->error(err);
		return;
	}

	mManagerReady = true;
	logger->notice("OgreManager initialized");
	nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_END_INITIALIZE));
}

void OgreManager::run() {
	nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_BEGIN_RUN));

	if(!mRenderWindow->isClosed()) {
		mRoot->renderOneFrame();
		Ogre::WindowEventUtilities::messagePump();
	} else
		mManagerReady = false;

	//Update mTimeDelta
	unsigned long curTime = mRoot->getTimer()->getMilliseconds();
	mTimeDelta = curTime - mOldTime;
	mOldTime = curTime;

	nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_END_RUN));
}

void OgreManager::deinit() {
	nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_BEGIN_DEINITIALIZE));
	logger->information("OgreManager deinitializing...");

	mRoot->destroySceneManager(mRootScene);
	mRoot->shutdown();
	mRoot.reset();

	mManagerReady = false;
	logger->information("OgreManager deinitialized");
	nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_END_DEINITIALIZE));
}

void OgreManager::loadOgrePlugin(Ogre::String pluginName) {
	if(&(*mRoot) != NULL) {
		if(OGRE_DEBUG_MODE)
			pluginName.append("_d");

		mRoot->loadPlugin(pluginName);
	}
}

void OgreManager::unloadOgrePlugin(Ogre::String pluginName) {
	if(&(*mRoot) != NULL) {
		if(OGRE_DEBUG_MODE)
			pluginName.append("_d");

		mRoot->unloadPlugin(pluginName);
	}
}

void OgreManager::setRenderWindowConfig(RenderWindowConfig conf) {
	nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_BEGIN_RECONFIGURE));

	if((conf.Name != config.Name) || (conf.MiscParams != config.MiscParams)) { //As far as I know, the mRenderWindow must be totally recreated to change either of these
		nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_BEGIN_DEINITIALIZE));
		mRoot->detachRenderTarget(mRenderWindow);
		//mRenderWindow->destroy();
		nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_END_DEINITIALIZE));
		nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_BEGIN_INITIALIZE));
		mRenderWindow = mRoot->createRenderWindow(conf.Name, conf.Width, conf.Height, conf.Fullscreen, conf.MiscParams);
		mRenderWindow->setActive(true);
		mViewport = mRenderWindow->addViewport(mRootCamera);
		mViewport->setAutoUpdated(true);
		nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_END_INITIALIZE));
	} else { //Otherwise, we can just use setFullscreen
		mRenderWindow->setFullscreen(conf.Fullscreen, conf.Width, conf.Height);
		mViewport->setDimensions(0, 0, 1.0, 1.0);
	}

	config = conf;
	nfCenter.postNotification(new nfManagerStatus(M_OGRE, MSI_END_RECONFIGURE));
}

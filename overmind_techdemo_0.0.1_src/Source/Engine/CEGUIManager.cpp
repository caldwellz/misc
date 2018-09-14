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

template<> CEGUIManager* OvermindEngine::Singleton<CEGUIManager>::msSingleton = 0;

void CEGUIManager::hManagerStatus(const AutoPtr<nfManagerStatus>& stat) {
	if(stat->mManager == M_OGRE) {
		switch(stat->mManagerStatusIndicator) {
			case MSI_BEGIN_RUN:
				run();
				return;

			case MSI_END_INITIALIZE:
				init();
				return;

			case MSI_BEGIN_DEINITIALIZE:
				deinit();
				return;
		}
	}
}

void CEGUIManager::init() {
	nfCenter.postNotification(new nfManagerStatus(M_CEGUI, MSI_BEGIN_INITIALIZE));
	logger = &logOvermind->get("CEGUIManager");
	logger->information("CEGUIManager initializing...");

	//Bootstrap renderer
	mCEGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem(*(OgreManager::getSingleton().mRenderWindow));

	mManagerReady = true;
	logger->notice("CEGUIManager initialized");
	nfCenter.postNotification(new nfManagerStatus(M_CEGUI, MSI_END_INITIALIZE));
}

void CEGUIManager::run() {
	nfCenter.postNotification(new nfManagerStatus(M_CEGUI, MSI_BEGIN_RUN));

	//Inject time pulse
	CEGUI::System::getSingleton().injectTimePulse((float) (OgreManager::getSingleton().mTimeDelta / 1000));

	nfCenter.postNotification(new nfManagerStatus(M_CEGUI, MSI_END_RUN));
}

void CEGUIManager::deinit() {
	nfCenter.postNotification(new nfManagerStatus(M_CEGUI, MSI_BEGIN_DEINITIALIZE));
	logger->information("CEGUIManager deinitializing...");

	mCEGUIRenderer->destroySystem();

	logger->information("CEGUIManager deinitialized");
	nfCenter.postNotification(new nfManagerStatus(M_CEGUI, MSI_END_DEINITIALIZE));
}

//Internal function used to convert mMouse mButtonID types
//Ripped from 'Basic Tutorial 7'
inline CEGUI::MouseButton convertButton(const OIS::MouseButtonID *mButtonID) {
	switch (*(mButtonID)) {
		case OIS::MB_Left:
			return CEGUI::LeftButton;

		case OIS::MB_Right:
			return CEGUI::RightButton;

		case OIS::MB_Middle:
			return CEGUI::MiddleButton;
    }
}

void CEGUIManager::hInput(const AutoPtr<nfInput>& input) {
	CEGUI::System &sys = CEGUI::System::getSingleton();

	switch(input->mInputType) {
		case IT_MOUSEMOVE:
			if(input->mMouseState->Z.rel)
				sys.injectMouseWheelChange(input->mMouseState->Z.rel / 120.0f);
			else
				sys.injectMouseMove(input->mMouseState->X.rel, input->mMouseState->Y.rel);
			return;

		case IT_MOUSEPRESS:
			sys.injectMouseButtonDown(convertButton(input->mButtonID));
			return;

		case IT_MOUSERELEASE:
			sys.injectMouseButtonUp(convertButton(input->mButtonID));
			return;

		case IT_KEYPRESS:
			sys.injectKeyDown(input->mKeyEvent->key);
			sys.injectChar(input->mKeyEvent->text);
			return;

		case IT_KEYRELEASE:
			sys.injectKeyUp(input->mKeyEvent->key);
			return;
	}
}

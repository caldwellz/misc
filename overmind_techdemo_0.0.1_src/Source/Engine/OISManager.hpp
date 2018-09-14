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

#ifndef _OISManager_hpp_
#define _OISManager_hpp_

#include "Manager.hpp"

#include "OIS/OIS.h"

namespace OvermindEngine {
	/// \brief Input type enumeration
	enum InputType {
		/// \brief Mouse moved
		IT_MOUSEMOVE,
		/// \brief Mouse mButtonID pressed
		IT_MOUSEPRESS,
		/// \brief Mouse mButtonID released
		IT_MOUSERELEASE,
		/// \brief Keyboard key pressed
		IT_KEYPRESS,
		/// \brief Keyboard key released
		IT_KEYRELEASE,
		//IT_JOYSTICK,
	};

	/// \brief Input notification
	class nfInput: public Notification {
		public:
			/// \brief Input type
			const InputType mInputType;

			/// \brief MouseState pointer
			/// \remarks Const pointer to current MouseState if mInputType is one of EIT_Mouse*, otherwise NULL.
			const OIS::MouseState* mMouseState;

			/// \brief MouseButtonID pointer
			/// \remarks Const pointer to event MouseButtonID if mInputType is IT_MOUSEPRESS or IT_MOUSERELEASE, otherwise NULL.
			const OIS::MouseButtonID* mButtonID;

			/// \brief KeyEvent pointer
			/// \remarks Const pointer to fired KeyEvent if mInputType is one of EIT_Key*, otherwise NULL.
			const OIS::KeyEvent* mKeyEvent;

			/// \brief Constructor
			/// \param inputType Input type
			/// \param mouseState Mouse state pointer
			/// \param buttonID Mouse event button ID pointer
			/// \param keyEvent Key event pointer
			nfInput(const InputType& inputType, const OIS::MouseState* mouseState = NULL, const OIS::MouseButtonID* buttonID = NULL, const OIS::KeyEvent* keyEvent = NULL):
				mInputType(inputType),
				mMouseState(mouseState),
				mButtonID(buttonID),
				mKeyEvent(keyEvent)
			{ }
	};

	/// \brief OIS library manager
	class OISManager: public OvermindEngine::Singleton<OISManager>, public Manager, public OIS::MouseListener, public OIS::KeyListener {
		public:
			void hManagerStatus(const AutoPtr<nfManagerStatus>& stat);

			/// \brief Get the OIS InputManager
			/// \return Pointer to the internal InputManager object
			virtual OIS::InputManager* getInputManager() { return mInputManager; }

			/// \brief Get the OIS Mouse
			/// \return Pointer to the internal Mouse object
			virtual OIS::Mouse* getMouse() { return mMouse; }

			/// \brief Get the OIS Keyboard
			/// \return Pointer to the internal Keyboard object
			virtual OIS::Keyboard* getKeyboard() { return mKeyboard; }


			/// \brief MouseListener method, should not generally be used by user
			bool mouseMoved(const OIS::MouseEvent &arg) { nfCenter.postNotification(new nfInput(IT_MOUSEMOVE, &arg.state)); }
			/// \brief MouseListener method, should not generally be used by user
			bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) { nfCenter.postNotification(new nfInput(IT_MOUSEPRESS, &arg.state, &id)); }
			/// \brief MouseListener method, should not generally be used by user
			bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) { nfCenter.postNotification(new nfInput(IT_MOUSERELEASE, &arg.state, &id)); }


			/// \brief KeyListener method, should not generally be used by user
			bool keyPressed(const OIS::KeyEvent &arg) { nfCenter.postNotification(new nfInput(IT_KEYPRESS, NULL, NULL, &arg)); }
			/// \brief KeyListener method, should not generally be used by user
			bool keyReleased(const OIS::KeyEvent &arg) { nfCenter.postNotification(new nfInput(IT_KEYRELEASE, NULL, NULL, &arg)); }



		protected:
			void init();
			void run();
			void deinit();

			/// \brief Main OIS InputManager
			OIS::InputManager* mInputManager;

			/// \brief OIS Mouse
			OIS::Mouse* mMouse;

			/// \brief OIS Keyboard
			OIS::Keyboard* mKeyboard;
	};
} //namespace OvermindEngine

#endif //_OISManager_hpp_

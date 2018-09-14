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

#ifndef _CEGUIManager_hpp_
#define _CEGUIManager_hpp_

#include "Manager.hpp"

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

namespace OvermindEngine {
	/// \brief CEGUI library manager
	class CEGUIManager: public OvermindEngine::Singleton<CEGUIManager>, public Manager {
		public:
			/// \brief Constructor
			CEGUIManager():
				oInput(*this, &CEGUIManager::hInput),
				mCEGUIRenderer(NULL)
			{ nfCenter.addObserver(oInput); }

			void hManagerStatus(const AutoPtr<nfManagerStatus>& stat);

			/// \brief Input notification handler
			void hInput(const AutoPtr<nfInput>& input);


			/// \brief Pointer to the CEGUI OgreRenderer
			CEGUI::OgreRenderer* mCEGUIRenderer;

		protected:
			void init();
			void run();
			void deinit();

			NObserver<CEGUIManager, nfInput> oInput;
	};
} //namespace OvermindEngine

#endif //_CEGUIManager_hpp_

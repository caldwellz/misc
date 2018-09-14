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

#ifndef _OgreManager_hpp_
#define _OgreManager_hpp_

#include <memory>

#include "Manager.hpp"

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
using namespace Ogre;

namespace OvermindEngine {
	/// \brief Render window configuration
	/// \remarks Render window configuration class. It's probably best not to use this, the only real reason it exists is to be able to specify mRenderWindow name and MiscParams. Once a better way of doing that stuff is found, this system will be marked deprecated.
	class RenderWindowConfig {
		public:
			Ogre::String Name;
			unsigned int Width;
			unsigned int Height;
			bool Fullscreen;
			const NameValuePairList* MiscParams;

			RenderWindowConfig(Ogre::String name = "OGRE window", unsigned int width = 640, unsigned int height = 480, bool fullscreen = false, const NameValuePairList* miscParams = NULL)
				{ Name = name; Width = width; Height = height; Fullscreen = fullscreen; MiscParams = miscParams; }
	};

	/// \brief Ogre3D library manager
	class OgreManager: public OvermindEngine::Singleton<OgreManager>, public Manager {
		public:
			void hManagerStatus(const AutoPtr<nfManagerStatus>& stat);


			/// \brief Load an Ogre plugin.
			/// \remarks Load an Ogre plugin. Automatically loads debug version if in debug mode, which is why this method exists.
			/// \param pluginName Name of the plugin to load
			virtual void loadOgrePlugin(Ogre::String pluginName);

			/// \brief Unload an Ogre plugin.
			/// \remarks Load an Ogre plugin. Automatically unloads debug version if in debug mode, which is why this method exists.
			/// \param pluginName Name of the plugin to unload
			virtual void unloadOgrePlugin(Ogre::String pluginName);

			/// \brief Get the current render mRenderWindow config
			/// \return Const pointer to the current RenderWindowConfig in use by the mManager.
			virtual const RenderWindowConfig* getRenderWindowConfig() const { return &config; }

			/// \brief Reconfigure the render mRenderWindow
			/// \remarks Reconfigure the render mRenderWindow using an initialized RenderWindowConfig. It's probably best not to use this, see RenderWindowConfig remarks.
			/// \param conf A RenderWindowConfig instance set with new parameters.
			virtual void setRenderWindowConfig(RenderWindowConfig conf);


			/// \brief Ogre Root
			std::auto_ptr<Ogre::Root> mRoot;

			/// \brief Ogre RenderWindow
			RenderWindow* mRenderWindow;

			/// \brief Main Viewport
			Viewport* mViewport;

			/// \brief Time delta
			/// \remarks Time delta since last run, in milliseconds.
			unsigned long mTimeDelta;

		protected:
			void init();
			void run();
			void deinit();

			SceneManager* mRootScene;
			Camera* mRootCamera;

			unsigned long mOldTime;
			RenderWindowConfig config;
	};
} //namespace OvermindEngine

#endif //_OgreManager_hpp_

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

#ifndef _OvermindEngine_hpp_
#define _OvermindEngine_hpp_

#include <OvermindConfig.h>

#include "OgreManager.hpp"
#include "LuaManager.hpp"
#include "OISManager.hpp"
#include "CEGUIManager.hpp"
#include "OgreAudioManager.hpp"

namespace OvermindEngine {
	/// \brief Our grand central NotificationCenter
	extern NotificationCenter nfCenter;

	/// \brief Top-level Overmind logger
	extern Logger *logOvermind;

	/// \brief OvermindEngine readiness variable
	extern bool overmindReady;

	/// \brief Initialize OvermindEngine.
	/// \remarks Initialize OvermindEngine. Automatically initializes mManagers and top-level logging.
	void init();

	/// \brief Run a frame of OvermindEngine.
	void run();

	/// \brief Deinitialize OvermindEngine.
	void deinit();
} //namespace OvermindEngine

#endif //_OvermindEngine_hpp_

/*
 *	ForTheWin: A modular windoze utility program.
 *	This file is part of ForTheWin.
 *
 *	Copyright (C) 2012 Zach Caldwell
 *
 * ForTheWin is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 * ForTheWin is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 *	along with ForTheWin.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _Chooser_hpp_
#define _Chooser_hpp_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Res/resource.h"

#include <vector>

#include "Module.hpp"

namespace ForTheWin {
	/// \brief Chooser dialog callback.
	BOOL CALLBACK ChooserProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	class Chooser {
		public:
			/// \brief Insert a module into the chooser list.
			void insertModule(Module* mod) { modList.push_back(mod); }

			/// \brief Module list (well, vector :)
			/// \remarks The reason a vector is used instead of a std::list is because we need direct position access
			std::vector<Module*> modList;

			HINSTANCE instance;
			HWND wnd;
	} extern chooser;
} //namespace ForTheWin

#endif //_Chooser_hpp_

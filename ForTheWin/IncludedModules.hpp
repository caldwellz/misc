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

#ifndef _IncludedModules_hpp_
#define _IncludedModules_hpp_

#include "Chooser.hpp"

#define INCLUDE_PiSpigot
#ifdef INCLUDE_PiSpigot
	#include "Modules/PiSpigot/PiSpigot-dlg.hpp"
#endif

#define INCLUDE_PrimeSift
#ifdef INCLUDE_PrimeSift
	#include "Modules/PrimeSift/PrimeSift-dlg.hpp"
#endif

namespace ForTheWin {
	void insertIncludedModules() {
		#ifdef INCLUDE_PiSpigot
			PiSpigot pispigot;
			chooser.insertModule(&pispigot);
		#endif

		#ifdef INCLUDE_PrimeSift
			PrimeSift primesift;
			chooser.insertModule(&primesift);
		#endif
	}
} //namespace ForTheWin

#endif //_IncludedModules_hpp_

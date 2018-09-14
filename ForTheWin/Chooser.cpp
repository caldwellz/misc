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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdlib>

#include "Res/resource.h" 
#include "Chooser.hpp"
using namespace ForTheWin;

Chooser ForTheWin::chooser;

BOOL CALLBACK ForTheWin::ChooserProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_INITDIALOG:
			// This is where we set up the dialog box, and initialise any default values

			chooser.wnd = hwnd;

			for(std::vector<Module*>::iterator it=chooser.modList.begin(); it < chooser.modList.end(); it++)
				SendDlgItemMessage(hwnd, IDC_MODULELIST, LB_ADDSTRING, 0, (LPARAM) (*(*it)).getName());

			break;

		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_GO:
					if(chooser.modList.size() > 0)
						chooser.modList.at( SendMessage( GetDlgItem(hwnd, IDC_MODULELIST), LB_GETCURSEL, 0, 0) )->go();
			}
			break;

		case WM_CLOSE:
			EndDialog(hwnd, 0);
			exit(0);
			break;

		default:
			return FALSE;
	}
	return TRUE;
}

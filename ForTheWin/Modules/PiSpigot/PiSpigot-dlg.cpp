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

#include "PiSpigot-dlg.hpp"
using namespace ForTheWin;

extern "C" {
#include "PiSpigot.h"
}

BOOL CALLBACK PiSpigotProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:
			// This is where we set up the dialog box, and initialise any default values
			SetDlgItemInt(hwnd, IDC_PIPOSITION, 1, FALSE);
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_PIDRIP:
				{
					//Save the current cursor and set a busy cursor
					HCURSOR SavCrs = GetCursor();
					SetCursor(LoadCursor(0, (LPCTSTR)IDC_WAIT));

					BOOL bSuccess;
					int posnum = GetDlgItemInt(hwnd, IDC_PIPOSITION, &bSuccess, FALSE);
					if(bSuccess) 
					{
						SetDlgItemInt(hwnd, IDC_PIDIGITS, piSpigot(posnum), FALSE);
					}
				/*	else 
					{
						MessageBox(hwnd, "You didn't enter anything!", "Warning", MB_OK);
					}	*/

					//Restore cursor
					SetCursor(SavCrs);
				}
				break;
			}
			break;

		case WM_CLOSE:
			EndDialog(hwnd, 0);
			break;

		default:
			return FALSE;
	}
	return TRUE;
}

void PiSpigot::go() {
	DialogBox(chooser.instance, MAKEINTRESOURCE(IDD_PISPIGOT), NULL, PiSpigotProc);
}

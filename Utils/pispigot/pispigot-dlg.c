#include <windows.h>
#include <stdlib.h>
#include "pispigot.h"
#include "resource.h" 

BOOL CALLBACK DlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
/*
int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow){
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
}
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:
			// This is where we set up the dialog box, and initialise any default values
			SetDlgItemInt(hwnd, IDC_POSITION, 1, FALSE);
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_DRIP:
				{
					//Save the current cursor and set a busy cursor
					HCURSOR SavCrs = GetCursor();
					SetCursor(LoadCursor(0, (LPCTSTR)IDC_WAIT));

					BOOL bSuccess;
					int posnum = GetDlgItemInt(hwnd, IDC_POSITION, &bSuccess, FALSE);
					if(bSuccess) 
					{
						//SetDlgItemInt(hwnd, IDC_PIDIGITS, 0, FALSE); *Doesn't seem to work* //Clear the display first.
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

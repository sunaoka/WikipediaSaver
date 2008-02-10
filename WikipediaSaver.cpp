/*
 * WikipediaSaver is copyright 2008 SUNAOKA Norifumi <http://pocari.org/>.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma comment(lib, "scrnsave.lib")

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <atlhost.h>

#include <windows.h>
#include <scrnsave.h>

CComModule _Module;
BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

#define WIKIPEDIA_URL "http://ja.wikipedia.org/wiki/%E7%89%B9%E5%88%A5:Random"
#define SCREEN_MARGIN 5
#define ID_TIMER 32767

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return TRUE;
}

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rc;

	static CComQIPtr<IWebBrowser2>	pIE;
	static HWND						hWndIE;
	CComPtr<IUnknown>				pUnkIE;
	CComVariant						vempty;
	CComVariant						vUrl(WIKIPEDIA_URL);
	
	switch (message) {
	case WM_CREATE:
		
		GetClientRect(hWnd, &rc);
		
		_Module.Init(ObjectMap, hMainInstance); // hMainInstance: scrnsave.h
		AtlAxWinInit();
		
		hWndIE = CreateWindow("AtlAxWin", "Shell.Explorer.2", WS_CHILD | WS_VISIBLE, 
					SCREEN_MARGIN, SCREEN_MARGIN, 
					rc.right - SCREEN_MARGIN * 2, rc.bottom - SCREEN_MARGIN * 2, 
					hWnd, (HMENU)0, GetModuleHandle(NULL), NULL);
		
		if (AtlAxGetControl(hWndIE, &pUnkIE) == S_OK) {
			pIE = pUnkIE;
			if (pIE) {
				pIE->Navigate2(&vUrl, &vempty, &vempty, &vempty, &vempty);
			}
		}
		
		SetTimer(hWnd, ID_TIMER, 15 * 1000, NULL);
		return 0;

	case WM_DESTROY:
		KillTimer(hWnd, ID_TIMER);
		pIE.Release();
		break;

	case WM_ERASEBKGND:
		break;

	case WM_TIMER:
		pIE->Navigate2(&vUrl, &vempty, &vempty, &vempty, &vempty);
		return 0;

	case WM_SIZE:
		return 0;

	default:
		break;
	}

	return DefScreenSaverProc(hWnd, message, wParam, lParam);
}

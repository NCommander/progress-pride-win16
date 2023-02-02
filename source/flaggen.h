#include "resource.h"

#ifndef WINVER
/* Pre-3.1 windows.h */

#define CALLBACK _far _pascal
typedef unsigned int UINT;
typedef UINT WPARAM;
typedef LONG LPARAM;

#endif

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
long CALLBACK __export MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL __export CALLBACK About(HWND, unsigned, WORD, LONG);

/* Color defines */
#define RED_COLORREF RGB( 255, 0, 0 )

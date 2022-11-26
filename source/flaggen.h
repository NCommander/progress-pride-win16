#include "resource.h"

int PASCAL WinMain(HANDLE, HANDLE, LPSTR, int);
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);
long CALLBACK __export MainWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL __export CALLBACK About(HWND, unsigned, WORD, LONG);

/* Color defines */
#define RED_COLORREF RGB( 255, 0, 0 )
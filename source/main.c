/****************************************************************************

    PROGRAM: Generic.c

    PURPOSE: Generic template for Windows applications

    FUNCTIONS:

    WinMain() - calls initialization function, processes message loop
    InitApplication() - initializes window data and registers window
    InitInstance() - saves instance handle and creates main window
    MainWndProc() - processes messages
    About() - processes messages for "About" dialog box

    COMMENTS:

        Windows can have several copies of your application running at the
        same time.  The variable hInst keeps track of which instance this
        application is so that processing will be to the correct window.

****************************************************************************/

#include "windows.h"            /* required for all Windows applications */
#include "flaggen.h"            /* specific to this program          */

HANDLE hInst;               /* current instance              */


/****************************************************************************

    FUNCTION: WinMain(HANDLE, HANDLE, LPSTR, int)

    PURPOSE: calls initialization function, processes message loop

    COMMENTS:

        Windows recognizes this function by name as the initial entry point
        for the program.  This function calls the application initialization
        routine, if no other instance of the program is running, and always
        calls the instance initialization routine.  It then executes a message
        retrieval and dispatch loop that is the top-level control structure
        for the remainder of execution.  The loop is terminated when a WM_QUIT
        message is received, at which time this function exits the application
        instance by returning the value passed by PostQuitMessage().

        If this function must abort before entering the message loop, it
        returns the conventional value NULL.

****************************************************************************/

int PASCAL WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
HANDLE hInstance;                /* current instance         */
HANDLE hPrevInstance;            /* previous instance        */
LPSTR lpCmdLine;                 /* command line             */
int nCmdShow;                    /* show-window type (open/icon) */
{
    MSG msg;                     /* message              */

    if (!hPrevInstance)          /* Other instances of app running? */
    if (!InitApplication(hInstance)) /* Initialize shared things */
        return (FALSE);      /* Exits if unable to initialize     */

    /* Perform initializations that apply to a specific instance */

    if (!InitInstance(hInstance, nCmdShow))
        return (FALSE);

    /* Acquire and dispatch messages until a WM_QUIT message is received. */

    while (GetMessage(&msg,    /* message structure              */
        NULL,          /* handle of window receiving the message */
        NULL,          /* lowest message to examine          */
        NULL))         /* highest message to examine         */
    {
    TranslateMessage(&msg);    /* Translates virtual key codes       */
    DispatchMessage(&msg);     /* Dispatches message to window       */
    }
    return (msg.wParam);       /* Returns the value from PostQuitMessage */
}


/****************************************************************************

    FUNCTION: InitApplication(HANDLE)

    PURPOSE: Initializes window data and registers window class

    COMMENTS:

        This function is called at initialization time only if no other
        instances of the application are running.  This function performs
        initialization tasks that can be done once for any number of running
        instances.

        In this case, we initialize a window class by filling out a data
        structure of type WNDCLASS and calling the Windows RegisterClass()
        function.  Since all instances of this application use the same window
        class, we only need to do this when the first instance is initialized.


****************************************************************************/

BOOL InitApplication(hInstance)
HANDLE hInstance;                  /* current instance       */
{
    WNDCLASS  wc;

    /* Fill in window class structure with parameters that describe the       */
    /* main window.                                                           */

    wc.style = NULL;                    /* Class style(s).                    */
    wc.lpfnWndProc = MainWndProc;       /* Function to retrieve messages for  */
                                        /* windows of this class.             */
    wc.cbClsExtra = 0;                  /* No per-class extra data.           */
    wc.cbWndExtra = 0;                  /* No per-window extra data.          */
    wc.hInstance = hInstance;           /* Application that owns the class.   */
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PRIDE));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  "GenericMenu";   /* Name of menu resource in .RC file. */
    wc.lpszClassName = "FlagGenWClass"; /* Name used in call to CreateWindow. */

    /* Register the window class and return success/failure code. */

    return (RegisterClass(&wc));

}


/****************************************************************************

    FUNCTION:  InitInstance(HANDLE, int)

    PURPOSE:  Saves instance handle and creates main window

    COMMENTS:

        This function is called at initialization time for every instance of
        this application.  This function performs initialization tasks that
        cannot be shared by multiple instances.

        In this case, we save the instance handle in a static variable and
        create and display the main program window.

****************************************************************************/

BOOL InitInstance(hInstance, nCmdShow)
    HANDLE          hInstance;          /* Current instance identifier.       */
    int             nCmdShow;           /* Param for first ShowWindow() call. */
{
    HWND            hWnd;               /* Main window handle.                */

    /* Save the instance handle in static variable, which will be used in  */
    /* many subsequence calls from this application to Windows.            */

    hInst = hInstance;

    /* Create a main window for this application instance.  */

    hWnd = CreateWindow(
        "FlagGenWClass",                /* See RegisterClass() call.          */
        "Flag Generator",   /* Text for window title bar.         */
        WS_OVERLAPPEDWINDOW,            /* Window style.                      */
        CW_USEDEFAULT,                  /* Default horizontal position.       */
        CW_USEDEFAULT,                  /* Default vertical position.         */
        CW_USEDEFAULT,                  /* Default width.                     */
        CW_USEDEFAULT,                  /* Default height.                    */
        NULL,                           /* Overlapped windows have no parent. */
        NULL,                           /* Use the window class menu.         */
        hInstance,                      /* This instance owns this window.    */
        NULL                            /* Pointer not needed.                */
    );

    /* If window could not be created, return "failure" */

    if (!hWnd)
        return (FALSE);

    /* Make the window visible; update its client area; and return "success" */

    ShowWindow(hWnd, nCmdShow);  /* Show the window                        */
    UpdateWindow(hWnd);          /* Sends WM_PAINT message                 */
    return (TRUE);               /* Returns the value from PostQuitMessage */

}

HBRUSH DrawStripe(hdc, hbr, rect, stripeSize, sizePos)
HDC hdc;
HBRUSH hbr;
RECT * rect;
int sizePos;
int stripeSize;
{
	HBRUSH hbrOld;
	/* Red Stripe */
    hbrOld = SelectObject(hdc, hbr);
    //Rectangle(hdc, nLeftRect, nTopRect, nRightRect, nBottomRect);
	Rectangle(hdc, 0, stripeSize*(sizePos), rect->right+1, stripeSize*(sizePos+1)+1);	
	
	return hbrOld;
}

void DrawChevron(hdc, hbr, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, xOffset, yOffset, centerOffset)
HDC hdc;
HBRUSH hbr;
RECT *rect;
int gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, xOffset, yOffset, centerOffset;
{
    int pointDrawOrder = 0;
    int pointsToDraw = 5;
    POINT chevronPoints[8];
    
    SelectObject(hdc, hbr);

	/* Draw Chevron */
	chevronPoints[0].x = xOffset;
	chevronPoints[0].y = yOffset;

	chevronPoints[1].x = gPixelsPerChevron + xOffset;	
	chevronPoints[1].y = 0;
	 
    chevronPoints[2].x = gWindowHalfwayXPos - centerOffset;
    chevronPoints[2].y = gWindowHalfwayYPos;	

	/* Handle even windows sizes */
	if (gWindowHalfwayXPos % 2) {
	  /* It's an odd window */
	  chevronPoints[3].x = gWindowHalfwayXPos + 1 - centerOffset;
	  chevronPoints[3].y = gWindowHalfwayYPos + 1;
	} else {
	  chevronPoints[3].x = gWindowHalfwayXPos - centerOffset;    
	  chevronPoints[3].y = gWindowHalfwayYPos;	
	}

    chevronPoints[4].x = gWindowHalfwayXPos - gPixelsPerChevron - centerOffset;
    chevronPoints[4].y = gWindowHalfwayYPos;

	/* Change the drawing order if needed */
	if (yOffset != 0) {
		int tmpX, tmpY;
		tmpX = chevronPoints[1].x;
		tmpY = chevronPoints[1].y;
		
		chevronPoints[1].x = 0;
		chevronPoints[1].y = yOffset - gPixelsPerChevron;
	}
    Polygon(hdc, chevronPoints, pointsToDraw);

	if (yOffset == 0) {
		chevronPoints[0].x = xOffset;
		chevronPoints[0].y = rect->bottom - yOffset;
	
		chevronPoints[1].x = gPixelsPerChevron + xOffset;
		chevronPoints[1].y = rect->bottom - yOffset;
    } else {
		chevronPoints[0].x = xOffset;
		chevronPoints[0].y = rect->bottom - yOffset;

		chevronPoints[1].x = xOffset;
		chevronPoints[1].y = rect->bottom - yOffset + gPixelsPerChevron;		
	}

    Polygon(hdc, chevronPoints, /*sizeof(chevronPoints)/sizeof(POINT) */ 5);
}

void DrawFlag(HDC hdc, RECT *rect)
//HWND hwnd;
{
	HPEN hPenNull, hPenBlack;
    HBRUSH hbrRed, hbrOrange, hbrYellow, hbrGreen, hbrBlue, hbrPurple, hbrOld;
    HBRUSH hbrBrown, hbrBlack, hbrLBlue, hbrPink, hbrWhite, hbrPurple2;
    
    int gPixelsPerStripe;
	int gPixelsPerChevron;
	int gWindowHalfwayXPos;
	int gWindowHalfwayYPos;
	
	int circleThickness;



    /* Initialize pens */
    hPenNull = GetStockObject(NULL_PEN);
	hPenBlack = GetStockObject(BLACK_PEN);

	/* Initialize brushes */
	hbrRed = CreateSolidBrush( RED_COLORREF );
	hbrOrange = CreateSolidBrush( RGB( 255, 127, 0));
	hbrYellow = CreateSolidBrush( RGB( 255, 255, 0 ));
	hbrGreen = CreateSolidBrush( RGB( 0, 255, 0 ));
	hbrBlue = CreateSolidBrush( RGB( 0, 0, 255 ));
	hbrPurple = CreateSolidBrush( RGB( 255, 0, 255 ));
	hbrBlack = GetStockObject(BLACK_BRUSH); // CreateSolidBrush( RGB( 0, 0, 0 )); 
	hbrBrown = CreateSolidBrush( RGB( 128, 64, 0));
	hbrLBlue = CreateSolidBrush( RGB( 0, 255, 255 ));
	hbrPink = CreateSolidBrush( RGB( 255, 128, 255 ));
	hbrWhite = GetStockObject(WHITE_BRUSH); // CreateSolidBrush( RGB( 255, 255, 255 ));
	hbrPurple2 = CreateSolidBrush( RGB( 64, 0, 128 ));

	/* Get size of current window */
    gPixelsPerStripe = (rect->bottom/6)+1;
	gPixelsPerChevron = (rect->right/10)+1;
	gWindowHalfwayXPos = (rect->right/2)+1;
	gWindowHalfwayYPos = (rect->bottom/2)+1;
	
	/* Sets Null pen (disables border on rectangles */
	SelectObject(hdc, hPenNull);
    
    /* Draw each stripe */
    hbrOld = DrawStripe(hdc, hbrRed, rect, gPixelsPerStripe, 0);
    DrawStripe(hdc, hbrOrange, rect, gPixelsPerStripe, 1);
	DrawStripe(hdc, hbrYellow, rect, gPixelsPerStripe, 2);
	DrawStripe(hdc, hbrGreen, rect, gPixelsPerStripe, 3);
	DrawStripe(hdc, hbrBlue, rect, gPixelsPerStripe, 4);
	DrawStripe(hdc, hbrPurple, rect, gPixelsPerStripe, 5);

#if 0
	// Unity Flag	
	DrawChevron(hdc, hbrBlack, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, gPixelsPerChevron, 0, 0);
	DrawChevron(hdc, hbrBrown, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, 0, 0, gPixelsPerChevron);
	DrawChevron(hdc, hbrLBlue, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, 0, gPixelsPerChevron, gPixelsPerChevron*2);
	DrawChevron(hdc, hbrPink, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, 0, gPixelsPerChevron*2, gPixelsPerChevron*3);
	DrawChevron(hdc, hbrWhite, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, 0, gPixelsPerChevron*3, gPixelsPerChevron*4);
#endif

	gPixelsPerChevron = (rect->right/16)+1;
    DrawChevron(hdc, hbrBlack, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, gPixelsPerChevron*5, 0, 0);
	DrawChevron(hdc, hbrBrown, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, gPixelsPerChevron*4, 0, gPixelsPerChevron);
	DrawChevron(hdc, hbrLBlue, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, gPixelsPerChevron*3, 0, gPixelsPerChevron*2);
	DrawChevron(hdc, hbrPink, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, gPixelsPerChevron*2, 0, gPixelsPerChevron*3);
	DrawChevron(hdc, hbrWhite, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, gPixelsPerChevron, 0, gPixelsPerChevron*4);
    DrawChevron(hdc, hbrYellow, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, 0, 0, gPixelsPerChevron*5);
    DrawChevron(hdc, hbrYellow, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, 0, 0, gPixelsPerChevron*6);
    DrawChevron(hdc, hbrYellow, rect, gPixelsPerChevron, gWindowHalfwayXPos, gWindowHalfwayYPos, 0, 0, gPixelsPerChevron*7);
    

    
    circleThickness = 10;
    SelectObject(hdc, hbrPurple2);
    Ellipse( hdc,
    		gPixelsPerChevron/2,
    		gWindowHalfwayYPos-(gPixelsPerChevron),
    		gPixelsPerChevron*2,
    		gWindowHalfwayYPos+(gPixelsPerChevron));
    
    SelectObject(hdc, hbrYellow);
    Ellipse( hdc,
    		(gPixelsPerChevron/2)+circleThickness,
    		(gWindowHalfwayYPos-(gPixelsPerChevron))+circleThickness,
    		(gPixelsPerChevron*2)-circleThickness,
    		(gWindowHalfwayYPos+(gPixelsPerChevron))-circleThickness);

	/* Deallocate brushes */
	SelectObject(hdc, hbrOld);
	DeleteObject(hbrRed);
	DeleteObject(hbrOrange);
	DeleteObject(hbrYellow);
	DeleteObject(hbrGreen);
	DeleteObject(hbrBlue);
	DeleteObject(hbrPurple);
	// DeleteObject(hbrBlack);
	DeleteObject(hbrBrown);
	DeleteObject(hbrLBlue);
	DeleteObject(hbrPink);
	// DeleteObject(hbrWhite);
	DeleteObject(hbrPurple2);

}

void PaintWindow(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT gAppWindowSize;

	
	/* Get size of current window */
    GetClientRect(hwnd, &gAppWindowSize);
	
	hdc = BeginPaint(hwnd, &ps);
	
	DrawFlag(hdc, &gAppWindowSize);
	

	EndPaint(hwnd, &ps);

}

long CALLBACK __export MainWndProc(hWnd, message, wParam, lParam)
HWND hWnd;                      /* window handle                 */
UINT message;                   /* type of message               */
WPARAM wParam;                  /* additional information        */
LPARAM lParam;                  /* additional information        */
{
    switch (message)
    {
        case WM_PAINT:
        	PaintWindow(hWnd);
        	break;

		case WM_SIZE:
			InvalidateRect(hWnd, NULL, TRUE);
			break;

        case WM_COMMAND:       /* message: command from application menu */
            if (wParam == IDM_ABOUT)
            {

                DialogBox(hInst,        /* current instance          */
                    "AboutBox",         /* resource to use           */
                    hWnd,               /* parent handle             */
                    About);             /* About() instance address  */

                break;
            }
            else                        /* Lets Windows process it   */
                return (DefWindowProc(hWnd, message, wParam, lParam));

        case WM_DESTROY:          /* message: window being destroyed */
            PostQuitMessage(0);
            break;

        default:                  /* Passes it on if unproccessed    */
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (NULL);
}


/****************************************************************************

    FUNCTION: About(HWND, unsigned, WORD, LONG)

    PURPOSE:  Processes messages for "About" dialog box

    MESSAGES:

    WM_INITDIALOG - initialize dialog box
    WM_COMMAND    - Input received

    COMMENTS:

    No initialization is needed for this particular dialog box, but TRUE
    must be returned to Windows.

    Wait for user to click on "Ok" button, then close the dialog box.

****************************************************************************/

BOOL __export CALLBACK About(hDlg, message, wParam, lParam)
HWND hDlg;               /* window handle of the dialog box */
unsigned message;        /* type of message                 */
WORD wParam;             /* message-specific information    */
LONG lParam;
{
    switch (message)
    {
        case WM_INITDIALOG:            /* message: initialize dialog box */
            return (TRUE);

        case WM_COMMAND:               /* message: received a command */
            if (wParam == IDOK         /* "OK" box selected?          */
                || wParam == IDCANCEL) /* System menu close command?  */
            {
                EndDialog(hDlg, TRUE); /* Exits the dialog box        */
                return (TRUE);
            }
            break;
    }
    return (FALSE);               /* Didn't process a message    */
}
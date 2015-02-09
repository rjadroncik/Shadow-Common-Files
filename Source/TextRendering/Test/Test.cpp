#include "Test.h"

using namespace SCFBase;
using namespace SCFTimer;
using namespace SCFTextRendering;
using namespace SCFGraphics;

////Handle to instance
HINSTANCE	g_hInst;								

//Handles to windows
HWND	g_hTextFrameWindow = NULL;
HWND	g_hTextFrameInfoWindow = NULL;

//Options
BOOL	g_bLockTextFrameDimensions = FALSE;
BOOL	g_bDisableStyles = FALSE;
BOOL	g_bEmbedOptions = TRUE;

BOOL	g_bDumpDebugInfo = TRUE;

DWORD	g_dwDisplayOptions = RO_DRAW_SELECTION | RO_DRAW_TEXTFRAME_OUTLINE | RO_DRAW_COLUMN_OUTLINES | RO_DRAW_PARAGRAPH_OUTLINES | RO_DRAW_LINE_OUTLINES | RO_DRAW_WORD_OUTLINES;

//Text-frame measurements
RECT	g_TextFrameRect;
INT		g_fInsets = 5;

BOOL	g_bCodeViewVisible = TRUE;
BOOL	g_bInfoViewVisible = TRUE;

RECT	g_TextFrameWindowRect;
RECT	g_TextFrameInfoWindowRect;
RECT	g_CodeViewWindowRect;

//Selection state
INT	g_iSelectionStart = 0;
INT	g_iSelectionEnd = 0;

BOOL	g_bDragging = FALSE;

POINT	g_MousePoint;
POINT	g_StartPoint;

//Device/world conversion variables
INT	g_fScreen2Point = 1;
INT	g_fPoint2Screen = 1;

//Edit control sub-classing variables
HWND	g_hEditOrigin		= NULL;
WNDPROC	g_EditProcOrigin	= NULL;
BOOL	g_bSkipResizing		= NULL;

CFrame g_Frame;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(hPrevInstance);

 	MSG msg;
	HACCEL hAccelTable;

	//Perform application initialization:
	if (FAILED(RegisterClasses(hInstance)))		{ return E_FAIL; }
	if (FAILED(InitInstance(hInstance, nCmdShow)))	{ return E_FAIL; }

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TESTAPP);

	//Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

HRESULT RegisterClasses(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)TextFrameProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TESTAPP);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_TESTAPP;
	wcex.lpszClassName	= TEXT("TextFrameWindowClass");
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	if (!RegisterClassEx(&wcex)) { return E_FAIL; }

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)TextFrameInfoProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, (LPCTSTR)IDI_TESTAPP);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("TextFrameInfoWindowClass");
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	if (!RegisterClassEx(&wcex)) { return E_FAIL; }

	return S_OK;
}

LRESULT CALLBACK EditProcSubClass(HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN)
	{
		if ((int)wParam == VK_RETURN)
		{ 
//			SendMessage(hWnd, EM_SETSEL, 0, -1);
			SendMessage(GetParent(hWnd), EN_ENTER, GetWindowLongPtr(hWnd, GWL_ID), (LONG_PTR)hWnd);
		
			return FALSE;
		}
	}

	return CallWindowProc(g_EditProcOrigin, hWnd, uMsg, wParam, lParam);
}

HRESULT InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	UNREFERENCED_PARAMETER(nCmdShow);

	if (!SCFTextRendering::Initialize()) { return E_FAIL; }

	g_Frame.Text(STRING("Testik zobrazovania textu"));

	INITCOMMONCONTROLSEX initEx;
	initEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	initEx.dwICC = ICC_WIN95_CLASSES;

	InitCommonControlsEx(&initEx);

	//Store instance handle in our global variable
	g_hInst = hInstance; 

	g_hTextFrameWindow = CreateWindowEx(NULL, TEXT("TextFrameWindowClass"), TEXT("Text Render Test Session"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 400, 300, NULL, NULL, hInstance, NULL);
	if (!g_hTextFrameWindow) { return E_FAIL; }

	GetWindowRect(g_hTextFrameWindow, &g_TextFrameWindowRect);

	g_hEditOrigin = CreateWindowEx(NULL, TEXT("EDIT"), NULL, WS_CHILD, 0, 0, 0, 0, g_hTextFrameWindow, (HMENU)9992, hInstance, NULL);
	g_EditProcOrigin = (WNDPROC)(DWORD)SetClassLongPtr(g_hEditOrigin, GCLP_WNDPROC, (LONG)(LONG_PTR)(WNDPROC)EditProcSubClass);

	g_hTextFrameInfoWindow	= CreateDialog(hInstance, MAKEINTRESOURCE(IDD_FRAME_INFO), g_hTextFrameWindow, (DLGPROC)TextFrameInfoProc);
	if (!g_hTextFrameInfoWindow) { return E_FAIL; }

	SetWindowPos(g_hTextFrameInfoWindow, g_hTextFrameWindow, g_TextFrameWindowRect.right, g_TextFrameWindowRect.top, 0, 0, SWP_NOSIZE);

	CString str = STRING("KOKOT") + STRING("TestApp.cfg");

	CDirectory currentDir;
	CFile settingsFile(currentDir.ToString() + STRING("TestApp.cfg"));

	if (settingsFile.Exists())
	{
		CStreamFileRead streamRead(settingsFile);
		CStreamReadTextGeneric streamReadText(streamRead);

		CString text;
		streamReadText.GetString(text, 0);

		LoadOptions(text);
	}

	ExecuteOptions();

	SetFocus(g_hTextFrameWindow);
	
	return S_OK;
}

void UpdateTextFrameWindowRect()
{
	RECT rt;
	rt.left		= 0;
	rt.top		= 0;
	rt.right	= 1 + ((g_TextFrameRect.right	+ g_fInsets * 2) * g_fPoint2Screen) / 1000000;
	rt.bottom	= 1 + ((g_TextFrameRect.bottom	+ g_fInsets * 2) * g_fPoint2Screen) / 1000000;

	MENUBARINFO menuInfo;
	menuInfo.cbSize = sizeof(MENUBARINFO);

	GetMenuBarInfo(g_hTextFrameWindow, OBJID_MENU, 0, &menuInfo);

	AdjustWindowRectEx(&rt, WS_OVERLAPPEDWINDOW, TRUE, NULL);

	g_TextFrameWindowRect.right		= rt.right - rt.left;
	g_TextFrameWindowRect.bottom	= rt.bottom - rt.top;
}

HRESULT ExecuteOptions()
{
	if (g_bCodeViewVisible)
	{
		HMENU hMenu = GetMenu(g_hTextFrameWindow);
		hMenu = GetSubMenu(hMenu, 1);

		CheckMenuItem(hMenu, ID_WINDOWS_CODEVIEW, MF_BYCOMMAND | MF_CHECKED);
	}
	else
	{
		HMENU hMenu = GetMenu(g_hTextFrameWindow);
		hMenu = GetSubMenu(hMenu, 1);

		CheckMenuItem(hMenu, ID_WINDOWS_CODEVIEW, MF_BYCOMMAND | MF_UNCHECKED);
	}

	if (g_bInfoViewVisible)
	{
		HMENU hMenu = GetMenu(g_hTextFrameWindow);
		hMenu = GetSubMenu(hMenu, 1);

		CheckMenuItem(hMenu, ID_WINDOWS_FRAME_INFO, MF_BYCOMMAND | MF_CHECKED);

		SetWindowPos(g_hTextFrameInfoWindow, NULL, g_TextFrameInfoWindowRect.left, g_TextFrameInfoWindowRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		ShowWindow(g_hTextFrameInfoWindow, SW_SHOW);
		UpdateWindow(g_hTextFrameInfoWindow);
	}
	else
	{
		HMENU hMenu = GetMenu(g_hTextFrameWindow);
		hMenu = GetSubMenu(hMenu, 1);

		SetWindowPos(g_hTextFrameInfoWindow, NULL, g_TextFrameInfoWindowRect.left, g_TextFrameInfoWindowRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	
		CheckMenuItem(hMenu, ID_WINDOWS_FRAME_INFO, MF_BYCOMMAND | MF_UNCHECKED);
		ShowWindow(g_hTextFrameInfoWindow, SW_HIDE);
	}

	SetWindowPos(g_hTextFrameWindow, NULL, g_TextFrameWindowRect.left, g_TextFrameWindowRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	//Display all created windows
	ShowWindow(g_hTextFrameWindow, SW_SHOW);
	UpdateWindow(g_hTextFrameWindow);

	return S_OK;
}

bool LoadOptions(_IN CString& rText)
{
	int	iWhitespaceLength = 0;
	int	iOptionLength     = 0;
	int	iOptionNameLength = 0;

	g_dwDisplayOptions = 0;

	CStringRange textLeft(rText);
	CStringSearch search(textLeft);

	while ((iWhitespaceLength = search.FindCharsInv(STRING(" \t\xfeff\n\r"))) > -1)
	{
		textLeft.ChangeStart(textLeft.Start() + iWhitespaceLength);
	
		if ((iOptionLength = search.FindChars(STRING(" \t\xfeff\n\r"))) == -1) { break; }

		CStringRange optionWithValue(rText, textLeft.Start(), iOptionLength);
		
		if ((iOptionNameLength = search.FindChars(STRING("="))) == -1) { return FALSE; } 

		CStringRange option(rText, textLeft.Start(), iOptionNameLength);
		CStringRange value (rText, textLeft.Start() + iOptionNameLength + 1, iOptionLength - iOptionNameLength - 1);

		textLeft.ChangeStart(textLeft.Start() + iOptionLength);
					
		//[Text-frame options]
		if (option == STRING("WindowX"))
		{
			g_TextFrameWindowRect.left = CInt::Parse(value, NULL);
		}

		if (option == STRING("WindowY"))
		{
			g_TextFrameWindowRect.top = CInt::Parse(value, NULL);
		}

		if (option == STRING("Width"))
		{
			g_TextFrameRect.right = CInt::Parse(value, NULL);
		}

		if (option == STRING("Height"))
		{
			g_TextFrameRect.bottom = CInt::Parse(value, NULL);
		}

		if (option == STRING("Insets"))
		{
			g_fInsets = CInt::Parse(value, NULL);
		}

		//[Display options]
		if (option == STRING("FrameBorder"))
		{
			if (CBool::Parse(value, NULL)) { g_dwDisplayOptions |= RO_DRAW_TEXTFRAME_OUTLINE; }
		}

		if (option == STRING("ColumnOutlines"))
		{
			if (CBool::Parse(value, NULL)) { g_dwDisplayOptions |= RO_DRAW_COLUMN_OUTLINES; }
		}

		if (option == STRING("ParagraphOutlines"))
		{
			if (CBool::Parse(value, NULL)) { g_dwDisplayOptions |= RO_DRAW_PARAGRAPH_OUTLINES; }
		}

		if (option == STRING("LineOutlines"))
		{
			if (CBool::Parse(value, NULL)) { g_dwDisplayOptions |= RO_DRAW_LINE_OUTLINES; }
		}

		if (option == STRING("WordOutlines"))
		{
			if (CBool::Parse(value, NULL)) { g_dwDisplayOptions |= RO_DRAW_WORD_OUTLINES; }
		}

		if (option == STRING("Selection"))
		{
			if (CBool::Parse(value, NULL)) { g_dwDisplayOptions |= RO_DRAW_SELECTION; }
		}

		//[General options]
		//if (option == STRING("StartupPath"))
		//{
		//	int iStart = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("\"")) + 1;
		//	int iTextLength = FindChars(&sBuffer[uiCurChar + iStart], iLength - (uiCurChar + iStart), TEXT("\""));

		//	if (g_szStartupFilePath) { delete[] g_szStartupFilePath; g_szStartupFilePath = NULL; }

		//	g_szStartupFilePath = new TCHAR[iTextLength + 1];

		//	wcsncpy_s(g_szStartupFilePath, iTextLength + 1, &sBuffer[uiCurChar + iStart], iTextLength);
		//}

		//if (option == STRING("NewFileTemplatePath"))
		//{
		//	if (SUCCEEDED((iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
		//	{
		//		int iStart = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("\"")) + 1;
		//		int iTextLength = FindChars(&sBuffer[uiCurChar + iStart], iLength - (uiCurChar + iStart), TEXT("\""));

		//		if (g_szNewFileTemplatePath) { delete[] g_szNewFileTemplatePath; g_szNewFileTemplatePath = NULL; }

		//		g_szNewFileTemplatePath = new TCHAR[iTextLength + 1];

		//		wcsncpy_s(g_szNewFileTemplatePath, iTextLength + 1, &sBuffer[uiCurChar + iStart], iTextLength);
		//	}
		//}

		if (option == STRING("EmbedOptions"))
		{
			g_bEmbedOptions = CBool::Parse(value, NULL);
		}

		if (option == STRING("DumpDebugInfo"))
		{
			g_bDumpDebugInfo = CBool::Parse(value, NULL);
		}

		if (option == STRING("CodeViewVisible"))
		{
			g_bCodeViewVisible = CBool::Parse(value, NULL);
		}

		if (option == STRING("InfoViewVisible"))
		{
			g_bInfoViewVisible = CBool::Parse(value, NULL);
		}

		if (option == STRING("CodeViewX"))
		{
			g_CodeViewWindowRect.left = CInt::Parse(value, NULL);
		}

		if (option == STRING("CodeViewY"))
		{
			g_CodeViewWindowRect.top = CInt::Parse(value, NULL);
		}

		if (option == STRING("InfoViewX"))
		{
			g_TextFrameInfoWindowRect.left = CInt::Parse(value, NULL);
		}

		if (option == STRING("InfoViewY"))
		{
			g_TextFrameInfoWindowRect.top = CInt::Parse(value, NULL);
		}

		if (option == STRING("CodeViewWidth"))
		{
			g_CodeViewWindowRect.right = CInt::Parse(value, NULL);
		}

		if (option == STRING("CodeViewHeight"))
		{
			g_CodeViewWindowRect.bottom = CInt::Parse(value, NULL);
		}
	}
 
	return TRUE;
}

HRESULT SaveOptions(_IN HANDLE hFile, _IN bool bEmbed)
{
	if (hFile == INVALID_HANDLE_VALUE) { return E_INVALIDARG; }

	TCHAR	szBuffer[2048];
	INT	uiBufferSize = 2048;

	DWORD	dwBytesWritten = 0;

	if (bEmbed)	{ wcscpy_s(szBuffer, uiBufferSize, TEXT("<!--TestAppSettings ")); }
	else			{ wcscpy_s(szBuffer, uiBufferSize, TEXT("[TestApp v1.000r]\r\n\r\n")); } 

	//Save text-frame options
	if (!bEmbed) 
	{
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s"), szBuffer, TEXT("[Text-frame options]\r\n\r\n"));										

		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("WindowX="),	g_TextFrameWindowRect.left,	TEXT("px\r\n"));	
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("WindowY="),	g_TextFrameWindowRect.top,	TEXT("px\r\n\r\n"));	
	}

	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("Width="),		g_TextFrameRect.right,		TEXT("em"));	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); } 
	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("Height="),	g_TextFrameRect.bottom,	TEXT("em"));	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); } 
	
	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("Insets="),	g_fInsets,					TEXT("em"));	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); } 

	//Save general options
	if (!bEmbed) 
	{ 
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s"), szBuffer, TEXT("[General options]\r\n\r\n"));							
		
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("EmbedOptions="),	g_bEmbedOptions);	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); 
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("DumpDebugInfo="),	g_bDumpDebugInfo);	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); 
	}

	//Save display options
	if (!bEmbed) { swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s"), szBuffer, TEXT("[Display options]\r\n\r\n")); }

	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("FrameBorder="),			__min(1, g_dwDisplayOptions & RO_DRAW_TEXTFRAME_OUTLINE));	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); } 
	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("ColumnOutlines="),		__min(1, g_dwDisplayOptions & RO_DRAW_COLUMN_OUTLINES));		if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); } 
	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("ParagraphOutlines="),	__min(1, g_dwDisplayOptions & RO_DRAW_PARAGRAPH_OUTLINES));	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); } 
	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("LineOutlines="),		__min(1, g_dwDisplayOptions & RO_DRAW_LINE_OUTLINES));		if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); } 
	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("WordOutlines="),		__min(1, g_dwDisplayOptions & RO_DRAW_WORD_OUTLINES));		if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); } 
	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("Selection="),			__min(1, g_dwDisplayOptions & RO_DRAW_SELECTION));			if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT("")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); } 

	//Save window options
	if (!bEmbed) 
	{ 
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s"), szBuffer, TEXT("[Window options]\r\n\r\n"));
		
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("InfoViewVisible="),		IsWindowVisible(g_hTextFrameInfoWindow));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); 

		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("InfoViewX="),			g_TextFrameInfoWindowRect.left,	TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n"));
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("InfoViewY="),			g_TextFrameInfoWindowRect.top,	TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n"));

		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("CodeViewX="),			g_CodeViewWindowRect.left,		TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n"));
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("CodeViewY="),			g_CodeViewWindowRect.top,		TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n"));
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("CodeViewWidth="),		g_CodeViewWindowRect.right,		TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n"));
		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("CodeViewHeight="),	g_CodeViewWindowRect.bottom,	TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n"));
	}

	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT("-->")); }

	WriteFile(hFile, szBuffer, sizeof(TCHAR) * (DWORD)(int)wcslen(szBuffer), &dwBytesWritten, NULL);

	return S_OK;
}

void RedrawTextFrame(_IN HWND hWnd, _IN UpdateOptions eUpdateOptions)
{
	HDC hdc = GetDC(hWnd);

	RECT rt; GetClientRect(hWnd, &rt);

	//Create a second device context & a bitmap to paint to
	HBITMAP	hBitmap		= CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
	HDC		hBitmapDC	= CreateCompatibleDC(hdc);

	HBITMAP	hOldBitmap	= (HBITMAP)SelectObject(hBitmapDC, hBitmap);

	//Draw background for the text-frame
	SelectObject(hBitmapDC, GetStockObject(WHITE_PEN));
	Rectangle(hBitmapDC, 0, 0, rt.right, rt.bottom);

	//Call layout function
	g_Frame.LayOut(hBitmapDC, eUpdateOptions, TRUE, LO_EXPAND_EMPTY_LINES | LO_FIT_HORIZONTALLY | LO_FIT_VERTICALLY);

	//Call render function
	g_Frame.Render(hBitmapDC, NULL);

	//Copy rendered image
	BitBlt(hdc, 0, 0, rt.right, rt.bottom, hBitmapDC, 0, 0, SRCCOPY);  

	//Clean up
	SelectObject(hBitmapDC, hOldBitmap);
	DeleteObject(hBitmap);

	DeleteDC(hBitmapDC);

	ReleaseDC(hWnd, hdc);
}

LRESULT CALLBACK TextFrameProc(HWND hWnd, INT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;

	switch(message) 
	{
	case WM_LBUTTONDOWN:
		{
			g_StartPoint.x = GET_X_LPARAM(lParam);
			g_StartPoint.y = GET_Y_LPARAM(lParam);

			if (SUCCEEDED(g_Frame.CursorPosToCharacterPos(((INT)GET_X_LPARAM(lParam) * g_fScreen2Point) / 1000, ((INT)GET_Y_LPARAM(lParam) * g_fScreen2Point) / 1000, &g_iSelectionStart)))
			{
				g_bDragging = TRUE;
				SetCapture(hWnd);
			}

			break;
		}

	case WM_MOUSEMOVE:
		{
			g_MousePoint.x = GET_X_LPARAM(lParam);
			g_MousePoint.y = GET_Y_LPARAM(lParam);

			if (g_bDragging)
			{
				g_Frame.CursorPosToCharacterPos(((INT)GET_X_LPARAM(lParam) * g_fScreen2Point) / 1000, ((INT)GET_Y_LPARAM(lParam) * g_fScreen2Point) / 1000, &g_iSelectionEnd);

				g_Frame.ClearSelection();

				if (g_iSelectionStart > g_iSelectionEnd)	{ g_Frame.AddSelectionRange(g_iSelectionEnd, g_iSelectionStart - g_iSelectionEnd + 1); }
				else										{ g_Frame.AddSelectionRange(g_iSelectionStart, g_iSelectionEnd - g_iSelectionStart + 1); }

				RedrawTextFrame(hWnd, UO_UPDATE_SELECTION);
			}

			break;
		}

	case WM_LBUTTONUP:
		{
			if ((g_StartPoint.x == GET_X_LPARAM(lParam)) && (g_StartPoint.y == GET_Y_LPARAM(lParam)))
			{
				if (g_Frame.IsCursorInText(((INT)GET_X_LPARAM(lParam) * g_fScreen2Point) / 1000, ((INT)GET_Y_LPARAM(lParam) * g_fScreen2Point) / 1000))
				{
					if ((abs((int)g_iSelectionEnd - (int)g_iSelectionStart)) == 1)
					{
						if (g_Frame.IsCursorInSelection(((INT)GET_X_LPARAM(lParam) * g_fScreen2Point) / 1000, ((INT)GET_Y_LPARAM(lParam) * g_fScreen2Point) / 1000))
						{
							g_Frame.ClearSelection();
							g_iSelectionEnd = g_iSelectionStart;
			
							RedrawTextFrame(hWnd, UO_UPDATE_SELECTION);
						}
						else
						{
							g_Frame.ClearSelection();
							g_Frame.AddSelectionRange(g_iSelectionStart, 1);
							g_iSelectionEnd = g_iSelectionStart + 1;
			
							RedrawTextFrame(hWnd, UO_UPDATE_SELECTION);
						}
					}
					else
					{
						g_Frame.ClearSelection();
						g_Frame.AddSelectionRange(g_iSelectionStart, 1);
						g_iSelectionEnd = g_iSelectionStart + 1;
	
						RedrawTextFrame(hWnd, UO_UPDATE_SELECTION);
					}
				}
				else
				{
					g_Frame.ClearSelection();
					g_iSelectionEnd = g_iSelectionStart;

					RedrawTextFrame(hWnd, UO_UPDATE_SELECTION);
				}
			}
	
			ReleaseCapture();
			g_bDragging = FALSE;

			break;
		}

	case WM_CREATE:
		{
			break;
		}

	case WM_SIZING:
		{
			if (g_bLockTextFrameDimensions)
			{
				*((RECT*)lParam) = g_TextFrameWindowRect;
	
				((RECT*)lParam)->right		+= g_TextFrameWindowRect.left;
				((RECT*)lParam)->bottom		+= g_TextFrameWindowRect.top;
				return TRUE;
			}

		//	if ((((RECT*)lParam)->right - ((RECT*)lParam)->left) < 170)
		//	{
		//		((RECT*)lParam)->right = ((RECT*)lParam)->left + 170;
		//	}

			if ((((RECT*)lParam)->bottom - ((RECT*)lParam)->top) < 50)
			{
				((RECT*)lParam)->bottom = ((RECT*)lParam)->top + 50;
			}

//			break;
		}

	case WM_SIZE:
		{
			bool bDoResize = FALSE;

			GetWindowRect(hWnd, &g_TextFrameWindowRect);
	
			g_TextFrameWindowRect.right		-= g_TextFrameWindowRect.left;
			g_TextFrameWindowRect.bottom	-= g_TextFrameWindowRect.top;

	//		if (g_TextFrameWindowRect.right < 170) { g_TextFrameWindowRect.right = 170; bDoResize = TRUE; }
			if (g_TextFrameWindowRect.bottom < 50) { g_TextFrameWindowRect.bottom = 50; bDoResize = TRUE; }

		/*	if (bDoResize)
			{
				//Resize window & beep to indicate unsuccessful resizing
				SetWindowPos(hWnd, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER); 
				MessageBeep(MB_ICONERROR);
			}*/
		}
	case WM_MOVE:
		{
			GetWindowRect(hWnd, &g_TextFrameWindowRect);

			g_TextFrameWindowRect.right		-= g_TextFrameWindowRect.left;
			g_TextFrameWindowRect.bottom	-= g_TextFrameWindowRect.top;

			HDC hdc = GetDC(hWnd);

			//Obtain the ratio used to covert between screen & world units (in this case points)
			INT	uiDPI = GetDeviceCaps(hdc, LOGPIXELSY);
			g_fScreen2Point = (72 * 1000) / uiDPI;
			g_fPoint2Screen = (uiDPI * 1000) / 72;

			ReleaseDC(hWnd, hdc);

			if (g_bSkipResizing) { g_bSkipResizing = FALSE; return TRUE; }

			//Update text-frame rectangle
			RECT rt; GetClientRect(hWnd, &rt);

			g_TextFrameRect.left		= (rt.left		* g_fScreen2Point) + g_fInsets;
			g_TextFrameRect.top		= (rt.top		* g_fScreen2Point) + g_fInsets;
			g_TextFrameRect.right		= (rt.right	* g_fScreen2Point) - g_fInsets * 2;
			g_TextFrameRect.bottom		= (rt.bottom	* g_fScreen2Point) - g_fInsets * 2;

			SendMessage(g_hTextFrameInfoWindow, UPDATE_DIMENSIONS, 0, 0);
			break;
		}

	case WM_SYSCOMMAND:
		{
			if (wParam == SC_CLOSE) 
			{
				SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDM_EXIT, 0), 0);
				return 0;
			}

			return DefWindowProc(hWnd, message, wParam, lParam);
		}

	case WM_COMMAND:
		{
			wmId	= LOWORD(wParam); 
			wmEvent	= HIWORD(wParam); 

		//Parse the menu selections:
			switch(wmId)
			{
			case ID_WINDOWS_CODEVIEW:
				{
					HMENU hMenu = GetMenu(hWnd);
					hMenu = GetSubMenu(hMenu, 1);

					MENUITEMINFO ItemInfo;
					ItemInfo.cbSize = sizeof(MENUITEMINFO);
					ItemInfo.fMask = MIIM_STATE;

					GetMenuItemInfo(hMenu, ID_WINDOWS_CODEVIEW, FALSE, &ItemInfo);

					if (ItemInfo.fState & MFS_CHECKED)
					{
						CheckMenuItem(hMenu, ID_WINDOWS_CODEVIEW, MF_BYCOMMAND | MF_UNCHECKED);
					}
					else
					{
						CheckMenuItem(hMenu, ID_WINDOWS_CODEVIEW, MF_BYCOMMAND | MF_CHECKED);
					}

					break;
				}
			case ID_WINDOWS_FRAME_INFO:
				{
					HMENU hMenu = GetMenu(hWnd);
					hMenu = GetSubMenu(hMenu, 1);

					MENUITEMINFO ItemInfo;
					ItemInfo.cbSize = sizeof(MENUITEMINFO);
					ItemInfo.fMask = MIIM_STATE;

					GetMenuItemInfo(hMenu, ID_WINDOWS_FRAME_INFO, FALSE, &ItemInfo);

					if (ItemInfo.fState & MFS_CHECKED)
					{
						CheckMenuItem(hMenu, ID_WINDOWS_FRAME_INFO, MF_BYCOMMAND | MF_UNCHECKED);
						ShowWindow(g_hTextFrameInfoWindow, SW_HIDE);
					}
					else
					{
						CheckMenuItem(hMenu, ID_WINDOWS_FRAME_INFO, MF_BYCOMMAND | MF_CHECKED);
						ShowWindow(g_hTextFrameInfoWindow, SW_SHOW);
					}

					break;
				}

			case IDM_ABOUT:          { DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About); break; }
			case ID_OPTIONS_DISPLAY: { DialogBox(g_hInst, (LPCTSTR)IDD_OPTIONS_DISPLAY, hWnd, (DLGPROC)DisplayOptions); break; }
			case ID_OPTIONS_GENERAL: { DialogBox(g_hInst, (LPCTSTR)IDD_OPTIONS_GENERAL, hWnd, (DLGPROC)GeneralOptions); break; }

			case IDM_EXIT:	
				{ 
					TCHAR szBuffer[1024];

					//Get full path to the executable
					GetModuleFileName(NULL, szBuffer, 1024);

					//Truncate file name
					PathRemoveFileSpec(szBuffer);

					//Append new name
					wcscat_s(szBuffer, TEXT("\\TestApp.cfg"));

					//Open destination file
					HANDLE hFile = CreateFile(szBuffer, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			
					if (hFile != INVALID_HANDLE_VALUE)
					{
						DWORD dwBytesWritten = 0;

						WriteFile(hFile, TEXT("\xfeff") , sizeof(TCHAR), &dwBytesWritten, NULL);

						SaveOptions(hFile, FALSE);

						//Close file
						CloseHandle(hFile);
					}

					DestroyWindow(hWnd);
					break; 
				}

			case ID_FILE_PRINT:
				{
					DOCINFO docInfo;
					ZeroMemory(&docInfo, sizeof(DOCINFO));

					docInfo.cbSize = sizeof(DOCINFO);

					docInfo.lpszDocName = TEXT("Test");

					//Create a PRINTDLG structure, and initialize the appropriate fields.
					PRINTDLG printDlg;
					ZeroMemory(&printDlg, sizeof(PRINTDLG));
					
					printDlg.lStructSize = sizeof(PRINTDLG);
					printDlg.Flags = PD_RETURNDC;

					//Display a print dialog box.
					if (PrintDlg(&printDlg))
					{
						StartDoc(printDlg.hDC, &docInfo);
						StartPage(printDlg.hDC);

						//Call layout function
						g_Frame.LayOut(printDlg.hDC, UO_UPDATE_ALL, TRUE, LO_EXPAND_EMPTY_LINES | LO_FIT_HORIZONTALLY | LO_FIT_VERTICALLY);

						//Call render function
						g_Frame.Render(printDlg.hDC, NULL);

						EndPage(printDlg.hDC);
						EndDoc(printDlg.hDC); 
					}

					if (printDlg.hDevMode)  { GlobalFree(printDlg.hDevMode); }
					if (printDlg.hDevNames)	{ GlobalFree(printDlg.hDevNames); }
					if (printDlg.hDC)       { DeleteDC(printDlg.hDC); }

					break;
				}

			default: { return DefWindowProc(hWnd, message, wParam, lParam); }
			}

			break;
		}

	case WM_PAINT:
		{
			HDC hdc;

			FILE* pFile = NULL; 
			
			if (g_bDumpDebugInfo)
			{
				TCHAR szExecutablePath[MAX_PATH + 1];
				GetModuleFileName(NULL, szExecutablePath, MAX_PATH);
				PathRemoveFileSpec(szExecutablePath);

				PathAppend(szExecutablePath, TEXT("\\DebugDump.txt"));

				_wfopen_s(&pFile, szExecutablePath, TEXT("w"));
			}

			hdc = BeginPaint(hWnd, &ps);

			//Obtain the ratio used to covert between screen & world units (in this case points)
			INT	uiDPI = GetDeviceCaps(hdc, LOGPIXELSY);
			g_fScreen2Point = (72 * 1000) / uiDPI;
			g_fPoint2Screen = (uiDPI * 1000) / 72;

			//Set the new rectangle in points
			//TODO: g_Frame.Unit(UNIT_EP);
			g_Frame.Rect(Rect4i(g_TextFrameRect.left, g_TextFrameRect.top, g_TextFrameRect.right, g_TextFrameRect.bottom));

			//Set rendering options
			g_Frame.RenderOptions(g_dwDisplayOptions | RO_QUALITY_NORMAL);

			if (pFile) 
			{
			//Dump text-frame rectangle
				fwprintf(pFile, TEXT("%s\n%f %f %f %f\n"), TEXT("[text-frame rect]"), g_TextFrameRect.left, g_TextFrameRect.top, g_TextFrameRect.right, g_TextFrameRect.bottom);
				fflush(pFile);
			}

			if (pFile) 
			{
				//Dump text-frame text content
				//fwprintf(pFile, TEXT("\n%s\n%s"), TEXT("[text-frame content]"), g_Frame.Text());
				fflush(pFile);
			}

			//Close dump report
			if (pFile) { fclose(pFile); }

		/*	if (g_bDisableStyles)
			{
				//Delete all style entries, but NOT the styles themselves 
				g_Frame.SetStyleType(STYLE_TEXTFRAME);
				g_Frame.ClearAllStyles(0, -1);

				g_Frame.SetStyleType(STYLE_PARAGRAPH);
				g_Frame.ClearAllStyles(0, -1);
				g_Frame.ClearDefaultStyle();

				g_Frame.SetStyleType(STYLE_CHARACTER);
				g_Frame.ClearAllStyles(0, -1);
				g_Frame.ClearDefaultStyle();
			}*/

			RECT rt; GetClientRect(hWnd, &rt);

			//Create a second device context & a bitmap to paint to
			HBITMAP	hBitmap		= CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
			HDC		hBitmapDC	= CreateCompatibleDC(hdc);

			HBITMAP	hOldBitmap	= (HBITMAP)SelectObject(hBitmapDC, hBitmap);

			//Draw background for the text-frame
			SelectObject(hBitmapDC, GetStockObject(WHITE_PEN));
			Rectangle(hBitmapDC, 0, 0, rt.right, rt.bottom);

			g_Frame.OutputScale(100);

			//Call layout function
			g_Frame.LayOut(hBitmapDC, UO_UPDATE_ALL, TRUE, LO_EXPAND_EMPTY_LINES | LO_FIT_HORIZONTALLY | LO_FIT_VERTICALLY);

			RECT	l_RectF = { 0, 0, 0, 0 };
			RECT	l_Rect = { 0, 0, 0, 0 };

			GetClientRect(hWnd, &l_Rect);
		
			l_RectF.left	= (l_Rect.right * g_fScreen2Point) / 2000;
			l_RectF.top		= (l_Rect.bottom * g_fScreen2Point) / 2000;

			l_RectF.right	= 50;
			l_RectF.bottom	= 50;


			//Call render function
			g_Frame.Render(hBitmapDC, NULL);//&l_RectF);

			//Copy rendered image
			BitBlt(hdc, 0, 0, rt.right, rt.bottom, hBitmapDC, 0, 0, SRCCOPY);  

			//Clean up
			SelectObject(hBitmapDC, hOldBitmap);
			DeleteObject(hBitmap);

			DeleteDC(hBitmapDC);

			EndPaint(hWnd, &ps);

			//Update statistics
			SendMessage(g_hTextFrameInfoWindow, UPDATE_CONTENT_STATS, 0, 0);
			break;
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

	default: { return DefWindowProc(hWnd, message, wParam, lParam); }
	}
	return 0;
}

LRESULT CALLBACK DisplayOptions(HWND hDlg, INT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch(message)
	{
	case WM_INITDIALOG: 
		{
			if (g_dwDisplayOptions & RO_DRAW_TEXTFRAME_OUTLINE)	{ CheckDlgButton(hDlg, IDC_FRAME_BORDER,		BST_CHECKED); }
			if (g_dwDisplayOptions & RO_DRAW_COLUMN_OUTLINES)		{ CheckDlgButton(hDlg, IDC_COLUMN_BORDERS,		BST_CHECKED); }
			if (g_dwDisplayOptions & RO_DRAW_PARAGRAPH_OUTLINES)	{ CheckDlgButton(hDlg, IDC_PARAGRAPH_BORDERS,	BST_CHECKED); }
			if (g_dwDisplayOptions & RO_DRAW_LINE_OUTLINES)		{ CheckDlgButton(hDlg, IDC_LINE_BORDERS,		BST_CHECKED); }
			if (g_dwDisplayOptions & RO_DRAW_WORD_OUTLINES)		{ CheckDlgButton(hDlg, IDC_WORD_BORDERS,		BST_CHECKED); }
			if (g_dwDisplayOptions & RO_DRAW_SELECTION)			{ CheckDlgButton(hDlg, IDC_SELECTION,			BST_CHECKED); }

			return TRUE; 
		}

	case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK)
			{
				g_dwDisplayOptions = 0;

				if (IsDlgButtonChecked(hDlg, IDC_FRAME_BORDER) ==			BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_TEXTFRAME_OUTLINE; }
				if (IsDlgButtonChecked(hDlg, IDC_COLUMN_BORDERS) ==		BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_COLUMN_OUTLINES; }
				if (IsDlgButtonChecked(hDlg, IDC_PARAGRAPH_BORDERS) ==	BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_PARAGRAPH_OUTLINES; }
				if (IsDlgButtonChecked(hDlg, IDC_LINE_BORDERS) ==			BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_LINE_OUTLINES; }
				if (IsDlgButtonChecked(hDlg, IDC_WORD_BORDERS) ==			BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_WORD_OUTLINES; }
				if (IsDlgButtonChecked(hDlg, IDC_SELECTION) ==			BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_SELECTION; }

				EndDialog(hDlg, LOWORD(wParam));

				RedrawWindow(g_hTextFrameWindow, NULL, NULL, RDW_INVALIDATE);
				return TRUE;
			}
				
			if (LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}

			break;
		}
	}
	return FALSE;
}

LRESULT CALLBACK GeneralOptions(HWND hDlg, INT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch(message)
	{
	case WM_INITDIALOG: 
		{
			if (g_bEmbedOptions)	{ CheckDlgButton(hDlg, IDC_EMBED_OPTIONS, BST_CHECKED); }
			if (g_bDumpDebugInfo)	{ CheckDlgButton(hDlg, IDC_DUMP_DEBUG_INFO, BST_CHECKED); }

			return TRUE; 
		}

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_STARTUP_FILE_BROWSE:
				{
					TCHAR szPath[MAX_PATH + 1];
					szPath[0] = 0;

					OPENFILENAME OpenFile;
					OpenFile.lStructSize		= sizeof(OPENFILENAME);
					OpenFile.hwndOwner			= hDlg;
					OpenFile.hInstance			= g_hInst;
					OpenFile.lpstrFilter		= TEXT("XML file (*.xml)\0*.xml\0All files (*.*)\0*.*\0\0");
					OpenFile.lpstrCustomFilter	= NULL;
					OpenFile.nMaxCustFilter		= 0;
					OpenFile.nFilterIndex		= 0;
					OpenFile.lpstrFile			= szPath;
					OpenFile.nMaxFile			= _MAX_PATH;
					OpenFile.lpstrFileTitle		= NULL;
					OpenFile.nMaxFileTitle		= 0;
					OpenFile.lpstrInitialDir	= NULL;
					OpenFile.lpstrTitle			= TEXT("Choose a startup file");
					OpenFile.nFileOffset		= 0;
					OpenFile.nFileExtension		= 0;
					OpenFile.lpstrDefExt		= NULL;
					OpenFile.lCustData			= NULL;
					OpenFile.lpfnHook			= NULL;
					OpenFile.lpTemplateName		= NULL;

					OpenFile.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;

					if (GetOpenFileName(&OpenFile)) 
					{
						TCHAR szRelativePath[MAX_PATH + 1];
						TCHAR szExecutablePath[MAX_PATH + 1];

						GetModuleFileName(NULL, szExecutablePath, MAX_PATH);

						if (PathRelativePathTo(szRelativePath, szExecutablePath, NULL, szPath, NULL))
						{
							SetDlgItemText(hDlg, IDC_STARTUP_FILE, szRelativePath); 
						}
						else
						{
							SetDlgItemText(hDlg, IDC_STARTUP_FILE, szPath); 
						}
					}

					break;
				}

			case IDC_NEW_FILE_BROWSE:
				{
					TCHAR szPath[MAX_PATH + 1];
					szPath[0] = 0;

					OPENFILENAME OpenFile;
					OpenFile.lStructSize		= sizeof(OPENFILENAME);
					OpenFile.hwndOwner			= hDlg;
					OpenFile.hInstance			= g_hInst;
					OpenFile.lpstrFilter		= TEXT("XML file (*.xml)\0*.xml\0All files (*.*)\0*.*\0\0");
					OpenFile.lpstrCustomFilter	= NULL;
					OpenFile.nMaxCustFilter		= 0;
					OpenFile.nFilterIndex		= 0;
					OpenFile.lpstrFile			= szPath;
					OpenFile.nMaxFile			= _MAX_PATH;
					OpenFile.lpstrFileTitle		= NULL;
					OpenFile.nMaxFileTitle		= 0;
					OpenFile.lpstrInitialDir	= NULL;
					OpenFile.lpstrTitle			= TEXT("Choose a startup file");
					OpenFile.nFileOffset		= 0;
					OpenFile.nFileExtension		= 0;
					OpenFile.lpstrDefExt		= NULL;
					OpenFile.lCustData			= NULL;
					OpenFile.lpfnHook			= NULL;
					OpenFile.lpTemplateName		= NULL;

					OpenFile.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;

					if (GetOpenFileName(&OpenFile)) 
					{
						TCHAR szRelativePath[MAX_PATH + 1];
						TCHAR szExecutablePath[MAX_PATH + 1];

						GetModuleFileName(NULL, szExecutablePath, MAX_PATH);

						if (PathRelativePathTo(szRelativePath, szExecutablePath, NULL, szPath, NULL))
						{
							SetDlgItemText(hDlg, IDC_NEW_FILE, szRelativePath); 
						}
						else
						{
							SetDlgItemText(hDlg, IDC_NEW_FILE, szPath); 
						}
					}

					break;
				}

			case IDCANCEL:
				{
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
				}

			case IDOK:
				{
					if (IsDlgButtonChecked(hDlg, IDC_EMBED_OPTIONS)	== BST_CHECKED) { g_bEmbedOptions = TRUE; }	else { g_bEmbedOptions = FALSE; }
					if (IsDlgButtonChecked(hDlg, IDC_DUMP_DEBUG_INFO)	== BST_CHECKED) { g_bDumpDebugInfo = TRUE; }	else { g_bDumpDebugInfo = FALSE; }

					EndDialog(hDlg, LOWORD(wParam));

					RedrawWindow(g_hTextFrameWindow, NULL, NULL, RDW_INVALIDATE);
					return TRUE;
				}
			}

			break;
		}
	}
	return FALSE;
}

//Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, INT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch(message)
	{
	case WM_INITDIALOG: 
		{
		/*	DLLGETVERSIONPROC DllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(g_hTextRender, "DllGetVersion");
			
			if (DllGetVersion)
			{
				DLLVERSIONINFO VersionInfo;

				//DllGetVersion()
			}

			TCHAR szBuffer[512];
			GetModuleFileName(g_hTextRender, szBuffer, 512);
			
			void* vpVersionData = malloc(GetFileVersionInfoSize(szBuffer, NULL));
			GetFileVersionInfo(szBuffer, NULL, GetFileVersionInfoSize(szBuffer, NULL), vpVersionData);

			TCHAR szVersionString[16];
	/		VerQueryValue(vpVersionData, TEXT("\\StringFileInfo\\080004b0\\ProductVersion"), (LPVOID*)&szVersionString, NULL);
	
		//	SetDlgItemText(hDlg, IDC_TEXT_RENDER_VERSION, szVersionString);
*/
			return TRUE; 
		}

	case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
		}
	}
	return FALSE;
}

LRESULT CALLBACK TextFrameInfoProc(HWND hDlg, INT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	int wmId, wmEvent;

	switch(message)
	{
	case WM_SHOWWINDOW: { g_bInfoViewVisible = (BOOL)wParam; break; }

	case WM_SIZE:
	case WM_MOVE:
		{
			GetWindowRect(hDlg, &g_TextFrameInfoWindowRect);
			break;
		}

	case WM_INITDIALOG: 
		{
			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Adjust height to fit contents."));
			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_SETITEMDATA, 0, 0);

			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Adjust width to fit widest word."));
			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_SETITEMDATA, 1, 0);

			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Adjust size to fit all words."));
			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_SETITEMDATA, 2, 0);

			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Adjust size to fit lines without wrapping."));
			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_SETITEMDATA, 3, 0);

			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Adjust width to fit contents."));
			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_SETITEMDATA, 4, 0);
	
			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Run test 01."));

			SetDlgItemTextA(hDlg, IDC_MEASUREMENTS_INSETS, "500ep");

			return TRUE; 
		}

	case UPDATE_CONTENT_STATS:
		{
			SetDlgItemInt(hDlg, IDC_CHARACTER_COUNT, g_Frame.ContentStats(STATS_CHARACTERS_FIT), FALSE);
			SetDlgItemInt(hDlg, IDC_WORD_COUNT,      g_Frame.ContentStats(STATS_WORDS_FIT),      FALSE);
			SetDlgItemInt(hDlg, IDC_LINE_COUNT,      g_Frame.ContentStats(STATS_LINES),          FALSE);
			SetDlgItemInt(hDlg, IDC_PARAGRAPH_COUNT, g_Frame.ContentStats(STATS_PARAGRAPHS_FIT), FALSE);

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE);
			break;
		}

	case UPDATE_DIMENSIONS:
		{
			char szBuffer[64];

			_itoa_s(g_TextFrameRect.right, szBuffer, 64, 10);
			strcat_s(szBuffer, 64, "ep");

			SetDlgItemTextA(hDlg, IDC_MEASUREMENTS_WIDTH, szBuffer);

			_itoa_s(g_TextFrameRect.bottom, szBuffer, 64, 10);
			strcat_s(szBuffer, 64, "ep");

			SetDlgItemTextA(hDlg, IDC_MEASUREMENTS_HEIGHT, szBuffer);

			_itoa_s(g_fInsets, szBuffer, 64, 10);
			strcat_s(szBuffer, 64, "ep");

			SetDlgItemTextA(hDlg, IDC_MEASUREMENTS_INSETS, szBuffer);

			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE);
			break;
		}

	case EN_ENTER:
		{
			switch(wParam)
			{
			case IDC_MEASUREMENTS_WIDTH:
				{
					char szBuffer[65];
					GetDlgItemTextA(hDlg, IDC_MEASUREMENTS_WIDTH, szBuffer, 64);
					g_TextFrameRect.right = atoi(szBuffer);

					UpdateTextFrameWindowRect();

					SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
					break;
				}

			case IDC_MEASUREMENTS_HEIGHT:
				{
					char szBuffer[65];
					GetDlgItemTextA(hDlg, IDC_MEASUREMENTS_HEIGHT, szBuffer, 64);

					g_TextFrameRect.bottom = atoi(szBuffer);

					UpdateTextFrameWindowRect();

					SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
					break;
				}

			case IDC_MEASUREMENTS_INSETS:
				{
					char szBuffer[65];
					GetDlgItemTextA(hDlg, IDC_MEASUREMENTS_INSETS, szBuffer, 64);

					g_fInsets = atoi(szBuffer);

					UpdateTextFrameWindowRect();

					SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
					break;
				}
			}

			break;
		}

	case WM_COMMAND:
		{
			wmId	= LOWORD(wParam);
			wmEvent	= HIWORD(wParam);
			
			//Parse the menu selections:
			switch(wmId)
			{
			case IDC_LOCK_DIMENSIONS:
				{
					g_bLockTextFrameDimensions = !g_bLockTextFrameDimensions;  
					break;
				}
			case IDC_RESIZE:
				{
					//switch(SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_GETCURSEL, 0, 0))
					//{
					//case 5:
					//	{
					//		HDC hdc = GetDC(g_hTextFrameWindow);

					//		//g_Frame.SetUnit(UNIT_EP);

					//		INT l_iWidth			= g_TextFrameRect.right;
					//		INT l_iMeasuredHeight	= 0;
					//		INT l_iMeasuredWidth	= 0;

					//		TCHAR l_szCaption[32];

					//		ResetTime01();

					//		for (INT i = 0; i < 10; i++)
					//		{
					//			swprintf_s(l_szCaption, 32, TEXT("Pass %d"), i);
					//			SetWindowText(hDlg, l_szCaption);
					//					
					//			g_Frame.RectBasedOnFrameWidth(hdc, l_iWidth + i, &g_TextFrameRect);

					//			l_iMeasuredHeight = g_TextFrameRect.bottom;

					//			g_Frame.MeasureTextFrame(hdc, MO_GET_WIDTH_USING_GIVEN_HEIGHT, (tRect*)&g_TextFrameRect);

					//			if (l_iMeasuredHeight != g_TextFrameRect.bottom)
					//			{
					//				MessageBoxW(NULL, TEXT("Error 01"), TEXT(""), MB_ICONERROR);
					//			}

					//			l_iMeasuredWidth = g_TextFrameRect.right;

					//			g_Frame.MeasureTextFrame(hdc, MO_GET_HEIGHT_USING_GIVEN_WIDTH, (tRect*)&g_TextFrameRect);

					//			if ((l_iMeasuredHeight != g_TextFrameRect.bottom) || (l_iMeasuredWidth != g_TextFrameRect.right))
					//			{
					//				MessageBoxW(NULL, TEXT("Error 02"), TEXT(""), MB_ICONERROR);
					//			}

					//			g_Frame.MeasureTextFrame(hdc, MO_GET_WIDTH_USING_GIVEN_HEIGHT, (tRect*)&g_TextFrameRect);

					//			if ((l_iMeasuredHeight != g_TextFrameRect.bottom) || (l_iMeasuredWidth != g_TextFrameRect.right))
					//			{
					//				MessageBoxW(NULL, TEXT("Error 03"), TEXT(""), MB_ICONERROR);
					//			}
					//		}

					//		swprintf_s(l_szCaption, 32, TEXT("Time spent: %f"), GetTime01());
					//		SetWindowText(hDlg, l_szCaption);

					//		ReleaseDC(g_hTextFrameWindow, hdc);
					//		g_bSkipResizing = TRUE;
					//		UpdateTextFrameWindowRect();

					//		SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
					//		RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
					//		break;		
					//	}
					//case 0:
					//	{
					//		HDC hdc = GetDC(g_hTextFrameWindow);

					//		//g_Frame.SetUnit(UNIT_EP);
					//		g_Frame.MeasureTextFrame(hdc, MO_GET_HEIGHT_USING_GIVEN_WIDTH, (tRect*)&g_TextFrameRect);

					//		ReleaseDC(g_hTextFrameWindow, hdc);
					//		g_bSkipResizing = TRUE;
					//		UpdateTextFrameWindowRect();

					//		SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
					//		RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
					//		break;
					//	}
					//case 1:
					//	{
					//		HDC hdc = GetDC(g_hTextFrameWindow);

					//		RECT tempRectF = g_TextFrameRect;
					//	
					//		//g_Frame.SetUnit(UNIT_EP);
					//		g_Frame.MeasureTextFrame(hdc, MO_GET_RECT_USING_WIDEST_WORD, (tRect*)&tempRectF);
					//		
					//		g_TextFrameRect.right = tempRectF.right;

					//		ReleaseDC(g_hTextFrameWindow, hdc);
					//		g_bSkipResizing = TRUE;
					//		UpdateTextFrameWindowRect();

					//		SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
					//		RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
					//		break;
					//	}
					//case 2:
					//	{
					//		HDC hdc = GetDC(g_hTextFrameWindow);

					//		//g_Frame.SetUnit(UNIT_EP);
					//		g_Frame.MeasureTextFrame(hdc, MO_GET_RECT_USING_WIDEST_WORD, (tRect*)&g_TextFrameRect);

					//		ReleaseDC(g_hTextFrameWindow, hdc);
					//		g_bSkipResizing = TRUE;
					//		UpdateTextFrameWindowRect();

					//		SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
					//		RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
					//		break;
					//	}
					//case 3:
					//	{
					//		HDC hdc = GetDC(g_hTextFrameWindow);

					//		g_Frame.SetUnit(UNIT_EP);
					//		g_Frame.MeasureTextFrame(hdc, MO_GET_RECT_USING_NO_WRAP, (tRect*)&g_TextFrameRect);

					//		ReleaseDC(g_hTextFrameWindow, hdc);
					//		g_bSkipResizing = TRUE;
					//		UpdateTextFrameWindowRect();

					//		SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
					//		RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
					//		break;
					//	}
					//case 4:
					//	{
					//		HDC hdc = GetDC(g_hTextFrameWindow);

					//		g_Frame.SetUnit(UNIT_EP);
					//		g_Frame.MeasureTextFrame(hdc, MO_GET_WIDTH_USING_GIVEN_HEIGHT, (tRect*)&g_TextFrameRect);

					//		ReleaseDC(g_hTextFrameWindow, hdc);
					//		g_bSkipResizing = TRUE;
					//		UpdateTextFrameWindowRect();

					//		SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
					//		RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
					//		break;
					//	}
					//}

					break;
				}

			case IDC_REDRAW:
				{
					HDC hdc = GetDC(g_hTextFrameWindow);

					RECT rt; GetClientRect(g_hTextFrameWindow, &rt);
					
					SelectObject(hdc, GetStockObject(WHITE_PEN));

					Rectangle(hdc, 0, 0, rt.right, rt.bottom);

//					g_Frame.LayOut(hdc, UO_UPDATE_ALL);
					g_Frame.Render(hdc, NULL);

					ReleaseDC(g_hTextFrameWindow, hdc);

					break;
				}
			}

			break;
		}

	case WM_SYSCOMMAND:
		{
			if (wParam == SC_CLOSE) 
			{
				HMENU hMenu = GetMenu(g_hTextFrameWindow);
				hMenu = GetSubMenu(hMenu, 1);

				CheckMenuItem(hMenu, ID_WINDOWS_FRAME_INFO, MF_BYCOMMAND | MF_UNCHECKED);
				ShowWindow(hDlg, SW_HIDE);
				return 0;
			}

			break;
		}

	}
	return FALSE;
}

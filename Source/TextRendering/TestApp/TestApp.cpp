#include "TestApp.h"

using namespace SCFBase;
using namespace SCFTimer;

////Handle to instance
//HINSTANCE	g_hInst;								
//
////Handles to windows
//HWND	g_hTextFrameWindow = NULL;
//
//HWND	g_hCodeViewTextBox = NULL;
//HWND	g_hCodeViewWindow = NULL;
//
//HWND	g_hTextFrameInfoWindow = NULL;
//
////Current file information
//LPTSTR	g_szFilePath = NULL;
//
////Additional paths
//LPTSTR	g_szStartupFilePath = NULL;
//LPTSTR	g_szNewFileTemplatePath = NULL;
//
////Options
//BOOL	g_bLockTextFrameDimensions = FALSE;
//BOOL	g_bDisableStyles = FALSE;
//BOOL	g_bEmbedOptions = TRUE;
//
//BOOL	g_bDumpDebugInfo = TRUE;
//
//DWORD	g_dwDisplayOptions = RO_DRAW_SELECTION | RO_DRAW_TEXTFRAME_OUTLINE | RO_DRAW_COLUMN_OUTLINES | RO_DRAW_PARAGRAPH_OUTLINES | RO_DRAW_LINE_OUTLINES | RO_DRAW_WORD_OUTLINES;
//
////Text-frame measurements
//RECT	g_TextFrameRectF;
//INT		g_fInsets = 5;
//
//BOOL	g_bCodeViewVisible = TRUE;
//BOOL	g_bInfoViewVisible = TRUE;
//
//RECT	g_TextFrameWindowRect;
//RECT	g_TextFrameInfoWindowRect;
//RECT	g_CodeViewWindowRect;
//
////Selection state
//INT	g_iSelectionStart = 0;
//INT	g_iSelectionEnd = 0;
//
//BOOL	g_bDragging = FALSE;
//
//POINT	g_MousePoint;
//POINT	g_StartPoint;
//
////Device/world conversion variables
//INT	g_fScreen2Point = 1;
//INT	g_fPoint2Screen = 1;
//
////Edit control sub-classing variables
//HWND	g_hEditOrigin		= NULL;
//WNDPROC	g_EditProcOrigin	= NULL;
//BOOL	g_bSkipResizing		= NULL;
//
//
////Global imported - runtime linked functions 
//LPTR_TextRenderStartup			TR_TextRenderStartup		= NULL;
//LPTR_TextRenderShutdown			TR_TextRenderShutdown		= NULL;
//
//LPTR_SetUnit					TR_SetUnit					= NULL;	
//LPTR_SetRect					TR_SetRect					= NULL;	
//
//LPTR_SetStyleType				TR_SetStyleType				= NULL;	
//LPTR_RemoveAllStyles			TR_RemoveAllStyles			= NULL;	
//
//LPTR_ExecuteXMLString			TR_ExecuteXMLString			= NULL;	
//
//LPTR_Render						TR_Render					= NULL;	
//
//LPTR_ValidateStyle				TR_ValidateStyle			= NULL;	
//
//LPTR_SetText					TR_SetText					= NULL;	
//
//LPTR_GetContentStats			TR_GetContentStats			= NULL;	
//
//LPTR_MeasureTextFrame			TR_MeasureTextFrame			= NULL;
//
//LPTR_SetRenderOptions			TR_SetRenderOptions			= NULL;
//LPTR_GetRenderOptions			TR_GetRenderOptions			= NULL;
//
//LPTR_CursorPosToCharacterPos	TR_CursorPosToCharacterPos	= NULL;
//
//LPTR_ClearDefaultStyle			TR_ClearDefaultStyle		= NULL;
//LPTR_ClearAllStyles				TR_ClearAllStyles			= NULL;
//LPTR_ClearStyle					TR_ClearStyle				= NULL;
//
//LPTR_ClearSelection				TR_ClearSelection			= NULL;
//LPTR_AddSelectionRange			TR_AddSelectionRange		= NULL;
//
//LPTR_IsCursorInSelection		TR_IsCursorInSelection		= NULL;
//LPTR_IsCursorInText				TR_IsCursorInText			= NULL;
//
//LPTR_LayOut						TR_LayOut					= NULL;
//
//LPTR_GetStringProperty			TR_GetStringProperty		= NULL;
//LPTR_GetStringPropertySize		TR_GetStringPropertySize	= NULL;
//
//LPTR_SelectStyle				TR_SelectStyle				= NULL;
//
//LPTR_SetComplexProperty			TR_SetComplexProperty		= NULL;
//LPTR_GetComplexProperty			TR_GetComplexProperty		= NULL;
//
//LPTR_AddStyle					TR_AddStyle					= NULL;
//LPTR_GetComplexPropertySize		TR_GetComplexPropertySize	= NULL;
//LPTR_SetOutputScale				TR_SetOutputScale			= NULL;
//
//HMODULE	g_hTextRender = NULL;
//
//HRESULT InitTextRender()
//{
////Try to load library
//	g_hTextRender = LoadLibrary(TEXT("TextRender.dll"));
//	if (!g_hTextRender) { MessageBox(NULL, TEXT("Couldn't load \"TextRender.dll\"!"), TEXT("Initialization error!"), MB_ICONERROR); return FALSE; }
//
////Extract function pointers
//	TR_TextRenderStartup		= (LPTR_TextRenderStartup)GetProcAddress(			g_hTextRender, "TR_TextRenderStartup");
//	TR_TextRenderShutdown		= (LPTR_TextRenderShutdown)GetProcAddress(			g_hTextRender, "TR_TextRenderShutdown");
//
//	TR_SetUnit					= (LPTR_SetUnit)GetProcAddress(						g_hTextRender, "TR_SetUnit");
//	TR_SetRect					= (LPTR_SetRect)GetProcAddress(						g_hTextRender, "TR_SetRect");
//
//	TR_SetStyleType				= (LPTR_SetStyleType)GetProcAddress(				g_hTextRender, "TR_SetStyleType");
//	TR_RemoveAllStyles			= (LPTR_RemoveAllStyles)GetProcAddress(				g_hTextRender, "TR_RemoveAllStyles");
//
//	TR_ExecuteXMLString			= (LPTR_ExecuteXMLString)GetProcAddress(			g_hTextRender, "TR_ExecuteXMLString");
//	TR_Render					= (LPTR_Render)GetProcAddress(						g_hTextRender, "TR_Render");
//
//	TR_ValidateStyle			= (LPTR_ValidateStyle)GetProcAddress(				g_hTextRender, "TR_ValidateStyle");
//
//	TR_SetText					= (LPTR_SetText)GetProcAddress(						g_hTextRender, "TR_SetText");
//
//	TR_GetContentStats			= (LPTR_GetContentStats)GetProcAddress(				g_hTextRender, "TR_GetContentStats");
//
//	TR_MeasureTextFrame			= (LPTR_MeasureTextFrame)GetProcAddress(			g_hTextRender, "TR_MeasureTextFrame");
//
//	TR_SetRenderOptions			= (LPTR_SetRenderOptions)GetProcAddress(			g_hTextRender, "TR_SetRenderOptions");
//	TR_GetRenderOptions			= (LPTR_GetRenderOptions)GetProcAddress(			g_hTextRender, "TR_GetRenderOptions");
//
//	TR_CursorPosToCharacterPos	= (LPTR_CursorPosToCharacterPos)GetProcAddress(		g_hTextRender, "TR_CursorPosToCharacterPos");
//
//	TR_ClearDefaultStyle		= (LPTR_ClearDefaultStyle)GetProcAddress(			g_hTextRender, "TR_ClearDefaultStyle");
//	TR_ClearAllStyles			= (LPTR_ClearAllStyles)GetProcAddress(				g_hTextRender, "TR_ClearAllStyles");
//	TR_ClearStyle				= (LPTR_ClearStyle)GetProcAddress(					g_hTextRender, "TR_ClearStyle");
//	
//	TR_ClearSelection			= (LPTR_ClearSelection)GetProcAddress(				g_hTextRender, "TR_ClearSelection");
//	TR_AddSelectionRange		= (LPTR_AddSelectionRange)GetProcAddress(			g_hTextRender, "TR_AddSelectionRange");
//
//	TR_IsCursorInSelection		= (LPTR_IsCursorInSelection)GetProcAddress(			g_hTextRender, "TR_IsCursorInSelection");
//	TR_IsCursorInText			= (LPTR_IsCursorInText)GetProcAddress(				g_hTextRender, "TR_IsCursorInText");
//	TR_LayOut					= (LPTR_LayOut)GetProcAddress(						g_hTextRender, "TR_LayOut");
//	
//	TR_GetStringProperty		= (LPTR_GetStringProperty)GetProcAddress(			g_hTextRender, "TR_GetStringProperty");
//	TR_GetStringPropertySize	= (LPTR_GetStringPropertySize)GetProcAddress(		g_hTextRender, "TR_GetStringPropertySize");
//
//	TR_SelectStyle				= (LPTR_SelectStyle)GetProcAddress(					g_hTextRender, "TR_SelectStyle");
//	TR_SetComplexProperty		= (LPTR_SetComplexProperty)GetProcAddress(			g_hTextRender, "TR_SetComplexProperty");
//	TR_GetComplexProperty		= (LPTR_GetComplexProperty)GetProcAddress(			g_hTextRender, "TR_GetComplexProperty");
//	
//	TR_AddStyle					= (LPTR_AddStyle)GetProcAddress(					g_hTextRender, "TR_AddStyle");
//	TR_GetComplexPropertySize	= (LPTR_GetComplexPropertySize)GetProcAddress(		g_hTextRender, "TR_GetComplexPropertySize");
//
//	TR_SetOutputScale			= (LPTR_SetOutputScale)GetProcAddress(				g_hTextRender, "TR_SetOutputScale");
//	
//	return TRUE;
//}
//
//int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//	UNREFERENCED_PARAMETER(lpCmdLine);
//	UNREFERENCED_PARAMETER(hPrevInstance);
//
// 	MSG msg;
//	HACCEL hAccelTable;
//
//	//Perform application initialization:
//	if (FAILED(RegisterClasses(hInstance)))		{ return E_FAIL; }
//	if (FAILED(InitInstance(hInstance, nCmdShow)))	{ return E_FAIL; }
//
//	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TESTAPP);
//
//	//Main message loop:
//	while (GetMessage(&msg, NULL, 0, 0)) 
//	{
//		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}
//
//	return (int) msg.wParam;
//}
//
//HRESULT RegisterClasses(HINSTANCE hInstance)
//{
//	WNDCLASSEX wcex;
//	wcex.cbSize = sizeof(WNDCLASSEX); 
//
//	wcex.style			= CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc	= (WNDPROC)TextFrameProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		= hInstance;
//	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TESTAPP);
//	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName	= (LPCTSTR)IDC_TESTAPP;
//	wcex.lpszClassName	= TEXT("TextFrameWindowClass");
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
//
//	if (!RegisterClassEx(&wcex)) { return E_FAIL; }
//
//	wcex.style			= CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc	= (WNDPROC)TextFrameInfoProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		= hInstance;
//	wcex.hIcon			= NULL;//LoadIcon(hInstance, (LPCTSTR)IDI_TESTAPP);
//	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName	= NULL;
//	wcex.lpszClassName	= TEXT("TextFrameInfoWindowClass");
//	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
//
//	if (!RegisterClassEx(&wcex)) { return E_FAIL; }
//
//	wcex.style			= CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc	= (WNDPROC)CodeViewProc;
//	wcex.cbClsExtra		= 0;
//	wcex.cbWndExtra		= 0;
//	wcex.hInstance		= hInstance;
//	wcex.hIcon			= NULL;//LoadIcon(hInstance, (LPCTSTR)IDI_TESTAPP);
//	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName	= NULL;
//	wcex.lpszClassName	= TEXT("CodeViewWindowClass");
//	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
//
//	if (!RegisterClassEx(&wcex)) { return E_FAIL; }
//
//	return S_OK;
//}
//
//
//LRESULT CALLBACK EditProcSubClass(HWND hWnd, INT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (uMsg == WM_KEYDOWN)
//	{
//		if ((int)wParam == VK_RETURN)
//		{ 
////			SendMessage(hWnd, EM_SETSEL, 0, -1);
//			SendMessage(GetParent(hWnd), EN_ENTER, GetWindowLongPtr(hWnd, GWL_ID), (LONG_PTR)hWnd);
//		
//			return FALSE;
//		}
//	}
//
//	return CallWindowProc(g_EditProcOrigin, hWnd, uMsg, wParam, lParam);
//}
//
//HRESULT InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//	UNREFERENCED_PARAMETER(nCmdShow);
//
//	if (!InitTextRender()) { return E_FAIL; }
//
//	InitTimerDef();
//	
////	LoadLibrary(TEXT("Riched20.dll"));
//
//	INITCOMMONCONTROLSEX initEx;
//	initEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
//	initEx.dwICC = ICC_WIN95_CLASSES;
//
//	InitCommonControlsEx(&initEx);
//
//	//Store instance handle in our global variable
//	g_hInst = hInstance; 
//
//	g_hTextFrameWindow = CreateWindowEx(NULL, TEXT("TextFrameWindowClass"), TEXT("Text Render Test Session"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 400, 300, NULL, NULL, hInstance, NULL);
//	if (!g_hTextFrameWindow) { return E_FAIL; }
//
//	GetWindowRect(g_hTextFrameWindow, &g_TextFrameWindowRect);
//
//	g_hEditOrigin = CreateWindowEx(NULL, TEXT("EDIT"), NULL, WS_CHILD, 0, 0, 0, 0, g_hTextFrameWindow, (HMENU)9992, hInstance, NULL);
//	g_EditProcOrigin = (WNDPROC)(DWORD)SetClassLongPtr(g_hEditOrigin, GCLP_WNDPROC, (LONG)(LONG_PTR)(WNDPROC)EditProcSubClass);
//
//	g_hCodeViewWindow	= CreateWindowEx(WS_EX_TOOLWINDOW, TEXT("CodeViewWindowClass"), TEXT("Code View"), WS_OVERLAPPED | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU , g_TextFrameWindowRect.left, g_TextFrameWindowRect.bottom, g_TextFrameWindowRect.right - g_TextFrameWindowRect.left, 250, g_hTextFrameWindow, NULL, hInstance, NULL);
//	if (!g_hCodeViewWindow) { return E_FAIL; }
//
//	g_hTextFrameInfoWindow	= CreateDialog(hInstance, MAKEINTRESOURCE(IDD_FRAME_INFO), g_hTextFrameWindow, (DLGPROC)TextFrameInfoProc);
//	if (!g_hTextFrameInfoWindow) { return E_FAIL; }
//
//	SetWindowPos(g_hTextFrameInfoWindow, g_hTextFrameWindow, g_TextFrameWindowRect.right, g_TextFrameWindowRect.top, 0, 0, SWP_NOSIZE);
//
//	TCHAR	szBuffer[2048];
//
//	//Get full path to the executable
//	GetModuleFileName(NULL, szBuffer, 2048);
//
//	//Truncate file name
//	PathRemoveFileSpec(szBuffer);
//
//	//Append new name
//	wcscat_s(szBuffer, TEXT("\\TestApp.cfg"));
//
//	//Open destination file
//	HANDLE hFile = CreateFile(szBuffer, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if (hFile != INVALID_HANDLE_VALUE)
//	{
//		DWORD dwBytesRead = 0;
//
//		ReadFile(hFile, szBuffer, sizeof(TCHAR) * 2048, &dwBytesRead, NULL);
//
//		LoadOptions(szBuffer, dwBytesRead / sizeof(TCHAR));
//
//		//Close file
//		CloseHandle(hFile);
//	}
//
//	ExecuteOptions();
//
//	if (PathIsRelative(g_szStartupFilePath))
//	{
//		GetModuleFileName(NULL, szBuffer, 2048);
//		PathRemoveFileSpec(szBuffer);
//
//		PathAppend(szBuffer, g_szStartupFilePath);
//	
//		LoadXMLCode(szBuffer);
//	}
//	else
//	{
//		LoadXMLCode(g_szStartupFilePath);
//	}
//
//	SetFocus(g_hTextFrameWindow);
//	
//	return S_OK;
//}
//
//void UpdateTextFrameWindowRect()
//{
//	RECT rt;
//	rt.left		= 0;
//	rt.top		= 0;
//	rt.right	= 1 + ((g_TextFrameRectF.right	+ g_fInsets * 2) * g_fPoint2Screen) / 1000000;
//	rt.bottom	= 1 + ((g_TextFrameRectF.bottom	+ g_fInsets * 2) * g_fPoint2Screen) / 1000000;
//
//	MENUBARINFO menuInfo;
//	menuInfo.cbSize = sizeof(MENUBARINFO);
//
//	GetMenuBarInfo(g_hTextFrameWindow, OBJID_MENU, 0, &menuInfo);
//
//	AdjustWindowRectEx(&rt, WS_OVERLAPPEDWINDOW, TRUE, NULL);
//
//	g_TextFrameWindowRect.right		= rt.right - rt.left;
//	g_TextFrameWindowRect.bottom	= rt.bottom - rt.top;
//}
//
//HRESULT LoadXMLCode(LPTSTR szPath)
//{
//	if (!szPath) { return E_INVALIDARG; }
//
//	//Try opening output file 
//	HANDLE hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if (hFile != INVALID_HANDLE_VALUE)
//	{
//		//Compute string length (UNICODE)
//		INT uiTextLength = GetFileSize(hFile, NULL) / sizeof(TCHAR) - 1;
//
//		//Prepare memory for string
//		LPTSTR szFileText = new TCHAR[uiTextLength + 1];
//		szFileText[0] = 0;
//
//		if (uiTextLength)
//		{
//			DWORD dwBytesRead = 0;
//			SetFilePointer(hFile, sizeof(TCHAR), 0, FILE_BEGIN);
//
//			//Read the settings embedded as comment in the XML file
//			ReadFile(hFile, szFileText, GetFileSize(hFile, NULL) - sizeof(TCHAR), &dwBytesRead, NULL);
//
//			//Close string
//			szFileText[uiTextLength] = 0;
//
//			LPTSTR szFoundString = wcsstr(szFileText, TEXT("<!--TestAppSettings "));
//
//			if (szFoundString)
//			{
//				TCHAR szEndStr[] = TEXT("-->");
//				TCHAR* subStrings[] = { szEndStr };
//
//				HRESULT hResLength = FindSubStrings(szFoundString, -1, subStrings, 1, NULL);
//
//				if (SUCCEEDED(hResLength)) 
//				{
//					LoadOptions(szFoundString, hResLength); 
//
//					UpdateTextFrameWindowRect();
//					SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
//
//					int iCharsToShift = (int)(int)wcslen(szFoundString) - hResLength;
//
//					for (int iChar = 0; iChar < iCharsToShift; iChar++)
//					{
//						szFoundString[iChar] = szFoundString[iChar + hResLength + 3];
//					}
//				}
//			}
//		}
//
//		if (g_szFilePath) { delete[] g_szFilePath; }
//
//		g_szFilePath = new TCHAR[lstrlen(szPath) + 1];
//		wcscpy_s(g_szFilePath, lstrlen(szPath) + 1, szPath);
//
//		//Set "Code View" edit box text
//		SetWindowText(g_hCodeViewTextBox, szFileText);
//
//		TCHAR szTmp[1024]; 
//		wcscpy_s(szTmp, 1024, g_szFilePath);
//		PathStripPath(szTmp);
//
//		LPTSTR szTitle = new TCHAR[lstrlen(szTmp) + lstrlen(TEXT("Code View (")) + 1 + 1];
//
//		wcscpy_s(szTitle, lstrlen(szTmp) + lstrlen(TEXT("Code View (")) + 1 + 1, TEXT("Code View ["));
//		wcscat_s(szTitle, lstrlen(szTmp) + lstrlen(TEXT("Code View (")) + 1 + 1, szTmp);
//		wcscat_s(szTitle, lstrlen(szTmp) + lstrlen(TEXT("Code View (")) + 1 + 1, TEXT("]"));
//
//		SetWindowText(g_hCodeViewWindow, szTitle);
//
//		delete[] szTitle;
//		delete[] szFileText;
//
//		SendMessage(g_hCodeViewWindow, WM_COMMAND, MAKEWPARAM(0, EN_UPDATE), 0);
//		RedrawWindow(g_hTextFrameWindow, NULL, NULL, RDW_INVALIDATE);
//
//		//Close file
//		CloseHandle(hFile);
//		
//		return S_OK;
//	}
//
//	return E_FAIL;
//}
//
//HRESULT ExecuteOptions()
//{
//	if (g_bCodeViewVisible)
//	{
//		HMENU hMenu = GetMenu(g_hTextFrameWindow);
//		hMenu = GetSubMenu(hMenu, 1);
//
//		CheckMenuItem(hMenu, ID_WINDOWS_CODEVIEW, MF_BYCOMMAND | MF_CHECKED);
//
//		SetWindowPos(g_hCodeViewWindow, NULL, g_CodeViewWindowRect.left, g_CodeViewWindowRect.top, g_CodeViewWindowRect.right, g_CodeViewWindowRect.bottom, SWP_NOZORDER);
//
//		ShowWindow(g_hCodeViewWindow, SW_SHOW);
//		UpdateWindow(g_hCodeViewWindow);
//	}
//	else
//	{
//		HMENU hMenu = GetMenu(g_hTextFrameWindow);
//		hMenu = GetSubMenu(hMenu, 1);
//
//		CheckMenuItem(hMenu, ID_WINDOWS_CODEVIEW, MF_BYCOMMAND | MF_UNCHECKED);
//
//		SetWindowPos(g_hCodeViewWindow, NULL, g_CodeViewWindowRect.left, g_CodeViewWindowRect.top, g_CodeViewWindowRect.right, g_CodeViewWindowRect.bottom, SWP_NOZORDER);
//
//		ShowWindow(g_hCodeViewWindow, SW_HIDE);
//	}
//
//	if (g_bInfoViewVisible)
//	{
//		HMENU hMenu = GetMenu(g_hTextFrameWindow);
//		hMenu = GetSubMenu(hMenu, 1);
//
//		CheckMenuItem(hMenu, ID_WINDOWS_FRAME_INFO, MF_BYCOMMAND | MF_CHECKED);
//
//		SetWindowPos(g_hTextFrameInfoWindow, NULL, g_TextFrameInfoWindowRect.left, g_TextFrameInfoWindowRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
//
//		ShowWindow(g_hTextFrameInfoWindow, SW_SHOW);
//		UpdateWindow(g_hTextFrameInfoWindow);
//	}
//	else
//	{
//		HMENU hMenu = GetMenu(g_hTextFrameWindow);
//		hMenu = GetSubMenu(hMenu, 1);
//
//		SetWindowPos(g_hTextFrameInfoWindow, NULL, g_TextFrameInfoWindowRect.left, g_TextFrameInfoWindowRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
//	
//		CheckMenuItem(hMenu, ID_WINDOWS_FRAME_INFO, MF_BYCOMMAND | MF_UNCHECKED);
//		ShowWindow(g_hTextFrameInfoWindow, SW_HIDE);
//	}
//
//	SetWindowPos(g_hTextFrameWindow, NULL, g_TextFrameWindowRect.left, g_TextFrameWindowRect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
//
//	//Display all created windows
//	ShowWindow(g_hTextFrameWindow, SW_SHOW);
//	UpdateWindow(g_hTextFrameWindow);
//
//	return S_OK;
//}
//
//HRESULT LoadOptions(_IN LPTSTR sBuffer, _IN int iLength)
//{
//	if (!sBuffer) { return E_INVALIDARG; }
//
//	INT	uiCurChar = 0;
//	
//	HRESULT	l_iWhitespaceLength = E_FAIL;
//	HRESULT	l_iOptionLength		= E_FAIL;
//	HRESULT	l_iOptionNameLength = E_FAIL;
//
//	g_dwDisplayOptions = 0;
//
//	while (SUCCEEDED((l_iWhitespaceLength = FindCharsInv(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT(" \t\xfeff\n\r")))))
//	{
//		uiCurChar += l_iWhitespaceLength;
//	
//		if (SUCCEEDED((l_iOptionLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT(" \t\xfeff\n\r")))))
//
//		//[Text-frame options]
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("WindowX"), __min((int)wcslen(TEXT("WindowX")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_TextFrameWindowRect.left);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("WindowY"), __min((int)wcslen(TEXT("WindowY")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_TextFrameWindowRect.top);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("Width"), __min((int)wcslen(TEXT("Width")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_TextFrameRectF.right);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("Height"), __min((int)wcslen(TEXT("Height")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_TextFrameRectF.bottom);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("Insets"), __min((int)wcslen(TEXT("Insets")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_fInsets);
//			}
//		}
//
//		//[Display options]
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("FrameBorder"), __min((int)wcslen(TEXT("FrameBorder")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				BOOL bTemp;
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &bTemp);
//				if (bTemp) { g_dwDisplayOptions |= RO_DRAW_TEXTFRAME_OUTLINE; }
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("ColumnOutlines"), __min((int)wcslen(TEXT("ColumnOutlines")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				BOOL bTemp;
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &bTemp);
//				if (bTemp) { g_dwDisplayOptions |= RO_DRAW_COLUMN_OUTLINES; }
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("ParagraphOutlines"), __min((int)wcslen(TEXT("ParagraphOutlines")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				BOOL bTemp;
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &bTemp);
//				if (bTemp) { g_dwDisplayOptions |= RO_DRAW_PARAGRAPH_OUTLINES; }
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("LineOutlines"), __min((int)wcslen(TEXT("LineOutlines")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				BOOL bTemp;
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &bTemp);
//				if (bTemp) { g_dwDisplayOptions |= RO_DRAW_LINE_OUTLINES; }
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("WordOutlines"), __min((int)wcslen(TEXT("WordOutlines")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				BOOL bTemp;
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &bTemp);
//				if (bTemp) { g_dwDisplayOptions |= RO_DRAW_WORD_OUTLINES; }
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("Selection"), __min((int)wcslen(TEXT("Selection")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				BOOL bTemp;
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &bTemp);
//				if (bTemp) { g_dwDisplayOptions |= RO_DRAW_SELECTION; }
//			}
//		}
//
//		//[General options]
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("StartupPath"), __min((int)wcslen(TEXT("StartupPath")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				int iStart = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("\"")) + 1;
//				int iTextLength = FindChars(&sBuffer[uiCurChar + iStart], iLength - (uiCurChar + iStart), TEXT("\""));
//
//				if (g_szStartupFilePath) { delete[] g_szStartupFilePath; g_szStartupFilePath = NULL; }
//
//				g_szStartupFilePath = new TCHAR[iTextLength + 1];
//
//				wcsncpy_s(g_szStartupFilePath, iTextLength + 1, &sBuffer[uiCurChar + iStart], iTextLength);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("NewFileTemplatePath"), __min((int)wcslen(TEXT("NewFileTemplatePath")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				int iStart = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("\"")) + 1;
//				int iTextLength = FindChars(&sBuffer[uiCurChar + iStart], iLength - (uiCurChar + iStart), TEXT("\""));
//
//				if (g_szNewFileTemplatePath) { delete[] g_szNewFileTemplatePath; g_szNewFileTemplatePath = NULL; }
//
//				g_szNewFileTemplatePath = new TCHAR[iTextLength + 1];
//
//				wcsncpy_s(g_szNewFileTemplatePath, iTextLength + 1, &sBuffer[uiCurChar + iStart], iTextLength);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("EmbedOptions"), __min((int)wcslen(TEXT("EmbedOptions")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_bEmbedOptions);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("DumpDebugInfo"), __min((int)wcslen(TEXT("DumpDebugInfo")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_bDumpDebugInfo);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("CodeViewVisible"), __min((int)wcslen(TEXT("CodeViewVisible")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_bCodeViewVisible);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("InfoViewVisible"), __min((int)wcslen(TEXT("InfoViewVisible")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_bInfoViewVisible);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("CodeViewX"), __min((int)wcslen(TEXT("CodeViewX")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_CodeViewWindowRect.left);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("CodeViewY"), __min((int)wcslen(TEXT("CodeViewY")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_CodeViewWindowRect.top);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("InfoViewX"), __min((int)wcslen(TEXT("InfoViewX")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_TextFrameInfoWindowRect .left);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("InfoViewY"), __min((int)wcslen(TEXT("InfoViewY")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_TextFrameInfoWindowRect.top);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("CodeViewWidth"), __min((int)wcslen(TEXT("CodeViewWidth")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_CodeViewWindowRect.right);
//			}
//		}
//
//		if (!wcsncmp(&sBuffer[uiCurChar], TEXT("CodeViewHeight"), __min((int)wcslen(TEXT("CodeViewHeight")), l_iOptionLength)))
//		{
//			if (SUCCEEDED((l_iOptionNameLength = FindChars(&sBuffer[uiCurChar], iLength - uiCurChar, TEXT("=")))))
//			{
//				swscanf_s(&sBuffer[uiCurChar + l_iOptionNameLength + 1], TEXT("%d"), &g_CodeViewWindowRect.bottom);
//			}
//		}			
//
//		if (SUCCEEDED(l_iOptionLength)) { uiCurChar += l_iOptionLength; }
//		else { break; }
//	}
// 
//	return S_OK;
//}
//
//HRESULT SaveOptions(_IN HANDLE hFile, _IN bool bEmbed)
//{
//	if (hFile == INVALID_HANDLE_VALUE) { return E_INVALIDARG; }
//
//	TCHAR	szBuffer[2048];
//	INT	uiBufferSize = 2048;
//
//	DWORD	dwBytesWritten = 0;
//
//	if (bEmbed)	{ wcscpy_s(szBuffer, uiBufferSize, TEXT("<!--TestAppSettings ")); }
//	else			{ wcscpy_s(szBuffer, uiBufferSize, TEXT("[TestApp v1.000r]\r\n\r\n")); } 
//
//	//Save text-frame options
//	if (!bEmbed) 
//	{
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s"), szBuffer, TEXT("[Text-frame options]\r\n\r\n"));										
//
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("WindowX="),	g_TextFrameWindowRect.left,	TEXT("px\r\n"));	
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("WindowY="),	g_TextFrameWindowRect.top,	TEXT("px\r\n\r\n"));	
//	}
//
//	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("Width="),		g_TextFrameRectF.right,		TEXT("em"));	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); } 
//	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("Height="),	g_TextFrameRectF.bottom,	TEXT("em"));	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); } 
//	
//	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("Insets="),	g_fInsets,					TEXT("em"));	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); } 
//
//	//Save general options
//	if (!bEmbed) 
//	{ 
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s"), szBuffer, TEXT("[General options]\r\n\r\n"));							
//		
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("EmbedOptions="),	g_bEmbedOptions);	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); 
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("DumpDebugInfo="),	g_bDumpDebugInfo);	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); 
//
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s"), szBuffer, TEXT("StartupPath=\""));			wcscat_s(szBuffer, uiBufferSize, g_szStartupFilePath);		wcscat_s(szBuffer, uiBufferSize, TEXT("\"\r\n"));
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s"), szBuffer, TEXT("NewFileTemplatePath=\""));	wcscat_s(szBuffer, uiBufferSize, g_szNewFileTemplatePath);	wcscat_s(szBuffer, uiBufferSize, TEXT("\"\r\n\r\n"));
//	}
//
//	//Save display options
//	if (!bEmbed) { swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s"), szBuffer, TEXT("[Display options]\r\n\r\n")); }
//
//	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("FrameBorder="),			__min(1, g_dwDisplayOptions & RO_DRAW_TEXTFRAME_OUTLINE));	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); } 
//	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("ColumnOutlines="),		__min(1, g_dwDisplayOptions & RO_DRAW_COLUMN_OUTLINES));		if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); } 
//	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("ParagraphOutlines="),	__min(1, g_dwDisplayOptions & RO_DRAW_PARAGRAPH_OUTLINES));	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); } 
//	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("LineOutlines="),		__min(1, g_dwDisplayOptions & RO_DRAW_LINE_OUTLINES));		if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); } 
//	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("WordOutlines="),		__min(1, g_dwDisplayOptions & RO_DRAW_WORD_OUTLINES));		if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT(" ")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); } 
//	swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("Selection="),			__min(1, g_dwDisplayOptions & RO_DRAW_SELECTION));			if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT("")); } else { wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); } 
//
//	//Save window options
//	if (!bEmbed) 
//	{ 
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s"), szBuffer, TEXT("[Window options]\r\n\r\n"));
//		
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("CodeViewVisible="),		IsWindowVisible(g_hCodeViewWindow));			wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n")); 
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d"), szBuffer, TEXT("InfoViewVisible="),		IsWindowVisible(g_hTextFrameInfoWindow));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n")); 
//
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("InfoViewX="),			g_TextFrameInfoWindowRect.left,	TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n"));
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("InfoViewY="),			g_TextFrameInfoWindowRect.top,	TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n\r\n"));
//
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("CodeViewX="),			g_CodeViewWindowRect.left,		TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n"));
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("CodeViewY="),			g_CodeViewWindowRect.top,		TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n"));
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("CodeViewWidth="),		g_CodeViewWindowRect.right,		TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n"));
//		swprintf_s(szBuffer, uiBufferSize, TEXT("%s%s%d%s"), szBuffer, TEXT("CodeViewHeight="),	g_CodeViewWindowRect.bottom,	TEXT("px"));	wcscat_s(szBuffer, uiBufferSize, TEXT("\r\n"));
//	}
//
//	if (bEmbed) { wcscat_s(szBuffer, uiBufferSize, TEXT("-->")); }
//
//	WriteFile(hFile, szBuffer, sizeof(TCHAR) * (DWORD)(int)wcslen(szBuffer), &dwBytesWritten, NULL);
//
//	return S_OK;
//}
//
//HRESULT SaveXMLCode(LPTSTR szPath)
//{
//	if (!szPath) { return E_INVALIDARG; }
//
//	DWORD dwBytesWritten = 0;
//
//	//Open destination file
//	HANDLE hFile = CreateFile(szPath, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if (hFile != INVALID_HANDLE_VALUE)
//	{
//		WriteFile(hFile, TEXT("\xfeff"), sizeof(TCHAR), &dwBytesWritten, NULL);
//
//		TCHAR* szFileText = new TCHAR[GetWindowTextLength(g_hCodeViewTextBox) + 1];
//		GetWindowText(g_hCodeViewTextBox, szFileText, GetWindowTextLength(g_hCodeViewTextBox) + 1);
//
//		SetFilePointer(hFile, sizeof(TCHAR), 0, FILE_BEGIN);
//
//		if (g_bEmbedOptions)
//		{
//			SaveOptions(hFile, TRUE);
//		}
//
//		if (szFileText)
//		{
//			//Write data
//			WriteFile(hFile, szFileText, sizeof(TCHAR) * (DWORD)(int)wcslen(szFileText), &dwBytesWritten, NULL);
//	
////			LPTSTR szFoundString = wcsstr(szFileText, TEXT(<TextFrame>
//	
//		}
//
//		//Close file
//		CloseHandle(hFile);
//
//		if (g_szFilePath != szPath)
//		{
//			if (g_szFilePath) { delete[] g_szFilePath; }
//
//			g_szFilePath = new TCHAR[lstrlen(szPath) + 1];
//			wcscpy_s(g_szFilePath, lstrlen(szPath) + 1, szPath);
//		}
//
//		TCHAR szTmp[1024]; 
//		wcscpy_s(szTmp, 1024, g_szFilePath);
//		PathStripPath(szTmp);
//
//		LPTSTR szTitle = new TCHAR[lstrlen(szTmp) + lstrlen(TEXT("Code View (")) + 1 + 1];
//
//		wcscpy_s(szTitle, lstrlen(szTmp) + lstrlen(TEXT("Code View (")) + 1 + 1, TEXT("Code View ["));
//		wcscat_s(szTitle, lstrlen(szTmp) + lstrlen(TEXT("Code View (")) + 1 + 1, szTmp);
//		wcscat_s(szTitle, lstrlen(szTmp) + lstrlen(TEXT("Code View (")) + 1 + 1, TEXT("]"));
//
//		SetWindowText(g_hCodeViewWindow, szTitle);
//
//		delete[] szTitle;
//		delete[] szFileText;
//
//		return S_OK;
//	}
//
//	return E_FAIL;
//}
//
//void RedrawTextFrame(_IN HWND hWnd, _IN DWORD dwUpdateOptions)
//{
//	HDC hdc = GetDC(hWnd);
//
//	RECT rt; GetClientRect(hWnd, &rt);
//
//	//Create a second device context & a bitmap to paint to
//	HBITMAP	hBitmap		= CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
//	HDC		hBitmapDC	= CreateCompatibleDC(hdc);
//
//	HBITMAP	hOldBitmap	= (HBITMAP)SelectObject(hBitmapDC, hBitmap);
//
//	//Draw background for the text-frame
//	SelectObject(hBitmapDC, GetStockObject(WHITE_PEN));
//	Rectangle(hBitmapDC, 0, 0, rt.right, rt.bottom);
//
//	//Call layout function
//	TR_LayOut(hBitmapDC, dwUpdateOptions);
//
//	//Call render function
//	TR_Render(hBitmapDC, NULL);
//
//	//Copy rendered image
//	BitBlt(hdc, 0, 0, rt.right, rt.bottom, hBitmapDC, 0, 0, SRCCOPY);  
//
//	//Clean up
//	SelectObject(hBitmapDC, hOldBitmap);
//	DeleteObject(hBitmap);
//
//	DeleteDC(hBitmapDC);
//
//	ReleaseDC(hWnd, hdc);
//}
//
//LRESULT CALLBACK TextFrameProc(HWND hWnd, INT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//	PAINTSTRUCT ps;
//
//	switch(message) 
//	{
//	case WM_LBUTTONDOWN:
//		{
//			g_StartPoint.x = GET_X_LPARAM(lParam);
//			g_StartPoint.y = GET_Y_LPARAM(lParam);
//
//			TR_SetUnit(UNIT_POINT);
//
//			if (SUCCEEDED(TR_CursorPosToCharacterPos(((INT)GET_X_LPARAM(lParam) * g_fScreen2Point) / 1000, ((INT)GET_Y_LPARAM(lParam) * g_fScreen2Point) / 1000, &g_iSelectionStart)))
//			{
//				g_bDragging = TRUE;
//				SetCapture(hWnd);
//			}
//
//			break;
//		}
//
//	case WM_MOUSEMOVE:
//		{
//			g_MousePoint.x = GET_X_LPARAM(lParam);
//			g_MousePoint.y = GET_Y_LPARAM(lParam);
//
//			TR_SetUnit(UNIT_POINT);
//
//			if (g_bDragging)
//			{
//				TR_CursorPosToCharacterPos(((INT)GET_X_LPARAM(lParam) * g_fScreen2Point) / 1000, ((INT)GET_Y_LPARAM(lParam) * g_fScreen2Point) / 1000, &g_iSelectionEnd);
//
//				TR_ClearSelection();
//
//				if (g_iSelectionStart > g_iSelectionEnd)	{ TR_AddSelectionRange(g_iSelectionEnd, g_iSelectionStart - g_iSelectionEnd + 1); }
//				else										{ TR_AddSelectionRange(g_iSelectionStart, g_iSelectionEnd - g_iSelectionStart + 1); }
//
//				RedrawTextFrame(hWnd, UO_UPDATE_SELECTION);
//			}
//
//			break;
//		}
//
//	case WM_LBUTTONUP:
//		{
//			TR_SetUnit(UNIT_POINT);
//
//			if ((g_StartPoint.x == GET_X_LPARAM(lParam)) && (g_StartPoint.y == GET_Y_LPARAM(lParam)))
//			{
//				if (TR_IsCursorInText(((INT)GET_X_LPARAM(lParam) * g_fScreen2Point) / 1000, ((INT)GET_Y_LPARAM(lParam) * g_fScreen2Point) / 1000))
//				{
//					if ((abs((int)g_iSelectionEnd - (int)g_iSelectionStart)) == 1)
//					{
//						if (TR_IsCursorInSelection(((INT)GET_X_LPARAM(lParam) * g_fScreen2Point) / 1000, ((INT)GET_Y_LPARAM(lParam) * g_fScreen2Point) / 1000))
//						{
//							TR_ClearSelection();
//							g_iSelectionEnd = g_iSelectionStart;
//			
//							RedrawTextFrame(hWnd, UO_UPDATE_SELECTION);
//						}
//						else
//						{
//							TR_ClearSelection();
//							TR_AddSelectionRange(g_iSelectionStart, 1);
//							g_iSelectionEnd = g_iSelectionStart + 1;
//			
//							RedrawTextFrame(hWnd, UO_UPDATE_SELECTION);
//						}
//					}
//					else
//					{
//						TR_ClearSelection();
//						TR_AddSelectionRange(g_iSelectionStart, 1);
//						g_iSelectionEnd = g_iSelectionStart + 1;
//	
//						RedrawTextFrame(hWnd, UO_UPDATE_SELECTION);
//					}
//				}
//				else
//				{
//					TR_ClearSelection();
//					g_iSelectionEnd = g_iSelectionStart;
//
//					RedrawTextFrame(hWnd, UO_UPDATE_SELECTION);
//				}
//			}
//	
//			ReleaseCapture();
//			g_bDragging = FALSE;
//
//			break;
//		}
//
//	case WM_CREATE:
//		{
//			//Call startup function before any other call to the DLL
//			TR_TextRenderStartup();
//			break;
//		}
//
//	case WM_SIZING:
//		{
//			if (g_bLockTextFrameDimensions)
//			{
//				*((RECT*)lParam) = g_TextFrameWindowRect;
//	
//				((RECT*)lParam)->right		+= g_TextFrameWindowRect.left;
//				((RECT*)lParam)->bottom		+= g_TextFrameWindowRect.top;
//				return TRUE;
//			}
//
//		//	if ((((RECT*)lParam)->right - ((RECT*)lParam)->left) < 170)
//		//	{
//		//		((RECT*)lParam)->right = ((RECT*)lParam)->left + 170;
//		//	}
//
//			if ((((RECT*)lParam)->bottom - ((RECT*)lParam)->top) < 50)
//			{
//				((RECT*)lParam)->bottom = ((RECT*)lParam)->top + 50;
//			}
//
////			break;
//		}
//
//	case WM_SIZE:
//		{
//			bool bDoResize = FALSE;
//
//			GetWindowRect(hWnd, &g_TextFrameWindowRect);
//	
//			g_TextFrameWindowRect.right		-= g_TextFrameWindowRect.left;
//			g_TextFrameWindowRect.bottom	-= g_TextFrameWindowRect.top;
//
//	//		if (g_TextFrameWindowRect.right < 170) { g_TextFrameWindowRect.right = 170; bDoResize = TRUE; }
//			if (g_TextFrameWindowRect.bottom < 50) { g_TextFrameWindowRect.bottom = 50; bDoResize = TRUE; }
//
//		/*	if (bDoResize)
//			{
//				//Resize window & beep to indicate unsuccessful resizing
//				SetWindowPos(hWnd, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER); 
//				MessageBeep(MB_ICONERROR);
//			}*/
//		}
//	case WM_MOVE:
//		{
//			GetWindowRect(hWnd, &g_TextFrameWindowRect);
//
//			g_TextFrameWindowRect.right		-= g_TextFrameWindowRect.left;
//			g_TextFrameWindowRect.bottom	-= g_TextFrameWindowRect.top;
//
//			HDC hdc = GetDC(hWnd);
//
//			//Obtain the ratio used to covert between screen & world units (in this case points)
//			INT	uiDPI = GetDeviceCaps(hdc, LOGPIXELSY);
//			g_fScreen2Point = (72 * 1000) / uiDPI;
//			g_fPoint2Screen = (uiDPI * 1000) / 72;
//
//			ReleaseDC(hWnd, hdc);
//
//			if (g_bSkipResizing) { g_bSkipResizing = FALSE; return TRUE; }
//
//			//Update text-frame rectangle
//			RECT rt; GetClientRect(hWnd, &rt);
//
//			g_TextFrameRectF.left		= (rt.left		* g_fScreen2Point) + g_fInsets;
//			g_TextFrameRectF.top		= (rt.top		* g_fScreen2Point) + g_fInsets;
//			g_TextFrameRectF.right		= (rt.right	* g_fScreen2Point) - g_fInsets * 2;
//			g_TextFrameRectF.bottom		= (rt.bottom	* g_fScreen2Point) - g_fInsets * 2;
//
//			SendMessage(g_hTextFrameInfoWindow, UPDATE_DIMENSIONS, 0, 0);
//			break;
//		}
//
//	case WM_SYSCOMMAND:
//		{
//			if (wParam == SC_CLOSE) 
//			{
//				SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDM_EXIT, 0), 0);
//				return 0;
//			}
//
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//
//	case WM_COMMAND:
//		{
//			wmId	= LOWORD(wParam); 
//			wmEvent	= HIWORD(wParam); 
//
//		//Parse the menu selections:
//			switch(wmId)
//			{
//			case ID_WINDOWS_CODEVIEW:
//				{
//					HMENU hMenu = GetMenu(hWnd);
//					hMenu = GetSubMenu(hMenu, 1);
//
//					MENUITEMINFO ItemInfo;
//					ItemInfo.cbSize = sizeof(MENUITEMINFO);
//					ItemInfo.fMask = MIIM_STATE;
//
//					GetMenuItemInfo(hMenu, ID_WINDOWS_CODEVIEW, FALSE, &ItemInfo);
//
//					if (ItemInfo.fState & MFS_CHECKED)
//					{
//						CheckMenuItem(hMenu, ID_WINDOWS_CODEVIEW, MF_BYCOMMAND | MF_UNCHECKED);
//						ShowWindow(g_hCodeViewWindow, SW_HIDE);
//					}
//					else
//					{
//						CheckMenuItem(hMenu, ID_WINDOWS_CODEVIEW, MF_BYCOMMAND | MF_CHECKED);
//						ShowWindow(g_hCodeViewWindow, SW_SHOW);
//					}
//
//					break;
//				}
//			case ID_WINDOWS_FRAME_INFO:
//				{
//					HMENU hMenu = GetMenu(hWnd);
//					hMenu = GetSubMenu(hMenu, 1);
//
//					MENUITEMINFO ItemInfo;
//					ItemInfo.cbSize = sizeof(MENUITEMINFO);
//					ItemInfo.fMask = MIIM_STATE;
//
//					GetMenuItemInfo(hMenu, ID_WINDOWS_FRAME_INFO, FALSE, &ItemInfo);
//
//					if (ItemInfo.fState & MFS_CHECKED)
//					{
//						CheckMenuItem(hMenu, ID_WINDOWS_FRAME_INFO, MF_BYCOMMAND | MF_UNCHECKED);
//						ShowWindow(g_hTextFrameInfoWindow, SW_HIDE);
//					}
//					else
//					{
//						CheckMenuItem(hMenu, ID_WINDOWS_FRAME_INFO, MF_BYCOMMAND | MF_CHECKED);
//						ShowWindow(g_hTextFrameInfoWindow, SW_SHOW);
//					}
//
//					break;
//				}
//
//			case IDM_ABOUT:          { DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About); break; }
//			case ID_OPTIONS_DISPLAY: { DialogBox(g_hInst, (LPCTSTR)IDD_OPTIONS_DISPLAY, hWnd, (DLGPROC)DisplayOptions); break; }
//			case ID_OPTIONS_GENERAL: { DialogBox(g_hInst, (LPCTSTR)IDD_OPTIONS_GENERAL, hWnd, (DLGPROC)GeneralOptions); break; }
//
//			case IDM_EXIT:	
//				{ 
//					TCHAR szBuffer[1024];
//
//					//Get full path to the executable
//					GetModuleFileName(NULL, szBuffer, 1024);
//
//					//Truncate file name
//					PathRemoveFileSpec(szBuffer);
//
//					//Append new name
//					wcscat_s(szBuffer, TEXT("\\TestApp.cfg"));
//
//					//Open destination file
//					HANDLE hFile = CreateFile(szBuffer, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//			
//					if (hFile != INVALID_HANDLE_VALUE)
//					{
//						DWORD dwBytesWritten = 0;
//
//						WriteFile(hFile, TEXT("\xfeff") , sizeof(TCHAR), &dwBytesWritten, NULL);
//
//						SaveOptions(hFile, FALSE);
//
//						//Close file
//						CloseHandle(hFile);
//					}
//
//					DestroyWindow(hWnd);
//					break; 
//				}
//
//			case ID_FILE_PRINT:
//				{
//					DOCINFO docInfo;
//					ZeroMemory(&docInfo, sizeof(DOCINFO));
//
//					docInfo.cbSize = sizeof(DOCINFO);
//
//					TCHAR szTmp[1024]; 
//					wcscpy_s(szTmp, 1024, g_szFilePath);
//					PathStripPath(szTmp);
//			
//					PathRemoveExtension(szTmp);
//
//					docInfo.lpszDocName = szTmp;
//
//					//Create a PRINTDLG structure, and initialize the appropriate fields.
//					PRINTDLG printDlg;
//					ZeroMemory(&printDlg, sizeof(PRINTDLG));
//					
//					printDlg.lStructSize = sizeof(PRINTDLG);
//					printDlg.Flags = PD_RETURNDC;
//
//					//Display a print dialog box.
//					if (PrintDlg(&printDlg))
//					{
//						StartDoc(printDlg.hDC, &docInfo);
//						StartPage(printDlg.hDC);
//
//						//Call layout function
//						TR_LayOut(printDlg.hDC, (DWORD)UO_UPDATE_ALL);
//
//						//Call render function
//						TR_Render(printDlg.hDC, NULL);
//
//						EndPage(printDlg.hDC);
//						EndDoc(printDlg.hDC); 
//					}
//
//					if (printDlg.hDevMode)  { GlobalFree(printDlg.hDevMode); }
//					if (printDlg.hDevNames)	{ GlobalFree(printDlg.hDevNames); }
//					if (printDlg.hDC)       { DeleteDC(printDlg.hDC); }
//
//					break;
//				}
//
//			case ID_FILE_NEW:
//				{
//					TCHAR szBuffer[1024];
//
//					if (PathIsRelative(g_szNewFileTemplatePath))
//					{
//						GetModuleFileName(NULL, szBuffer, 2048);
//						PathRemoveFileSpec(szBuffer);
//
//						PathAppend(szBuffer, g_szNewFileTemplatePath);
//					}
//					else
//					{
//						wcscpy_s(szBuffer, 1024, g_szNewFileTemplatePath);
//					}
//
//					if (SUCCEEDED(LoadXMLCode(szBuffer)))
//					{
//						if (g_szFilePath) { delete[] g_szFilePath;	g_szFilePath = NULL; }
//
//						SetWindowText(g_hCodeViewWindow, TEXT("Code View [Unnamed]"));
//					}
//
//					break;
//				}
//
//			case ID_FILE_LOAD:
//				{
//					TCHAR szPath[MAX_PATH + 1];
//					szPath[0] = 0;
//
//					OPENFILENAME OpenFile;
//					OpenFile.lStructSize		= sizeof(OPENFILENAME);
//					OpenFile.hwndOwner			= hWnd;
//					OpenFile.hInstance			= g_hInst;
//					OpenFile.lpstrFilter		= TEXT("XML file (*.xml)\0*.xml\0All files (*.*)\0*.*\0");
//					OpenFile.lpstrCustomFilter	= NULL;
//					OpenFile.nMaxCustFilter		= 0;
//					OpenFile.nFilterIndex		= 0;
//					OpenFile.lpstrFile			= szPath;
//					OpenFile.nMaxFile			= MAX_PATH;
//					OpenFile.lpstrFileTitle		= NULL;
//					OpenFile.nMaxFileTitle		= 0;
//					OpenFile.lpstrInitialDir	= NULL;
//					OpenFile.lpstrTitle			= TEXT("Open a file");
//					OpenFile.nFileOffset		= 0;
//					OpenFile.nFileExtension		= 0;
//					OpenFile.lpstrDefExt		= NULL;
//					OpenFile.lCustData			= NULL;
//					OpenFile.lpfnHook			= NULL;
//					OpenFile.lpTemplateName		= NULL;
//
//					OpenFile.Flags = OFN_FILEMUSTEXIST;
//
//					if (GetOpenFileName(&OpenFile)) { LoadXMLCode(szPath); }
//					break;
//				}
//
//			case ID_FILE_SAVE: { if (SUCCEEDED(SaveXMLCode(g_szFilePath))) { break; } }
//
//			case ID_FILE_SAVEAS:
//				{
//					TCHAR szPath[MAX_PATH + 1];
//					szPath[0] = 0;
//
//					if (g_szFilePath) { wcscpy_s(szPath, MAX_PATH + 1, g_szFilePath); }
//
//					PathStripPath(szPath);
//					PathRemoveExtension(szPath);
//
//					OPENFILENAME OpenFile;
//					OpenFile.lStructSize		= sizeof(OPENFILENAME);
//					OpenFile.hwndOwner			= hWnd;
//					OpenFile.hInstance			= g_hInst;
//					OpenFile.lpstrFilter		= TEXT("XML file (*.xml)\0*.xml\0All files (*.*)\0*.*\0");
//					OpenFile.lpstrCustomFilter	= NULL;
//					OpenFile.nMaxCustFilter		= 0;
//					OpenFile.nFilterIndex		= 0;
//					OpenFile.lpstrFile			= szPath;
//					OpenFile.nMaxFile			= MAX_PATH;
//					OpenFile.lpstrFileTitle		= NULL;
//					OpenFile.nMaxFileTitle		= 0;
//					OpenFile.lpstrInitialDir	= NULL;
//					OpenFile.lpstrTitle			= TEXT("Save code as file");
//					OpenFile.nFileOffset		= 0;
//					OpenFile.nFileExtension		= 0;
//					OpenFile.lpstrDefExt		= NULL;
//					OpenFile.lCustData			= NULL;
//					OpenFile.lpfnHook			= NULL;
//					OpenFile.lpTemplateName		= NULL;
//
//					OpenFile.Flags = OFN_HIDEREADONLY | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT;
//
//					if (GetSaveFileName(&OpenFile))
//					{
//						if (!lstrlen(szPath)) { return FALSE; }
//
//						if (!OpenFile.nFileExtension) { wcscat_s(szPath, _MAX_PATH, TEXT(".xml")); }
//						else { if (!szPath[OpenFile.nFileExtension]) { wcscat_s(szPath, _MAX_PATH, TEXT("xml")); } }
//
//						if (FAILED(SaveXMLCode(szPath)))
//						{
//							MessageBox(hWnd, szPath, TEXT("Can't save file!"), MB_ICONERROR);
//						}
//					}
//				
//					break;
//				}
//
//			default: { return DefWindowProc(hWnd, message, wParam, lParam); }
//			}
//
//			break;
//		}
//
//	case WM_PAINT:
//		{
//			HDC hdc;
//
//			FILE* pFile = NULL; 
//			
//			if (g_bDumpDebugInfo)
//			{
//				TCHAR szExecutablePath[MAX_PATH + 1];
//				GetModuleFileName(NULL, szExecutablePath, MAX_PATH);
//				PathRemoveFileSpec(szExecutablePath);
//
//				PathAppend(szExecutablePath, TEXT("\\DebugDump.txt"));
//
//				_wfopen_s(&pFile, szExecutablePath, TEXT("w"));
//			}
//
//			hdc = BeginPaint(hWnd, &ps);
//
//			//Obtain the ratio used to covert between screen & world units (in this case points)
//			INT	uiDPI = GetDeviceCaps(hdc, LOGPIXELSY);
//			g_fScreen2Point = (72 * 1000) / uiDPI;
//			g_fPoint2Screen = (uiDPI * 1000) / 72;
//
//			//Set the new rectangle in points
//			TR_SetUnit(UNIT_EP);
//			TR_SetRect(g_TextFrameRectF.left, g_TextFrameRectF.top, g_TextFrameRectF.right, g_TextFrameRectF.bottom);
//
//			//Set rendering options
//			TR_SetRenderOptions(g_dwDisplayOptions | RO_QUALITY_NORMAL);
//
//			if (pFile) 
//			{
//			//Dump text-frame rectangle
//				fwprintf(pFile, TEXT("%s\n%f %f %f %f\n"), TEXT("[text-frame rect]"), g_TextFrameRectF.left, g_TextFrameRectF.top, g_TextFrameRectF.right, g_TextFrameRectF.bottom);
//				fflush(pFile);
//			}
//
//			if (pFile) 
//			{
//				LPTSTR szNewText = new TCHAR[GetWindowTextLength(g_hCodeViewTextBox) + 1];
//				GetWindowText(g_hCodeViewTextBox, szNewText, GetWindowTextLength(g_hCodeViewTextBox) + 1);
//
//				//Dump text-frame text content
//				fwprintf(pFile, TEXT("\n%s\n%s"), TEXT("[text-frame content]"), szNewText);
//				fflush(pFile);
//
//				if (szNewText) { delete szNewText; }
//			}
//
//			//Close dump report
//			if (pFile) { fclose(pFile); }
//
//		/*	if (g_bDisableStyles)
//			{
//				//Delete all style entries, but NOT the styles themselves 
//				TR_SetStyleType(STYLE_TEXTFRAME);
//				TR_ClearAllStyles(0, -1);
//
//				TR_SetStyleType(STYLE_PARAGRAPH);
//				TR_ClearAllStyles(0, -1);
//				TR_ClearDefaultStyle();
//
//				TR_SetStyleType(STYLE_CHARACTER);
//				TR_ClearAllStyles(0, -1);
//				TR_ClearDefaultStyle();
//			}*/
//
//			RECT rt; GetClientRect(hWnd, &rt);
//
//			//Create a second device context & a bitmap to paint to
//			HBITMAP	hBitmap		= CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
//			HDC		hBitmapDC	= CreateCompatibleDC(hdc);
//
//			HBITMAP	hOldBitmap	= (HBITMAP)SelectObject(hBitmapDC, hBitmap);
//
//			//Draw background for the text-frame
//			SelectObject(hBitmapDC, GetStockObject(WHITE_PEN));
//			Rectangle(hBitmapDC, 0, 0, rt.right, rt.bottom);
//
//			TR_SetUnit(UNIT_PERCENT);
//			TR_SetOutputScale(100);
//
//			//Call layout function
//			TR_LayOut(hBitmapDC, (DWORD)UO_UPDATE_ALL);
//
//			RECT	l_RectF = { 0, 0, 0, 0 };
//			RECT	l_Rect = { 0, 0, 0, 0 };
//
//			GetClientRect(hWnd, &l_Rect);
//		
//			l_RectF.left	= (l_Rect.right * g_fScreen2Point) / 2000;
//			l_RectF.top		= (l_Rect.bottom * g_fScreen2Point) / 2000;
//
//			l_RectF.right	= 50;
//			l_RectF.bottom	= 50;
//
//
//			//Call render function
//			TR_Render(hBitmapDC, NULL);//&l_RectF);
//
//			//Copy rendered image
//			BitBlt(hdc, 0, 0, rt.right, rt.bottom, hBitmapDC, 0, 0, SRCCOPY);  
//
//			//Clean up
//			SelectObject(hBitmapDC, hOldBitmap);
//			DeleteObject(hBitmap);
//
//			DeleteDC(hBitmapDC);
//
//			EndPaint(hWnd, &ps);
//
//			//Update statistics
//			SendMessage(g_hTextFrameInfoWindow, UPDATE_CONTENT_STATS, 0, 0);
//			break;
//		}
//
//	case WM_DESTROY:
//		{
//			TR_TextRenderShutdown();
//			PostQuitMessage(0);
//			break;
//		}
//
//	default: { return DefWindowProc(hWnd, message, wParam, lParam); }
//	}
//	return 0;
//}
//
//LRESULT CALLBACK DisplayOptions(HWND hDlg, INT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//
//	switch(message)
//	{
//	case WM_INITDIALOG: 
//		{
//			if (g_dwDisplayOptions & RO_DRAW_TEXTFRAME_OUTLINE)	{ CheckDlgButton(hDlg, IDC_FRAME_BORDER,		BST_CHECKED); }
//			if (g_dwDisplayOptions & RO_DRAW_COLUMN_OUTLINES)		{ CheckDlgButton(hDlg, IDC_COLUMN_BORDERS,		BST_CHECKED); }
//			if (g_dwDisplayOptions & RO_DRAW_PARAGRAPH_OUTLINES)	{ CheckDlgButton(hDlg, IDC_PARAGRAPH_BORDERS,	BST_CHECKED); }
//			if (g_dwDisplayOptions & RO_DRAW_LINE_OUTLINES)		{ CheckDlgButton(hDlg, IDC_LINE_BORDERS,		BST_CHECKED); }
//			if (g_dwDisplayOptions & RO_DRAW_WORD_OUTLINES)		{ CheckDlgButton(hDlg, IDC_WORD_BORDERS,		BST_CHECKED); }
//			if (g_dwDisplayOptions & RO_DRAW_SELECTION)			{ CheckDlgButton(hDlg, IDC_SELECTION,			BST_CHECKED); }
//
//			return TRUE; 
//		}
//
//	case WM_COMMAND:
//		{
//			if (LOWORD(wParam) == IDOK)
//			{
//				g_dwDisplayOptions = 0;
//
//				if (IsDlgButtonChecked(hDlg, IDC_FRAME_BORDER) ==			BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_TEXTFRAME_OUTLINE; }
//				if (IsDlgButtonChecked(hDlg, IDC_COLUMN_BORDERS) ==		BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_COLUMN_OUTLINES; }
//				if (IsDlgButtonChecked(hDlg, IDC_PARAGRAPH_BORDERS) ==	BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_PARAGRAPH_OUTLINES; }
//				if (IsDlgButtonChecked(hDlg, IDC_LINE_BORDERS) ==			BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_LINE_OUTLINES; }
//				if (IsDlgButtonChecked(hDlg, IDC_WORD_BORDERS) ==			BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_WORD_OUTLINES; }
//				if (IsDlgButtonChecked(hDlg, IDC_SELECTION) ==			BST_CHECKED) { g_dwDisplayOptions |= RO_DRAW_SELECTION; }
//
//				EndDialog(hDlg, LOWORD(wParam));
//
//				RedrawWindow(g_hTextFrameWindow, NULL, NULL, RDW_INVALIDATE);
//				return TRUE;
//			}
//				
//			if (LOWORD(wParam) == IDCANCEL) 
//			{
//				EndDialog(hDlg, LOWORD(wParam));
//				return TRUE;
//			}
//
//			break;
//		}
//	}
//	return FALSE;
//}
//
//LRESULT CALLBACK GeneralOptions(HWND hDlg, INT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//
//	switch(message)
//	{
//	case WM_INITDIALOG: 
//		{
//			if (g_bEmbedOptions)	{ CheckDlgButton(hDlg, IDC_EMBED_OPTIONS, BST_CHECKED); }
//			if (g_bDumpDebugInfo)	{ CheckDlgButton(hDlg, IDC_DUMP_DEBUG_INFO, BST_CHECKED); }
//
//			SetDlgItemText(hDlg, IDC_STARTUP_FILE, g_szStartupFilePath); 
//			SetDlgItemText(hDlg, IDC_NEW_FILE, g_szNewFileTemplatePath); 
//
//			return TRUE; 
//		}
//
//	case WM_COMMAND:
//		{
//			switch(LOWORD(wParam))
//			{
//			case IDC_STARTUP_FILE_BROWSE:
//				{
//					TCHAR szPath[MAX_PATH + 1];
//					szPath[0] = 0;
//
//					OPENFILENAME OpenFile;
//					OpenFile.lStructSize		= sizeof(OPENFILENAME);
//					OpenFile.hwndOwner			= hDlg;
//					OpenFile.hInstance			= g_hInst;
//					OpenFile.lpstrFilter		= TEXT("XML file (*.xml)\0*.xml\0All files (*.*)\0*.*\0\0");
//					OpenFile.lpstrCustomFilter	= NULL;
//					OpenFile.nMaxCustFilter		= 0;
//					OpenFile.nFilterIndex		= 0;
//					OpenFile.lpstrFile			= szPath;
//					OpenFile.nMaxFile			= _MAX_PATH;
//					OpenFile.lpstrFileTitle		= NULL;
//					OpenFile.nMaxFileTitle		= 0;
//					OpenFile.lpstrInitialDir	= NULL;
//					OpenFile.lpstrTitle			= TEXT("Choose a startup file");
//					OpenFile.nFileOffset		= 0;
//					OpenFile.nFileExtension		= 0;
//					OpenFile.lpstrDefExt		= NULL;
//					OpenFile.lCustData			= NULL;
//					OpenFile.lpfnHook			= NULL;
//					OpenFile.lpTemplateName		= NULL;
//
//					OpenFile.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;
//
//					if (GetOpenFileName(&OpenFile)) 
//					{
//						TCHAR szRelativePath[MAX_PATH + 1];
//						TCHAR szExecutablePath[MAX_PATH + 1];
//
//						GetModuleFileName(NULL, szExecutablePath, MAX_PATH);
//
//						if (PathRelativePathTo(szRelativePath, szExecutablePath, NULL, szPath, NULL))
//						{
//							SetDlgItemText(hDlg, IDC_STARTUP_FILE, szRelativePath); 
//						}
//						else
//						{
//							SetDlgItemText(hDlg, IDC_STARTUP_FILE, szPath); 
//						}
//					}
//
//					break;
//				}
//
//			case IDC_NEW_FILE_BROWSE:
//				{
//					TCHAR szPath[MAX_PATH + 1];
//					szPath[0] = 0;
//
//					OPENFILENAME OpenFile;
//					OpenFile.lStructSize		= sizeof(OPENFILENAME);
//					OpenFile.hwndOwner			= hDlg;
//					OpenFile.hInstance			= g_hInst;
//					OpenFile.lpstrFilter		= TEXT("XML file (*.xml)\0*.xml\0All files (*.*)\0*.*\0\0");
//					OpenFile.lpstrCustomFilter	= NULL;
//					OpenFile.nMaxCustFilter		= 0;
//					OpenFile.nFilterIndex		= 0;
//					OpenFile.lpstrFile			= szPath;
//					OpenFile.nMaxFile			= _MAX_PATH;
//					OpenFile.lpstrFileTitle		= NULL;
//					OpenFile.nMaxFileTitle		= 0;
//					OpenFile.lpstrInitialDir	= NULL;
//					OpenFile.lpstrTitle			= TEXT("Choose a startup file");
//					OpenFile.nFileOffset		= 0;
//					OpenFile.nFileExtension		= 0;
//					OpenFile.lpstrDefExt		= NULL;
//					OpenFile.lCustData			= NULL;
//					OpenFile.lpfnHook			= NULL;
//					OpenFile.lpTemplateName		= NULL;
//
//					OpenFile.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOREADONLYRETURN;
//
//					if (GetOpenFileName(&OpenFile)) 
//					{
//						TCHAR szRelativePath[MAX_PATH + 1];
//						TCHAR szExecutablePath[MAX_PATH + 1];
//
//						GetModuleFileName(NULL, szExecutablePath, MAX_PATH);
//
//						if (PathRelativePathTo(szRelativePath, szExecutablePath, NULL, szPath, NULL))
//						{
//							SetDlgItemText(hDlg, IDC_NEW_FILE, szRelativePath); 
//						}
//						else
//						{
//							SetDlgItemText(hDlg, IDC_NEW_FILE, szPath); 
//						}
//					}
//
//					break;
//				}
//
//			case IDCANCEL:
//				{
//					EndDialog(hDlg, LOWORD(wParam));
//					return TRUE;
//				}
//
//			case IDOK:
//				{
//					if (IsDlgButtonChecked(hDlg, IDC_EMBED_OPTIONS)	== BST_CHECKED) { g_bEmbedOptions = TRUE; }	else { g_bEmbedOptions = FALSE; }
//					if (IsDlgButtonChecked(hDlg, IDC_DUMP_DEBUG_INFO)	== BST_CHECKED) { g_bDumpDebugInfo = TRUE; }	else { g_bDumpDebugInfo = FALSE; }
//
//					if (g_szStartupFilePath)		{ delete[] g_szStartupFilePath;		g_szStartupFilePath = NULL; }
//					if (g_szNewFileTemplatePath)	{ delete[] g_szNewFileTemplatePath;	g_szNewFileTemplatePath = NULL; }
//
//					TCHAR szPath[MAX_PATH + 1];
//					szPath[0] = 0;
//
//					GetDlgItemText(hDlg, IDC_STARTUP_FILE, szPath, MAX_PATH);
//
//					g_szStartupFilePath = new TCHAR[(int)wcslen(szPath) + 1];
//					wcscpy_s(g_szStartupFilePath, (int)wcslen(szPath) + 1, szPath);
//		
//					GetDlgItemText(hDlg, IDC_NEW_FILE, szPath, MAX_PATH);
//
//					g_szNewFileTemplatePath = new TCHAR[(int)wcslen(szPath) + 1];
//					wcscpy_s(g_szNewFileTemplatePath, (int)wcslen(szPath) + 1, szPath);
//
//					EndDialog(hDlg, LOWORD(wParam));
//
//					RedrawWindow(g_hTextFrameWindow, NULL, NULL, RDW_INVALIDATE);
//					return TRUE;
//				}
//			}
//
//			break;
//		}
//	}
//	return FALSE;
//}
//
////Message handler for about box.
//LRESULT CALLBACK About(HWND hDlg, INT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//
//	switch(message)
//	{
//	case WM_INITDIALOG: 
//		{
//		/*	DLLGETVERSIONPROC DllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(g_hTextRender, "DllGetVersion");
//			
//			if (DllGetVersion)
//			{
//				DLLVERSIONINFO VersionInfo;
//
//				//DllGetVersion()
//			}
//
//			TCHAR szBuffer[512];
//			GetModuleFileName(g_hTextRender, szBuffer, 512);
//			
//			void* vpVersionData = malloc(GetFileVersionInfoSize(szBuffer, NULL));
//			GetFileVersionInfo(szBuffer, NULL, GetFileVersionInfoSize(szBuffer, NULL), vpVersionData);
//
//			TCHAR szVersionString[16];
//	/		VerQueryValue(vpVersionData, TEXT("\\StringFileInfo\\080004b0\\ProductVersion"), (LPVOID*)&szVersionString, NULL);
//	
//		//	SetDlgItemText(hDlg, IDC_TEXT_RENDER_VERSION, szVersionString);
//*/
//			return TRUE; 
//		}
//
//	case WM_COMMAND:
//		{
//			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
//			{
//				EndDialog(hDlg, LOWORD(wParam));
//				return TRUE;
//			}
//			break;
//		}
//	}
//	return FALSE;
//}
//
//LRESULT CALLBACK TextFrameInfoProc(HWND hDlg, INT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//
//	int wmId, wmEvent;
//
//	switch(message)
//	{
//	case WM_SHOWWINDOW: { g_bInfoViewVisible = (BOOL)wParam; break; }
//
//	case WM_SIZE:
//	case WM_MOVE:
//		{
//			GetWindowRect(hDlg, &g_TextFrameInfoWindowRect);
//			break;
//		}
//
//	case WM_INITDIALOG: 
//		{
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Adjust height to fit contents."));
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_SETITEMDATA, 0, MO_GET_HEIGHT_USING_GIVEN_WIDTH);
//
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Adjust width to fit widest word."));
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_SETITEMDATA, 1, MO_GET_RECT_USING_WIDEST_WORD);
//
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Adjust size to fit all words."));
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_SETITEMDATA, 2, MO_GET_RECT_USING_WIDEST_WORD);
//
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Adjust size to fit lines without wrapping."));
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_SETITEMDATA, 3, MO_GET_RECT_USING_NO_WRAP);
//
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Adjust width to fit contents."));
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_SETITEMDATA, 4, MO_GET_WIDTH_USING_GIVEN_HEIGHT);
//	
//			SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_ADDSTRING, 0, (LPARAM)TEXT("Run test 01."));
//
//			SetDlgItemTextA(hDlg, IDC_MEASUREMENTS_INSETS, "500ep");
//
//			return TRUE; 
//		}
//
//	case UPDATE_CONTENT_STATS:
//		{
//			SetDlgItemInt(hDlg, IDC_CHARACTER_COUNT,	TR_GetContentStats(STATS_CHARACTERS_FIT),	FALSE);
//			SetDlgItemInt(hDlg, IDC_WORD_COUNT,		TR_GetContentStats(STATS_WORDS_FIT),		FALSE);
//			SetDlgItemInt(hDlg, IDC_LINE_COUNT,		TR_GetContentStats(STATS_LINES),			FALSE);
//			SetDlgItemInt(hDlg, IDC_PARAGRAPH_COUNT,	TR_GetContentStats(STATS_PARAGRAPHS_FIT),	FALSE);
//
//			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE);
//			break;
//		}
//
//	case UPDATE_DIMENSIONS:
//		{
//			char szBuffer[64];
//
//			_itoa_s(g_TextFrameRectF.right, szBuffer, 64, 10);
//			strcat_s(szBuffer, 64, "ep");
//
//			SetDlgItemTextA(hDlg, IDC_MEASUREMENTS_WIDTH, szBuffer);
//
//			_itoa_s(g_TextFrameRectF.bottom, szBuffer, 64, 10);
//			strcat_s(szBuffer, 64, "ep");
//
//			SetDlgItemTextA(hDlg, IDC_MEASUREMENTS_HEIGHT, szBuffer);
//
//			_itoa_s(g_fInsets, szBuffer, 64, 10);
//			strcat_s(szBuffer, 64, "ep");
//
//			SetDlgItemTextA(hDlg, IDC_MEASUREMENTS_INSETS, szBuffer);
//
//			RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE);
//			break;
//		}
//
//	case EN_ENTER:
//		{
//			switch(wParam)
//			{
//			case IDC_MEASUREMENTS_WIDTH:
//				{
//					char szBuffer[65];
//					GetDlgItemTextA(hDlg, IDC_MEASUREMENTS_WIDTH, szBuffer, 64);
//					g_TextFrameRectF.right = atoi(szBuffer);
//
//					UpdateTextFrameWindowRect();
//
//					SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
//					break;
//				}
//
//			case IDC_MEASUREMENTS_HEIGHT:
//				{
//					char szBuffer[65];
//					GetDlgItemTextA(hDlg, IDC_MEASUREMENTS_HEIGHT, szBuffer, 64);
//
//					g_TextFrameRectF.bottom = atoi(szBuffer);
//
//					UpdateTextFrameWindowRect();
//
//					SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
//					break;
//				}
//
//			case IDC_MEASUREMENTS_INSETS:
//				{
//					char szBuffer[65];
//					GetDlgItemTextA(hDlg, IDC_MEASUREMENTS_INSETS, szBuffer, 64);
//
//					g_fInsets = atoi(szBuffer);
//
//					UpdateTextFrameWindowRect();
//
//					SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
//					break;
//				}
//			}
//
//			break;
//		}
//
//	case WM_COMMAND:
//		{
//			wmId	= LOWORD(wParam);
//			wmEvent	= HIWORD(wParam);
//			
//			//Parse the menu selections:
//			switch(wmId)
//			{
//			case IDC_LOCK_DIMENSIONS:
//				{
//					g_bLockTextFrameDimensions = !g_bLockTextFrameDimensions;  
//					break;
//				}
//			case IDC_RESIZE:
//				{
//					switch(SendDlgItemMessage(hDlg, IDC_RESIZE_TYPE, CB_GETCURSEL, 0, 0))
//					{
//					case 5:
//						{
//							HDC hdc = GetDC(g_hTextFrameWindow);
//
//							TR_SetUnit(UNIT_EP);
//
//							INT l_iWidth			= g_TextFrameRectF.right;
//							INT l_iMeasuredHeight	= 0;
//							INT l_iMeasuredWidth	= 0;
//
//							TCHAR l_szCaption[32];
//
//							ResetTime01();
//
//							for (INT i = 0; i < 10; i++)
//							{
//								swprintf_s(l_szCaption, 32, TEXT("Pass %d"), i);
//								SetWindowText(hDlg, l_szCaption);
//
//								g_TextFrameRectF.right = l_iWidth + i;
//
//								TR_MeasureTextFrame(hdc, MO_GET_HEIGHT_USING_GIVEN_WIDTH, (tRect*)&g_TextFrameRectF);
//
//								l_iMeasuredHeight = g_TextFrameRectF.bottom;
//
//								TR_MeasureTextFrame(hdc, MO_GET_WIDTH_USING_GIVEN_HEIGHT, (tRect*)&g_TextFrameRectF);
//
//								if (l_iMeasuredHeight != g_TextFrameRectF.bottom)
//								{
//									MessageBoxW(NULL, TEXT("Error 01"), TEXT(""), MB_ICONERROR);
//								}
//
//								l_iMeasuredWidth = g_TextFrameRectF.right;
//
//								TR_MeasureTextFrame(hdc, MO_GET_HEIGHT_USING_GIVEN_WIDTH, (tRect*)&g_TextFrameRectF);
//
//								if ((l_iMeasuredHeight != g_TextFrameRectF.bottom) || (l_iMeasuredWidth != g_TextFrameRectF.right))
//								{
//									MessageBoxW(NULL, TEXT("Error 02"), TEXT(""), MB_ICONERROR);
//								}
//
//								TR_MeasureTextFrame(hdc, MO_GET_WIDTH_USING_GIVEN_HEIGHT, (tRect*)&g_TextFrameRectF);
//
//								if ((l_iMeasuredHeight != g_TextFrameRectF.bottom) || (l_iMeasuredWidth != g_TextFrameRectF.right))
//								{
//									MessageBoxW(NULL, TEXT("Error 03"), TEXT(""), MB_ICONERROR);
//								}
//							}
//
//							swprintf_s(l_szCaption, 32, TEXT("Time spent: %f"), GetTime01());
//							SetWindowText(hDlg, l_szCaption);
//
//							ReleaseDC(g_hTextFrameWindow, hdc);
//							g_bSkipResizing = TRUE;
//							UpdateTextFrameWindowRect();
//
//							SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
//							RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
//							break;		
//						}
//					case 0:
//						{
//							HDC hdc = GetDC(g_hTextFrameWindow);
//
//							TR_SetUnit(UNIT_EP);
//							TR_MeasureTextFrame(hdc, MO_GET_HEIGHT_USING_GIVEN_WIDTH, (tRect*)&g_TextFrameRectF);
//
//							ReleaseDC(g_hTextFrameWindow, hdc);
//							g_bSkipResizing = TRUE;
//							UpdateTextFrameWindowRect();
//
//							SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
//							RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
//							break;
//						}
//					case 1:
//						{
//							HDC hdc = GetDC(g_hTextFrameWindow);
//
//							RECT tempRectF = g_TextFrameRectF;
//						
//							TR_SetUnit(UNIT_EP);
//							TR_MeasureTextFrame(hdc, MO_GET_RECT_USING_WIDEST_WORD, (tRect*)&tempRectF);
//							
//							g_TextFrameRectF.right = tempRectF.right;
//
//							ReleaseDC(g_hTextFrameWindow, hdc);
//							g_bSkipResizing = TRUE;
//							UpdateTextFrameWindowRect();
//
//							SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
//							RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
//							break;
//						}
//					case 2:
//						{
//							HDC hdc = GetDC(g_hTextFrameWindow);
//
//							TR_SetUnit(UNIT_EP);
//							TR_MeasureTextFrame(hdc, MO_GET_RECT_USING_WIDEST_WORD, (tRect*)&g_TextFrameRectF);
//
//							ReleaseDC(g_hTextFrameWindow, hdc);
//							g_bSkipResizing = TRUE;
//							UpdateTextFrameWindowRect();
//
//							SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
//							RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
//							break;
//						}
//					case 3:
//						{
//							HDC hdc = GetDC(g_hTextFrameWindow);
//
//							TR_SetUnit(UNIT_EP);
//							TR_MeasureTextFrame(hdc, MO_GET_RECT_USING_NO_WRAP, (tRect*)&g_TextFrameRectF);
//
//							ReleaseDC(g_hTextFrameWindow, hdc);
//							g_bSkipResizing = TRUE;
//							UpdateTextFrameWindowRect();
//
//							SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
//							RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
//							break;
//						}
//					case 4:
//						{
//							HDC hdc = GetDC(g_hTextFrameWindow);
//
//							TR_SetUnit(UNIT_EP);
//							TR_MeasureTextFrame(hdc, MO_GET_WIDTH_USING_GIVEN_HEIGHT, (tRect*)&g_TextFrameRectF);
//
//							ReleaseDC(g_hTextFrameWindow, hdc);
//							g_bSkipResizing = TRUE;
//							UpdateTextFrameWindowRect();
//
//							SetWindowPos(g_hTextFrameWindow, NULL, 0, 0, g_TextFrameWindowRect.right, g_TextFrameWindowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
//							RedrawTextFrame(g_hTextFrameWindow, (DWORD)UO_UPDATE_ALL);
//							break;
//						}
//					}
//
//					break;
//				}
//
//			case IDC_REDRAW:
//				{
//					HDC hdc = GetDC(g_hTextFrameWindow);
//
//					RECT rt; GetClientRect(g_hTextFrameWindow, &rt);
//					
//					SelectObject(hdc, GetStockObject(WHITE_PEN));
//
//					Rectangle(hdc, 0, 0, rt.right, rt.bottom);
//
////					TR_LayOut(hdc, UO_UPDATE_ALL);
//					TR_Render(hdc, NULL);
//
//					ReleaseDC(g_hTextFrameWindow, hdc);
//
//					break;
//				}
//			}
//
//			break;
//		}
//
//	case WM_SYSCOMMAND:
//		{
//			if (wParam == SC_CLOSE) 
//			{
//				HMENU hMenu = GetMenu(g_hTextFrameWindow);
//				hMenu = GetSubMenu(hMenu, 1);
//
//				CheckMenuItem(hMenu, ID_WINDOWS_FRAME_INFO, MF_BYCOMMAND | MF_UNCHECKED);
//				ShowWindow(hDlg, SW_HIDE);
//				return 0;
//			}
//
//			break;
//		}
//
//	}
//	return FALSE;
//}
//
//LRESULT CALLBACK CodeViewProc(HWND hWnd, INT message, WPARAM wParam, LPARAM lParam)
//{
//	int wmId, wmEvent;
//
//	switch(message) 
//	{
//	case WM_SHOWWINDOW: { g_bCodeViewVisible = (BOOL)wParam; break; }
//
//	case WM_SIZE:
//	case WM_SIZING:
//		{
//			if (g_hCodeViewTextBox)
//			{
//				//Update the size of the text-box control
//				RECT rect;
//				GetClientRect(hWnd, &rect);
//				SetWindowPos(g_hCodeViewTextBox, NULL, 0, 0, rect.right, rect.bottom, SWP_SHOWWINDOW);
//			}
//		}
//	case WM_MOVE:
//		{
//			GetWindowRect(hWnd, &g_CodeViewWindowRect);
//
//			g_CodeViewWindowRect.right -= g_CodeViewWindowRect.left;
//			g_CodeViewWindowRect.bottom -= g_CodeViewWindowRect.top;
//			break;
//		}
//	case WM_CREATE:
//		{
////			g_hCodeViewTextBox	= CreateWindowEx(NULL, RICHEDIT_CLASS, TEXT(""), WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL, 0, 0, 100, 100, hWnd, NULL, hInst, NULL);
//			g_hCodeViewTextBox	= CreateWindowEx(NULL, TEXT("EDIT"), TEXT(""), WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL, 0, 0, 100, 100, hWnd, NULL, g_hInst, NULL);
//			if (!g_hCodeViewTextBox) { return -1; }
//	
//			 
////			SendMessage(g_hCodeViewTextBox, EM_SETTYPOGRAPHYOPTIONS, TO_SIMPLELINEBREAK, TO_SIMPLELINEBREAK);
////			SendMessage(g_hCodeViewTextBox, EM_SETZOOM, 0, 0);
//
//			INT tabWidth = 12;
//			SendMessage(g_hCodeViewTextBox, EM_SETTABSTOPS, 1, (LPARAM)&tabWidth);
//
//			//Update the size of the text-box control
//			RECT rect;
//			GetClientRect(hWnd, &rect);
//			SetWindowPos(g_hCodeViewTextBox, NULL, 0, 0, rect.right, rect.bottom, SWP_SHOWWINDOW);
//
//			break;
//		}
//
//	case WM_COMMAND:
//		{
//			wmId	= LOWORD(wParam); 
//			wmEvent	= HIWORD(wParam); 
//
//			switch(wmEvent)
//			{
//			case EN_UPDATE:
//				{
//					//Delete styles - not really necessary - but the way this test is written, you need to delete them to provide consistency between the content of the "Code View" window and the results produced by the DLL 
//					TR_SetStyleType(STYLE_TEXTFRAME);
//					TR_RemoveAllStyles();
//
//					TR_SetStyleType(STYLE_PARAGRAPH);
//					TR_RemoveAllStyles();
//					TR_ClearDefaultStyle();
//
//					TR_SetStyleType(STYLE_CHARACTER);
//					TR_RemoveAllStyles();
//					TR_ClearDefaultStyle();
//
//					//Update content
//					LPTSTR szNewText = new TCHAR[GetWindowTextLength(g_hCodeViewTextBox) + 1];
//					GetWindowText(g_hCodeViewTextBox, szNewText, GetWindowTextLength(g_hCodeViewTextBox) + 1);
//
//					TR_ExecuteXMLString(szNewText, -1);
//
//					if (szNewText) { delete szNewText; }
//
//				/*	TR_SetStyleType(STYLE_PARAGRAPH);
//
//					TR_AddStyle(1, FALSE);
//					TR_SelectStyle(1);
//
//					tTabEntry	Tabs[2];
//					tTabEntry	Tabs2[2];
//
//					Tabs[0].eAlign		= ALIGN_LEFT;
//					Tabs[0].eUnit		= UNIT_POINT;
//					Tabs[0].fPosition	= 100.0f;
//					Tabs[1].eAlign		= ALIGN_LEFT;
//					Tabs[1].eUnit		= UNIT_POINT;
//					Tabs[1].fPosition	= 100.0f;
//
//					TR_SetComplexProperty(PP_TAB_LIST, Tabs, sizeof(Tabs));
//					TR_GetComplexProperty(PP_TAB_LIST, Tabs2, TR_GetComplexPropertySize(PP_TAB_LIST));
//
//
//					TR_SetStyleType(STYLE_CHARACTER);
//
//					DWORD dwResults = NULL;
//
//					TR_ValidateStyle(1, &dwResults);
//
//					TCHAR	l_szBuffer[64];
//
//					TR_SetStyleType(STYLE_CHARACTER);
//					TR_SelectStyle(2);
//
//					TR_GetStringProperty(CP_FONT_FAMILY, l_szBuffer, 64);*/
//
//					RedrawWindow(g_hTextFrameWindow, NULL, NULL, RDW_INVALIDATE);
//					break;
//				}
//			}
//
//			break;
//		}
//
//	case WM_DESTROY:
//		{
//			break;
//		}
//
//	case WM_SYSCOMMAND:
//		{
//			if (wParam == SC_CLOSE) 
//			{
//				HMENU hMenu = GetMenu(g_hTextFrameWindow);
//				hMenu = GetSubMenu(hMenu, 1);
//
//				CheckMenuItem(hMenu, ID_WINDOWS_CODEVIEW, MF_BYCOMMAND | MF_UNCHECKED);
//				ShowWindow(hWnd, SW_HIDE);
//				return 0;
//			}
//
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//
//	default: { return DefWindowProc(hWnd, message, wParam, lParam); }
//	}
//
//	return 0;
//}
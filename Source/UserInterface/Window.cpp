#include "Window.h"
#include "Paint.h"
#include "Action.h"

using namespace SCFUI;

bool Window_bInitDone = FALSE;

CWindow::CWindow()
{
	if (!Window_bInitDone)
	{
		RegisterWin32Class();
		Window_bInitDone = TRUE;
	}

	this->WindowCreate(Win32ClassCWindow, WS_OVERLAPPED | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, NULL, NULL);
}

CWindow::~CWindow()
{
}

HRESULT CWindow::WindowAdd(_IN CWindowAbstract* pWindow)
{
	UNREFERENCED_PARAMETER(pWindow);
	return E_UNEXPECTED;
}

HRESULT CWindow::WindowRemove(_IN CWindowAbstract* pWindow)
{
	UNREFERENCED_PARAMETER(pWindow);
	return E_UNEXPECTED;
}

////////////////////////////// Private member functions ///////////////////////////

LRESULT CWindow::DoDefaultProcessing(_IN UINT uiMessage, _IN WPARAM wParam, _IN LPARAM lParam)
{
	switch(uiMessage) 
	{
	case WM_SYSCOMMAND: 
		{
			switch (wParam)
			{
			case SC_CLOSE:
				{
					CAction ActionClosed(ActionClosed, this);
					
					ActionPerform(ActionClosed);
					break;
				}
			}
		}
	}

	return CWindowAbstract::DoDefaultProcessing(uiMessage, wParam, lParam);
}

void CWindow::OnWindowPaint(_IN CEventWindow& rEvent)
{
	UNREFERENCED_PARAMETER(rEvent);
}

bool CWindow::RegisterWin32Class()
{
	WNDCLASSEX l_WndClassEx;
	l_WndClassEx.cbSize = sizeof(WNDCLASSEX);

	l_WndClassEx.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	l_WndClassEx.lpfnWndProc	= CWindowAbstract::WindowProc;
	l_WndClassEx.cbClsExtra		= 0;
	l_WndClassEx.cbWndExtra		= 0;
	l_WndClassEx.hInstance		= (HINSTANCE)GetModuleHandle(NULL);
	l_WndClassEx.hIcon			= NULL;
	l_WndClassEx.hCursor		= LoadCursor(NULL, IDC_ARROW);
	l_WndClassEx.hbrBackground	= NULL;
	l_WndClassEx.lpszMenuName	= NULL;
	l_WndClassEx.lpszClassName	= Win32ClassCWindow;
	l_WndClassEx.hIconSm		= NULL;

	if (!RegisterClassEx(&l_WndClassEx)) { return FALSE; }

	return TRUE;
}

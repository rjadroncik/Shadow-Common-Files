#include "Button.h"
#include "Paint.h"
#include "Event.h"

using namespace SCFUI;

bool Button_bInitDone = FALSE;

CButton::CButton()
{
	if (!Button_bInitDone)
	{
		RegisterWin32Class();
		Button_bInitDone = TRUE;
	}

	m_uiBevel = 1;
}

CButton::~CButton()
{
}

bool CButton::RegisterWin32Class()
{
	WNDCLASSEX l_WndClassEx;
	l_WndClassEx.cbSize = sizeof(WNDCLASSEX); 

	l_WndClassEx.style			= NULL;
	l_WndClassEx.lpfnWndProc	= CWindowAbstract::WindowProc;
	l_WndClassEx.cbClsExtra		= 0;
	l_WndClassEx.cbWndExtra		= 0;
	l_WndClassEx.hInstance		= (HINSTANCE)GetModuleHandle(NULL);
	l_WndClassEx.hIcon			= NULL;
	l_WndClassEx.hCursor		= LoadCursor(NULL, IDC_ARROW);
	l_WndClassEx.hbrBackground	= NULL;
	l_WndClassEx.lpszMenuName	= NULL;
	l_WndClassEx.lpszClassName	= Win32ClassCButton;
	l_WndClassEx.hIconSm		= NULL;

	if (!RegisterClassEx(&l_WndClassEx)) { return FALSE; }

	return TRUE;
}
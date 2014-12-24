#include "MessageLoop.h"
#include "Definitions.h"

using namespace SCFUI;

CMessageLoop::CMessageLoop()
{

}

CMessageLoop::~CMessageLoop()
{

}	

int CMessageLoop::Enter()
{
	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

void CMessageLoop::Quit()
{
	PostQuitMessage(0);
}


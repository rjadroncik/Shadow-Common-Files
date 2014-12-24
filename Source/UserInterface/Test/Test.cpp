#include "Test.h"

#include <SCFUserInterface.h>

using namespace SCFUI;
using namespace SCFGraphics;

CMessageLoop MessageQueue;

void FunctionTest(IN CAction& rAction)
{
	if (rAction.Source().Tag() == (void*)1)
	{
		MessageQueue.Quit();
	}
	else
	{
		//MessageBoxW(NULL, rAction.IDText(), rAction.Source().TypeName(), MB_OK);
	}
}

void CloseApplication(IN CAction& rAction)
{
	MessageQueue.Quit();
}

void OnAppClose(IN CAction& rAction)
{
	if (rAction.ID() == ActionClosed)
	{
		MessageQueue.Quit();
	}
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);
	UNREFERENCED_PARAMETER(hInstance);

// 	CWindow Window;
// 	Window.Visible(TRUE);
// 	
// 	CButtonClick Button(&Window);
// 	Button.Label(TEXT("Testik (Quit)"));
// 	Button.RectWindow(Rect4i(100, 10, 100, 20));
// 	Button.Visible(TRUE);
// 
// 	Button.ActionHandlerAdd((ActionHandler)CloseApplication);
// 	Window.ActionHandlerAdd((ActionHandler)OnAppClose);

 	CWindow Window;
	Window.Visible(TRUE);

	CButtonClick Button(&Window);

	Button.AutoSize(TRUE);
	Button.Visible(TRUE);
	Button.Tag((void*)1);

	Button.RectWindow(Rect4i(10, 10, 100, 20));
	Button.Label(TEXT("Testik (Quit)"));
	Button.Bevel(2);
	Button.Transparent(TRUE);

	CButtonRadio Button2(&Window);

	Button2.AutoSize(TRUE);
	Button2.Visible(TRUE);

	Button2.RectWindow(Rect4i(10, 60, 100, 20));
	Button2.Label(TEXT("Testik 2nnbvnvnb"));

	SCFGraphics::Rect4i rect(Button2.RectWindow());
	Button2.AutoRect(rect);
	Button2.RectWindow(rect);

	CButtonCheck Button3(&Window);

	Button3.AutoSize(TRUE);
	Button3.Visible(TRUE);

	Button3.RectWindow(Rect4i(10, 110, 100, 14));
	Button3.Label(TEXT("Testik 3"));

	CButtonPush Button4(&Window);

	Button4.AutoSize(TRUE);
	Button4.Visible(TRUE);

	Button4.RectWindow(Rect4i(10, 160, 60, 16));
	Button4.Label(TEXT("Testik 4"));
	Button4.Border(FALSE);

	CEdit Edit(&Window);

	Edit.AutoSize(TRUE);
	Edit.Visible(TRUE);

	Edit.RectWindow(Rect4i(10, 210, 100, 14));
	Edit.Label(TEXT("Testik 5"));

	Button.ActionHandlerAdd ((ActionHandler)FunctionTest);
	Button2.ActionHandlerAdd((ActionHandler)FunctionTest);
	Button3.ActionHandlerAdd((ActionHandler)FunctionTest);
	Button4.ActionHandlerAdd((ActionHandler)FunctionTest);

	CSpinner Spinner(&Window);

	Spinner.AutoSize(TRUE);
	Spinner.Visible(TRUE);

	Spinner.RectWindow(Rect4i(Edit.RectWindow().iX + Edit.RectWindow().iWidth + 3, 210, 12, 14));

	Window.ActionHandlerAdd((ActionHandler)OnAppClose);

	Spinner.Buddy(&Edit);

	if (CFile(TEXT("Test.xml")).Exists())
	{
		CModule Module1;
		Module1.BuildFrom(TEXT("Test.xml"));

	}

	CInt Cislo1(5), Cislo2(17), Cislo3(31);

	Cislo3 = Cislo1 - Cislo2;

	//MessageBox( NULL, Cislo3.ToString().Value(), NULL, MB_OK);

	return MessageQueue.Enter();
}

#include "Edit.h"
#include "Paint.h"

using namespace SCFUI;

bool Edit_bInitDone = FALSE;

HWND	g_hEditOrigin    = NULL;
HFONT	g_hDefaultFont   = NULL;
WNDPROC	g_EditProcOrigin = NULL;

CEdit::CEdit(CWindowAbstract* pParent)
{
	if (!Edit_bInitDone)
	{
		Edit_bInitDone = TRUE;

		//Spinner Control SubClassing
		g_hEditOrigin    = CreateWindowEx(WS_EX_TRANSPARENT, TEXT("EDIT"), TEXT(""), WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, pParent->WindowHandle(), (HMENU)999, (HINSTANCE)GetModuleHandle(NULL), (CWindowAbstract*)this);
#pragma warning(disable : 4244 4312)
		g_EditProcOrigin = (WNDPROC)SetClassLongPtr( g_hEditOrigin, GCLP_WNDPROC, (LONG_PTR)(WNDPROC)CEdit::WindowProc);
#pragma warning(default : 4244 4312)

		Graphics Graph(g_hEditOrigin);

		LOGFONTW logFont;
		CPaint::SysFonts.pArial_11px->GetLogFontW(&Graph, &logFont);

		g_hDefaultFont = CreateFontIndirect(&logFont);
 	}

	m_pParent = pParent;
	m_hWnd = CreateWindowEx(WS_EX_TRANSPARENT, TEXT("EDIT"), TEXT(""), WS_CHILD | WS_BORDER | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, pParent->WindowHandle(), (HMENU)999, (HINSTANCE)GetModuleHandle(NULL), (CWindowAbstract*)this);

#pragma warning(disable : 4244)
	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)(CWindowAbstract*)this);
#pragma warning(default : 4244)

	SendMessage(m_hWnd, WM_SETFONT, (WPARAM)g_hDefaultFont, TRUE);
}

CEdit::~CEdit()
{
}

bool CEdit::RegisterWin32Class()
{
	return TRUE;
}

LRESULT	CEdit::DoDefaultProcessing(_IN UINT uiMessage, _IN WPARAM wParam, _IN LPARAM lParam)
{
	switch (uiMessage)
	{
	case WM_PAINT:
		{
			static SCFGraphics::Rect4i s_UpdateRect;

			if (GetUpdateRect(m_hWnd, (LPRECT)&s_UpdateRect, FALSE))
			{
				{
					static PAINTSTRUCT s_PaintStruct;
					Graphics* pGraph = new Graphics(BeginPaint(m_hWnd, &s_PaintStruct));

					pGraph->SetClip(Rect(s_UpdateRect.iX, s_UpdateRect.iY, s_UpdateRect.iWidth, s_UpdateRect.iHeight));

					//Scope for the Event
					{
						CEventWindow Event(uiMessage, this);
						Event.Canvas(pGraph);

						OnWindowPaint(Event); 
					}
					EndPaint(m_hWnd, &s_PaintStruct);
				}

				//This omits the 1-pixel border from the update rectangle
				s_UpdateRect.iX      = __max(1, s_UpdateRect.iX);
				s_UpdateRect.iY      = __max(1, s_UpdateRect.iY);
				s_UpdateRect.iWidth  = __min(this->RectWindow().iWidth  - 1, s_UpdateRect.iWidth);
				s_UpdateRect.iHeight = __min(this->RectWindow().iHeight - 1, s_UpdateRect.iHeight);
 
				InvalidateRect(m_hWnd, (LPRECT)&s_UpdateRect, FALSE);
			}

			break;
		}
	}	

	return g_EditProcOrigin(m_hWnd, uiMessage, wParam, lParam);
}

LRESULT CALLBACK CEdit::WindowProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
	if (GetWindowLongPtr(hWnd, GWLP_USERDATA))
	{
		return ((CWindowAbstract*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA))->DoDefaultProcessing(uiMessage, wParam, lParam);
	}
	else
	{
		return g_EditProcOrigin(hWnd, uiMessage, wParam, lParam);
	}
}

void CEdit::OnWindowPaint(_IN CEventWindow& rEvent)
{
	//Prepare variables
	SCFGraphics::Rect4i rt = this->RectWindow(); rt.iX = 0; rt.iY = 0;

	//Prepare drawing canvas
	rEvent.Canvas()->SetSmoothingMode(SmoothingModeHighQuality);

	//Draw button rim
	if (this->Border()) { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, 1, CPaint::SysPens.pDarkShadow, CPaint::SysPens.pLight, NULL); }

	rEvent.Canvas()->SetSmoothingMode(SmoothingModeNone);
}

bool CEdit::OnWindowPaintBackground(_IN CEventWindow& rEvent)
{
	UNREFERENCED_PARAMETER(rEvent);
	return TRUE;
}

Int2& CEdit::Selection()
{
	SendMessage(m_hWnd, EM_GETSEL, (WPARAM)&m_Selection[0], (LPARAM)&m_Selection[1]);
	return m_Selection;
}

void CEdit::Selection(_IN Int2& rRange)
{
	SendMessage(m_hWnd, EM_SETSEL, rRange[0], rRange[1]);
}

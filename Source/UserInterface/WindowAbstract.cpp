#include "WindowAbstract.h"
#include "Paint.h"

using namespace SCFUI;

CWindowAbstract::CWindowAbstract()
{
	m_Rect.iHeight = 0;
	m_Rect.iX      = 0;
	m_Rect.iWidth  = 0;
	m_Rect.iY      = 0;

	m_hWnd    = NULL;
	m_szLabel = NULL;

	m_vpTag   = NULL;
	m_dwStyle = 0x00000000;

	m_pEventListeners      = NULL;
	m_uiEventListenerCount = 0;

	m_pActionHandlers      = NULL;
	m_uiActionHandlerCount = 0;
}

CWindowAbstract::~CWindowAbstract()
{
	if (m_szLabel) { free(m_szLabel);		m_szLabel = NULL; }
	if (m_hWnd)    { DestroyWindow(m_hWnd);	m_hWnd    = NULL; }
}

void CWindowAbstract::Label(_IN LPTSTR pBuffer)
{
	if (!pBuffer) { return; }

	m_szLabel = (LPTSTR)realloc(m_szLabel, sizeof(TCHAR) * (lstrlen(pBuffer) + 1));
	lstrcpy(m_szLabel, pBuffer);

	SetWindowText(m_hWnd, m_szLabel);
} 

SCFGraphics::Rect4i CWindowAbstract::RectWindow() _GET { return m_Rect; }

void CWindowAbstract::RectWindow(_IN SCFGraphics::Rect4i& rRect) _SET
{ 
	m_Rect = rRect;
//	MoveWindow(m_hWnd, m_Rect.iX, m_Rect.iY, m_Rect.iWidth, m_Rect.iHeight, TRUE);
	SetWindowPos(m_hWnd, NULL, m_Rect.iX, m_Rect.iY, m_Rect.iWidth, m_Rect.iHeight, SWP_NOZORDER);
}

void CWindowAbstract::Redraw(_IN bool bErase) { if (bErase) { RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE); } else { RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE); } }

int  CWindowAbstract::OnWindowCreate         (_IN CEventWindow& rEvent) { UNREFERENCED_PARAMETER(rEvent); return 0; }
void CWindowAbstract::OnWindowDestroy        (_IN CEventWindow& rEvent) { UNREFERENCED_PARAMETER(rEvent); m_hWnd = NULL; }

void CWindowAbstract::OnWindowMove           (_IN CEventWindow& rEvent) 
{ 
	m_Rect = rEvent.RectWindow();
//	UNREFERENCED_PARAMETER(rEvent); 
}
bool CWindowAbstract::OnWindowMoving         (_IN CEventWindow& rEvent) 
{ 
	m_Rect = rEvent.RectWindow();
//	UNREFERENCED_PARAMETER(rEvent); 
	return FALSE; 
}
void CWindowAbstract::OnWindowSize           (_IN CEventWindow& rEvent) 
{ 
	m_Rect = rEvent.RectWindow();
}
bool CWindowAbstract::OnWindowSizing         (_IN CEventWindow& rEvent)
{
	m_Rect = rEvent.RectWindow(); return TRUE; 
}
bool CWindowAbstract::OnWindowPaintBackground(_IN CEventWindow& rEvent) { rEvent.Canvas()->Clear(*CPaint::SysColors.pFace); return TRUE; }

void CWindowAbstract::OnMouseLeftDown         (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseLeftUp           (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseLeftDoubleClick  (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseMiddleDown       (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseMiddleUp         (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseMiddleDoubleClick(_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseRightDown        (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseRightUp          (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseRightDoubleClick (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseMove             (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseWheel            (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnMouseCaptuereLost     (_IN CEventMouse& rEvent) { UNREFERENCED_PARAMETER(rEvent); }

void CWindowAbstract::OnKeyChar         (_IN CEventKey& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnKeyDown         (_IN CEventKey& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnKeyUp           (_IN CEventKey& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnKeySystemDown   (_IN CEventKey& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnKeySystemUp     (_IN CEventKey& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnKeyFocusReceived(_IN CEventKey& rEvent) { UNREFERENCED_PARAMETER(rEvent); }
void CWindowAbstract::OnKeyFocusLost    (_IN CEventKey& rEvent) { UNREFERENCED_PARAMETER(rEvent); this->Redraw(TRUE); }

void CWindowAbstract::OnTimer(_IN UINT uiID) { UNREFERENCED_PARAMETER(uiID); }

LRESULT CWindowAbstract::DoDefaultProcessing(_IN UINT uiMessage, _IN WPARAM wParam, _IN LPARAM lParam)
{
	static PAINTSTRUCT s_PaintStruct;

	CEvent* pEvent = NULL;
	HDC hDC = NULL;

	LRESULT lRetVal = FALSE;

	switch (uiMessage)
	{
	case EventWindowCreate:
	case EventWindowDestroy:
		{
			pEvent = new CEventWindow(uiMessage, this);
			break;
		}
	case EventWindowMove:
	case EventWindowSize:
		{
			pEvent = new CEventWindow(uiMessage, this);

			SCFGraphics::Rect4i rt; GetWindowRect(m_hWnd, (LPRECT)&rt);
			rt.iWidth  -= rt.iX;
			rt.iHeight -= rt.iY;

			((CEventWindow*)pEvent)->RectWindow(rt);
			break; 
		}
	case EventWindowMoving:
	case EventWindowSizing:
		{
			pEvent = new CEventWindow(uiMessage, this);

			SCFGraphics::Rect4i rt = *(SCFGraphics::Rect4i*)lParam;
			rt.iWidth  -= rt.iX; 
			rt.iHeight -= rt.iY; 

			((CEventWindow*)pEvent)->RectWindow(rt);
			break;
		}
	case EventWindowPaint:
		{
			pEvent = new CEventWindow(uiMessage, this);

			static SCFGraphics::Rect4i s_UpdateRect;
			GetUpdateRect(m_hWnd, (LPRECT)&s_UpdateRect, FALSE);

			hDC = BeginPaint(m_hWnd, &s_PaintStruct);
			Graphics* pGraph = new Graphics(hDC);
			pGraph->SetClip(Rect(s_UpdateRect.iX, s_UpdateRect.iY, s_UpdateRect.iWidth, s_UpdateRect.iHeight));

			((CEventWindow*)pEvent)->RectUpdate(s_UpdateRect);
			((CEventWindow*)pEvent)->Canvas(pGraph);
			break;
		}
	case EventWindowPaintBackground:
		{
			pEvent = new CEventWindow(uiMessage, this);

			Graphics* pGraph = new Graphics((HDC)wParam);

			((CEventWindow*)pEvent)->Canvas(pGraph);
			break;
		}
	case EventMouseLeftDown:
	case EventMouseLeftUp:
	case EventMouseLeftDoubleClick:
	case EventMouseMiddleDown:
	case EventMouseMiddleUp:
	case EventMouseMiddleDoubleClick:
	case EventMouseRightDown:
	case EventMouseRightUp:
	case EventMouseRightDoubleClick:
	case EventMouseMove:
		{
			pEvent = new CEventMouse(uiMessage, this);
	
			SCFGraphics::Point2i pt;  pt.iX = GET_X_LPARAM(lParam); pt.iY = GET_Y_LPARAM(lParam);

			((CEventMouse*)pEvent)->Position(pt);
			((CEventMouse*)pEvent)->Flags((UINT)wParam);
			break;
		}
	case EventMouseWheel:
		{
			pEvent = new CEventMouse(uiMessage, this);

			SCFGraphics::Point2i pt;  pt.iX = GET_X_LPARAM(lParam); pt.iY = GET_Y_LPARAM(lParam);

			((CEventMouse*)pEvent)->Position(pt);
			((CEventMouse*)pEvent)->Flags     ((UINT)GET_KEYSTATE_WPARAM(wParam));
			((CEventMouse*)pEvent)->WheelDelta((int)GET_WHEEL_DELTA_WPARAM(wParam));
			break;
		}
	case EventMouseCaptuereLost:
		{
			pEvent = new CEventMouse(uiMessage, this);
			break;
		}
	case EventKeyChar:
		{
			pEvent = new CEventKey(uiMessage, this);

			((CEventKey*)pEvent)->Character  ((UINT)wParam);
			((CEventKey*)pEvent)->RepeatCount((UINT)lParam & 0xFFFF);
			break;
		}
	case EventKeyDown:
	case EventKeyUp:
	case EventKeySystemDown:
	case EventKeySystemUp:
		{
			pEvent = new CEventKey(uiMessage, this);

			((CEventKey*)pEvent)->VirtualKey ((UINT)wParam);
			((CEventKey*)pEvent)->RepeatCount((UINT)lParam & 0xFFFF);
			break;
		}
	case EventKeyFocusReceived:
	case EventKeyFocusLost:
		{
			pEvent = new CEventKey(uiMessage, this);
 			break;
		}
	}

	//If event processing returns [FALSE] it means we should not continue processing the event
//	if (!m_uiEventListenerCount || !pEvent || EventAnnounce(*pEvent)) 
	if (pEvent && EventAnnounce(*pEvent)) 
	{ 
		switch (uiMessage) 
		{
		case EventWindowCreate:	 { lRetVal = OnWindowCreate (*(CEventWindow*)pEvent); break; }
		case EventWindowDestroy: {           OnWindowDestroy(*(CEventWindow*)pEvent); break; }

		case EventWindowMove:    { OnWindowMove  (*(CEventWindow*)pEvent); break; }
		case EventWindowSize:    { OnWindowSize  (*(CEventWindow*)pEvent); break; }
		case EventWindowMoving:  { OnWindowMoving(*(CEventWindow*)pEvent); SCFGraphics::Rect4i rt = ((CEventWindow*)pEvent)->RectWindow(); rt.iWidth += rt.iX; rt.iHeight += rt.iY; *(SCFGraphics::Rect4i*)lParam = rt; lRetVal = TRUE; break; }
		case EventWindowSizing:  { OnWindowSizing(*(CEventWindow*)pEvent); SCFGraphics::Rect4i rt = ((CEventWindow*)pEvent)->RectWindow(); rt.iWidth += rt.iX; rt.iHeight += rt.iY; *(SCFGraphics::Rect4i*)lParam = rt; lRetVal = TRUE; break; }

		case EventWindowPaintBackground: { lRetVal = OnWindowPaintBackground(*(CEventWindow*)pEvent); break; }
		case EventWindowPaint:           { OnWindowPaint(*(CEventWindow*)pEvent); break; }
	
		case EventMouseLeftDown:	      { OnMouseLeftDown         (*(CEventMouse*)pEvent); break; }
		case EventMouseLeftUp:		      { OnMouseLeftUp           (*(CEventMouse*)pEvent); break; }
		case EventMouseLeftDoubleClick:	  { OnMouseLeftDoubleClick  (*(CEventMouse*)pEvent); break; }
		case EventMouseMiddleDown:	      { OnMouseMiddleDown       (*(CEventMouse*)pEvent); break; }
		case EventMouseMiddleUp:		  { OnMouseMiddleUp         (*(CEventMouse*)pEvent); break; }
		case EventMouseMiddleDoubleClick: { OnMouseMiddleDoubleClick(*(CEventMouse*)pEvent); break; }
		case EventMouseRightDown:	      { OnMouseRightDown        (*(CEventMouse*)pEvent); break; }
		case EventMouseRightUp:		      { OnMouseRightUp          (*(CEventMouse*)pEvent); break; }
		case EventMouseRightDoubleClick:  { OnMouseRightDoubleClick (*(CEventMouse*)pEvent); break; }
		case EventMouseMove:		      { OnMouseMove             (*(CEventMouse*)pEvent); break; }
		case EventMouseWheel:		      { OnMouseWheel            (*(CEventMouse*)pEvent); break; }
		case EventMouseCaptuereLost:      { OnMouseCaptuereLost     (*(CEventMouse*)pEvent); break; }
		
		case EventKeyChar:		    { OnKeyChar         (*(CEventKey*)pEvent); break; }
		case EventKeyDown:	        { OnKeyDown         (*(CEventKey*)pEvent); break; }
		case EventKeyUp:		    { OnKeyUp           (*(CEventKey*)pEvent); break; }
		case EventKeySystemDown:	{ OnKeySystemDown   (*(CEventKey*)pEvent); break; }
		case EventKeySystemUp:	    { OnKeySystemUp     (*(CEventKey*)pEvent); break; }
		case EventKeyFocusReceived: { OnKeyFocusReceived(*(CEventKey*)pEvent); break; }
		case EventKeyFocusLost:     { OnKeyFocusLost    (*(CEventKey*)pEvent); break; }

		case WM_TIMER: { OnTimer((UINT)wParam); break; }

		default: { delete pEvent; return DefWindowProc(m_hWnd, uiMessage, wParam, lParam); }
		}
	}
	else { return DefWindowProc(m_hWnd, uiMessage, wParam, lParam); }

	if (pEvent) { delete pEvent; }
	if (hDC)    { EndPaint(m_hWnd, &s_PaintStruct); }
	return lRetVal;
}

LRESULT CALLBACK CWindowAbstract::WindowProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{


	if (uiMessage == WM_NCCREATE)
	{
		return DefWindowProc(hWnd, uiMessage, wParam, lParam);
	}

	if (uiMessage == WM_CREATE)
	{
#pragma warning(disable : 4244)
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams);
#pragma warning(default : 4244)
	
		return DefWindowProc(hWnd, uiMessage, wParam, lParam);
	}

	if (GetWindowLongPtr(hWnd, GWLP_USERDATA))
	{
		return ((CWindowAbstract*)(LONG_PTR)GetWindowLongPtr(hWnd, GWLP_USERDATA))->DoDefaultProcessing(uiMessage, wParam, lParam);
	}

	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}

bool CWindowAbstract::WindowCreate(_IN LPTSTR szClass, _IN DWORD dwStyle, _IN DWORD dwStyleEx, _IN CWindowAbstract* pParent)
{
	HWND hParent = (pParent) ? (hParent = pParent->WindowHandle()) : (NULL);

	m_hWnd = CreateWindowEx(dwStyleEx, szClass, TEXT(""), dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, NULL, (HINSTANCE)GetModuleHandle(NULL), (CWindowAbstract*)this);

	return (m_hWnd) ? (TRUE) : (FALSE);
}

void CWindowAbstract::ActionHandlerAdd(_IN ActionHandler pFuction)
{
	m_uiActionHandlerCount++;
	m_pActionHandlers = (SActionBinding*)realloc(m_pActionHandlers, sizeof(SActionBinding) * m_uiActionHandlerCount);

	m_pActionHandlers[m_uiActionHandlerCount - 1].bClass    = FALSE;
	m_pActionHandlers[m_uiActionHandlerCount - 1].pFunction = pFuction;
}

void CWindowAbstract::ActionHandlerAdd(_INOUT IActionHandler* pInterface)
{
	m_uiActionHandlerCount++;
	m_pActionHandlers = (SActionBinding*)realloc(m_pActionHandlers, sizeof(SActionBinding) * m_uiActionHandlerCount);

	m_pActionHandlers[m_uiActionHandlerCount - 1].bClass     = TRUE;
	m_pActionHandlers[m_uiActionHandlerCount - 1].pInterface = pInterface;
}

void CWindowAbstract::EventListenerAdd(_IN EventListener pFuction)
{
	m_uiEventListenerCount++;
	m_pEventListeners = (SEventBinding*)realloc(m_pEventListeners, sizeof(SEventBinding) * m_uiEventListenerCount);

	m_pEventListeners[m_uiEventListenerCount - 1].bClass    = FALSE;
	m_pEventListeners[m_uiEventListenerCount - 1].pFunction = pFuction;
}

void CWindowAbstract::EventListenerAdd(_INOUT IEventListener* pInterface)
{
	m_uiEventListenerCount++;
	m_pEventListeners = (SEventBinding*)realloc(m_pEventListeners, sizeof(SEventBinding) * m_uiEventListenerCount);

	m_pEventListeners[m_uiEventListenerCount - 1].bClass     = TRUE;
	m_pEventListeners[m_uiEventListenerCount - 1].pInterface = pInterface;
} 

bool CWindowAbstract::ActionPerform(_IN CAction& rAction)
{
	for (UINT i = 0; i < m_uiActionHandlerCount; i++)
	{
		if (m_pActionHandlers[i].bClass) { if (!m_pActionHandlers[i].pInterface->OnAction(rAction)) { return FALSE; } }
		else                             { if (!m_pActionHandlers[i].pFunction(rAction))            { return FALSE; } }
	}
	return TRUE;
}

bool CWindowAbstract::EventAnnounce(_IN CEvent& rEvent)
{
	for (UINT i = 0; i < m_uiEventListenerCount; i++)
	{
		if (m_pEventListeners[i].bClass) { if (!m_pEventListeners[i].pInterface->OnEvent(rEvent)) { return FALSE; } }
		else                             { if (!m_pEventListeners[i].pFunction(rEvent))           { return FALSE; } }
	}

	return TRUE;
}

void CWindowAbstract::TimerStart(_IN UINT uiID, _IN UINT uiMilliseconds)
{
	SetTimer(m_hWnd, uiID, uiMilliseconds, NULL);
}

void CWindowAbstract::TimerStop(_IN UINT uiID)
{
	KillTimer(m_hWnd, uiID);
}



#include "WindowChild.h"
#include "Paint.h"

using namespace SCFUI;

CWindowChild::CWindowChild()
{
	m_pParent = NULL;
	m_bHot = FALSE;

	this->HotTrack(TRUE);
	this->Border(TRUE);
	this->FocusFrame(TRUE);
}

CWindowChild::~CWindowChild()
{
}

void CWindowChild::OnMouseLeftDown(_IN CEventMouse& rEvent)
{
	UNREFERENCED_PARAMETER(rEvent);

	//Needs to be done every time
	this->MouseCapture(TRUE);
	this->Focused(TRUE);
}

void CWindowChild::OnMouseLeftUp(_IN CEventMouse& rEvent)
{
	UNREFERENCED_PARAMETER(rEvent);

	//Needs to be done every time
	this->MouseCapture(FALSE);
	this->Redraw(TRUE);
}

void CWindowChild::OnMouseMove(_IN CEventMouse& rEvent)
{
	SCFGraphics::Rect4i rt = this->RectWindow(); rt.iX = 0; rt.iY = 0;

	//Handle standard hot-tracking - needs additional processing if iX mouse button is pressed
	if (this->HotTrack())
	{
		if (this->MouseCapture())
		{
			//Continue hot-tracking
			if (!PointInRect(rEvent.Position(), rt)) 
			{ 
				//The mouse iX the window - change state, redraw, end hot-tracking
				m_bHot = FALSE; 
				if (!rEvent.ButtonLeft()) { this->MouseCapture(FALSE); this->Redraw(TRUE); }
			}
			else
			{
				//The is in the window, redraw only if state changes
				if (!m_bHot) { m_bHot = TRUE; this->Redraw(TRUE); }
			}
		}
		else
		{
			//Start hot-tracking, change state, redraw
			m_bHot = TRUE;
			this->MouseCapture(TRUE);
			this->Redraw(TRUE);
		}
	}
}

bool CWindowChild::WindowCreate(_IN LPTSTR szClass, _IN DWORD dwStyle, _IN DWORD dwStyleEx, _INOUT CWindowAbstract* pParent)
{
	m_pParent = pParent;

	return CWindowAbstract::WindowCreate(szClass, dwStyle, dwStyleEx, pParent);
}

bool CWindowChild::AutoSize()                         _GET { return this->Style(StyleAutosize); }
void CWindowChild::AutoSize(_IN bool bEnable)         _SET {        this->Style(StyleAutosize, bEnable); }

bool CWindowChild::HotTrack()                         _GET { return this->Style(StyleHotTrack); }
void CWindowChild::HotTrack(_IN bool bEnable)         _SET {        this->Style(StyleHotTrack, bEnable); }

bool CWindowChild::AllowDeactivate()                  _GET { return this->Style(StyleAllowDeactivate); }
void CWindowChild::AllowDeactivate(_IN bool bAllow)   _SET {        this->Style(StyleAllowDeactivate, bAllow); }

bool CWindowChild::FocusFrame()                       _GET { return this->Style(StyleFocusFrame); }
void CWindowChild::FocusFrame(_IN bool bEnable)       _SET {        this->Style(StyleFocusFrame, bEnable); }

bool CWindowChild::Border()                           _GET { return this->Style(StyleBorder); }
void CWindowChild::Border(_IN bool bEnable)           _SET {        this->Style(StyleBorder, bEnable); }

bool CWindowChild::Transparent()                      _GET { return this->Style(StyleTransparent); }
void CWindowChild::Transparent(_IN bool bTransparent) _SET { this->Style(StyleTransparent, bTransparent); }

bool CWindowChild::OnWindowPaintBackground(_IN CEventWindow& rEvent) 
{ 
	if (!this->Transparent()) { rEvent.Canvas()->Clear(*CPaint::SysColors.pFace); } 
	return TRUE; 
}

LRESULT CWindowChild::DoDefaultProcessing(_IN UINT uiMessage, _IN WPARAM wParam, _IN LPARAM lParam)
{
	CEvent* pEvent = NULL;
	LRESULT lRetVal = FALSE;

	switch (uiMessage)
	{
	case EventWindowMove:
	case EventWindowSize:
		{
			pEvent = new CEventWindow(uiMessage, this);

			SCFGraphics::Rect4i rt; GetWindowRect(WindowHandle(), (LPRECT)&rt);
			rt.iWidth  -= rt.iX;
			rt.iHeight -= rt.iY;

			ScreenToClient(Parent()->WindowHandle(), (LPPOINT)&rt);

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

			ScreenToClient(Parent()->WindowHandle(), (LPPOINT)&rt);

			((CEventWindow*)pEvent)->RectWindow(rt);
			break;
		}

	default: { return CWindowAbstract::DoDefaultProcessing(uiMessage, wParam, lParam); }
	}

	//If event processing returns [FALSE] it means we should not continue processing the event
	if (pEvent && EventAnnounce(*pEvent)) 
	{ 
		switch (uiMessage) 
		{
		case EventWindowMove:    { OnWindowMove  (*(CEventWindow*)pEvent); break; }
		case EventWindowSize:    { OnWindowSize  (*(CEventWindow*)pEvent); break; }
		case EventWindowMoving:  { OnWindowMoving(*(CEventWindow*)pEvent); SCFGraphics::Rect4i rt = ((CEventWindow*)pEvent)->RectWindow(); rt.iWidth += rt.iX; rt.iHeight += rt.iY; *(SCFGraphics::Rect4i*)lParam = rt; lRetVal = TRUE; break; }
		case EventWindowSizing:  { OnWindowSizing(*(CEventWindow*)pEvent); SCFGraphics::Rect4i rt = ((CEventWindow*)pEvent)->RectWindow(); rt.iWidth += rt.iX; rt.iHeight += rt.iY; *(SCFGraphics::Rect4i*)lParam = rt; lRetVal = TRUE; break; }
		}
	}

	if (pEvent) { delete pEvent; }
	return lRetVal;
}
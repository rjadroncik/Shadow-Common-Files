#include "ButtonClick.h"
#include "Paint.h"
#include "Event.h"

using namespace SCFUI;

CButtonClick::CButtonClick(_INOUT CWindowAbstract* pParent)
{
	m_bActive = FALSE;

	this->WindowCreate(Win32ClassCButton, WS_CHILD, WS_EX_TRANSPARENT, pParent);
}

CButtonClick::~CButtonClick()
{
}

void CButtonClick::OnWindowPaint(_IN CEventWindow& rEvent)
{
	SCFGraphics::Rect4i rt = this->RectWindow(); rt.iX = 0; rt.iY = 0;
	RectF rect((float)rt.iX, (float)rt.iY, (float)rt.iWidth, (float)rt.iHeight);

	rEvent.Canvas()->SetSmoothingMode(SmoothingModeHighQuality);

	if (m_bActive)
	{
		LinearGradientBrush brush(Point(0, 0), Point(0, this->RectWindow().iHeight), *CPaint::SysColors.pFace, *CPaint::SysColors.pLightFace);
		CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, NULL, NULL, &brush); 
	}
	else 
	{
		LinearGradientBrush brush(Point(0, 0), Point(0, this->RectWindow().iHeight), *CPaint::SysColors.pLightFace, *CPaint::SysColors.pFace);
		CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, NULL, NULL, &brush); 
	}

	if (m_bHot) 
	{
		CPaint::RectResize(rt, -1);
		if (m_bActive) { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pLightShadow_3px, CPaint::SysPens.pLightShadow_3px, NULL); }
		else           { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pOrange_3px,      CPaint::SysPens.pOrange_3px,      NULL); }
		CPaint::RectResize(rt, 1);
	}

	if (m_bActive) { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pDarkShadow, CPaint::SysPens.pLight,      NULL); }
	else           
	{
		if (this->Border())
		{
			CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pLight,      CPaint::SysPens.pDarkShadow, NULL); 
		}
	}

	StringFormat format;
	format.SetAlignment    (StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);

	if (m_bActive) { rect.X++; rect.Y++; }
	rEvent.Canvas()->DrawString(this->Label(), -1, CPaint::SysFonts.pArial_11px, rect, &format, CPaint::SysBrushes.pBlack);

	if (this->FocusFrame() && this->Focused())
	{
		CPaint::RectResize(rt, -3);

		rEvent.Canvas()->SetSmoothingMode(SmoothingModeNone);
		CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel - 1, CPaint::SysPens.pDottedGray, CPaint::SysPens.pDottedGray, NULL);
		rEvent.Canvas()->SetSmoothingMode(SmoothingModeHighQuality);
	}
}

void CButtonClick::OnMouseLeftDown(_IN CEventMouse& rEvent)
{
	CButton::OnMouseLeftDown(rEvent);

	m_bActive = TRUE;
	this->Redraw(TRUE);
}

void CButtonClick::OnMouseLeftUp(_IN CEventMouse& rEvent)
{
	SCFGraphics::Rect4i rt = this->RectWindow(); rt.iX = 0; rt.iY = 0;

	m_bActive = FALSE;
	if (PointInRect(rEvent.Position(), rt)) { ActionClick(); }

	CButton::OnMouseLeftUp(rEvent);
}

void CButtonClick::OnMouseMove(_IN CEventMouse& rEvent)
{
	CButton::OnMouseMove(rEvent);

	SCFGraphics::Rect4i rt = this->RectWindow(); rt.iX = 0; rt.iY = 0;

	if (!this->MouseCapture() || !rEvent.ButtonLeft()) { return; }

	if (PointInRect(rEvent.Position(), rt)) 
	{
		if (!m_bActive)
		{
			m_bActive = TRUE;
			this->Redraw(TRUE);
		}	
	}
	else 
	{
		if (m_bActive)
		{
			m_bActive = FALSE;
			this->Redraw(TRUE);
		}
	}
} 

void CButtonClick::AutoRect(_OUT SCFGraphics::Rect4i& rOutRect)
{
	Graphics graphics(this->WindowHandle());
	RectF rt;
	PointF pt(0, 0);

	graphics.MeasureString(this->Label(), -1, CPaint::SysFonts.pArial_11px, pt, &rt);

	rOutRect.iWidth = (int)rt.Width + 7;
	rOutRect.iHeight = (int)rt.Height + 7;
}

void CButtonClick::ActionClick() 
{
	CAction ClickAction(ActionClicked, this);
	ActionPerform(ClickAction);
}

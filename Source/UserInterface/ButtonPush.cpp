#include "ButtonPush.h"
#include "Paint.h"
#include "Event.h"

using namespace SCFUI;

CButtonPush::CButtonPush(_INOUT CWindowAbstract* pParent)
{
	m_Colors.pPushed = CPaint::SysColors.pLightFace;
	m_Colors.pRaised = CPaint::SysColors.pLightFace;

	this->WindowCreate(Win32ClassCButton, WS_CHILD, WS_EX_TRANSPARENT, pParent);
}

CButtonPush::~CButtonPush()
{
}

void CButtonPush::OnWindowPaint(_IN CEventWindow& rEvent)
{
	//Prepare variables
	SCFGraphics::Rect4i rt = this->RectWindow(); rt.iX = 0; rt.iY = 0;
	RectF rect((float)rt.iX, (float)rt.iY, (float)rt.iWidth, (float)rt.iHeight);

	//Prepare drawing canvas
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
		CPaint::RectResize(rt, - 1);
		if (m_bActive) { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pLightShadow_3px, CPaint::SysPens.pLightShadow_3px, NULL); }
		else           { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pOrange_3px, CPaint::SysPens.pOrange_3px, NULL); }
		CPaint::RectResize(rt, 1);
	}

	//Draw static raised/lowered edge
	if (m_bActive) { CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pDarkShadow, CPaint::SysPens.pLight, NULL); }
	else 
	{ 
		if (this->Border())
		{
			CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel, CPaint::SysPens.pLight, CPaint::SysPens.pDarkShadow, NULL);
		}
	}

	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);

	//Draw label
	if (m_bActive) { rect.X++; rect.Y++; }

	if (this->Label())
	{
		rEvent.Canvas()->DrawString(this->Label(), -1, CPaint::SysFonts.pArial_11px, rect, &format, CPaint::SysBrushes.pBlack);
	}

	//Draw focused dotted outline
	if (this->FocusFrame() && this->Focused())
	{
		CPaint::RectResize(rt, -3);

		rEvent.Canvas()->SetSmoothingMode(SmoothingModeNone);
		CPaint::Draw3DRectangle(rEvent.Canvas(), rt, m_uiBevel - 1, CPaint::SysPens.pDottedGray, CPaint::SysPens.pDottedGray, NULL);
		rEvent.Canvas()->SetSmoothingMode(SmoothingModeHighQuality);
	}
}

void CButtonPush::AutoRect(_OUT SCFGraphics::Rect4i& rOutRect)
{
	Graphics graphics(this->WindowHandle());
	RectF rt;
	PointF pt(0, 0);

	graphics.MeasureString(this->Label(), -1, CPaint::SysFonts.pArial_11px, pt, &rt);

	rOutRect.iWidth = (int)rt.Width + 7;
	rOutRect.iHeight = (int)rt.Height + 7;
}

void CButtonPush::ColorRaised(_IN SCFGraphics::ColorRGBA color) { m_Colors.pRaised = new Color(SCF_RGBA_TO_ARGB(color)); }
void CButtonPush::ColorPushed(_IN SCFGraphics::ColorRGBA color) { m_Colors.pPushed = new Color(SCF_RGBA_TO_ARGB(color)); }

SCFGraphics::ColorRGBA CButtonPush::ColorRaised()
{
	return SCF_RGBA(m_Colors.pRaised->GetR(), m_Colors.pRaised->GetG(), m_Colors.pRaised->GetB(), m_Colors.pRaised->GetA());
}
SCFGraphics::ColorRGBA CButtonPush::ColorPushed()
{
	return SCF_RGBA(m_Colors.pPushed->GetR(), m_Colors.pPushed->GetG(), m_Colors.pPushed->GetB(), m_Colors.pPushed->GetA());
}
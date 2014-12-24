#include "Spinner.h"
#include "Paint.h"

using namespace SCFUI;

bool Spinner_bInitDone = FALSE;

CSpinner::CSpinner(_INOUT CWindowAbstract* pParent)
{
	m_Value.fValue = 100;

	m_Value.fStep     = 0.005f;
	m_Value.bRelative = TRUE;

	m_Value.uiDigits = 2;

	m_DragState.bDragging = FALSE;
 
	m_pButtonPositive = NULL;
	m_pButtonNegative = NULL;

	if (!Spinner_bInitDone)
	{
		RegisterWin32Class();
		Spinner_bInitDone = TRUE;
	}

	this->WindowCreate(Win32ClassCSpinner, WS_CHILD, WS_EX_TRANSPARENT, pParent);

	m_pButtonPositive = new CButtonPush(this);
	m_pButtonNegative = new CButtonPush(this);

	m_pButtonPositive->Visible(TRUE);
	m_pButtonNegative->Visible(TRUE);

	m_pButtonPositive->EventListenerAdd(this);
	m_pButtonNegative->EventListenerAdd(this);

	m_pButtonPositive->FocusFrame(FALSE);
	m_pButtonNegative->FocusFrame(FALSE);

	m_pButtonPositive->Bevel(0);
	m_pButtonNegative->Bevel(0);
}

CSpinner::~CSpinner()
{
}

bool CSpinner::RegisterWin32Class()
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
	l_WndClassEx.lpszClassName	= Win32ClassCSpinner;
	l_WndClassEx.hIconSm		= NULL;

	if (!RegisterClassEx(&l_WndClassEx)) { return FALSE; }

	return TRUE;
}

bool CSpinner::OnWindowPaintBackground(_IN CEventWindow& rEvent)
{
	UNREFERENCED_PARAMETER(rEvent);
	return TRUE;
}

void CSpinner::UpdateBuddy()
{
/*	if (!m_pBuddy) { return; }

	CString BuddyText[128];

	crlStrCpy    (s_Format, TEXT("%."));
	crlPrintToStr(s_Buffer, 128, TEXT("%i"), m_Value.uiDigits);
	crlStrCat    (s_Format, s_Buffer);
	crlStrCat    (s_Format, TEXT("f"));

	crlPrintToStr(s_Buffer, 128, s_Format, m_Value.fValue);

	m_pBuddy->Label(s_Buffer);*/
}	  

void CSpinner::OnTimer(_IN UINT uiID)
{
	if (uiID == 1) { this->TimerStart(2, 50); }
	  
	if (m_pButtonPositive->Active()) { this->Value(this->Value() + __abs(this->StepValue())); }
	if (m_pButtonNegative->Active()) { this->Value(this->Value() - __abs(this->StepValue())); }
}

float CSpinner::StepValue()
{
	if (m_Value.bRelative) { return m_Value.fStep * m_DragState.fValueStart; }
	else                   { return m_Value.fStep; }
}

bool CSpinner::OnEvent(_IN CEvent& rEvent)
{
	switch (rEvent.ID())
	{
	case EventWindowPaint:
		{
			CButtonPush* pButton = (CButtonPush*)&(rEvent.Source());

			pButton->OnWindowPaint((CEventWindow&)rEvent);

			SCFGraphics::Rect4i rt = rEvent.Source().RectWindow();
			if (pButton->Active()) { rt.iX = 1; rt.iY = 1; }  else { rt.iX = 0; rt.iY = 0; }

			static Point point[4];

			if (pButton == m_pButtonPositive)
			{
				point[0].X = rt.iX + (int)(rt.iWidth  * 0.25);
				point[0].Y = rt.iY + (int)(rt.iHeight * 0.7);

				point[1].X = rt.iX + (int)(rt.iWidth  * 0.5);
				point[1].Y = rt.iY + (int)(rt.iHeight * 0.3);

				point[2].X = rt.iX + (int)(rt.iWidth  * 0.75);
				point[2].Y = rt.iY + (int)(rt.iHeight * 0.7);

				point[3].X = rt.iX + (int)(rt.iWidth  * 0.25);
				point[3].Y = rt.iY + (int)(rt.iHeight * 0.7);
			}
			else
			{
				point[0].X = rt.iX + (int)(rt.iWidth  * 0.25);
				point[0].Y = rt.iY + (int)(rt.iHeight * 0.3);

				point[1].X = rt.iX + (int)(rt.iWidth  * 0.75);
				point[1].Y = rt.iY + (int)(rt.iHeight * 0.3);

				point[2].X = rt.iX + (int)(rt.iWidth  * 0.5);
				point[2].Y = rt.iY + (int)(rt.iHeight * 0.7);

				point[3].X = rt.iX + (int)(rt.iWidth  * 0.25);
				point[3].Y = rt.iY + (int)(rt.iHeight * 0.3);
			}

			((CEventWindow&)rEvent).Canvas()->FillPolygon(CPaint::SysBrushes.pBlack, point, 3, FillModeWinding);
			((CEventWindow&)rEvent).Canvas()->DrawLines(CPaint::SysPens.pDarkShadow, point, 2);
			((CEventWindow&)rEvent).Canvas()->DrawLines(CPaint::SysPens.pBlack,     &point[2], 1);
			return FALSE;
		}

	case EventMouseLeftDown:
		{
			m_DragState.fValueStart   = m_Value.fValue;
			m_DragState.MouseStartPos = ((CEventMouse&)rEvent).Position();

			CButtonPush* pButton = (CButtonPush*)&(rEvent.Source());

			pButton->MouseCapture(TRUE);
			pButton->Focused(TRUE);
	
			pButton->ActionActivate();
			pButton->Redraw(TRUE);

			this->TimerStart(1, 250);
			return FALSE;
		}
	case EventMouseLeftUp:
		{
			CButtonPush* pButton = (CButtonPush*)&(rEvent.Source());

 			if (!m_DragState.bDragging)
			{
				if (m_pButtonPositive->Active()) { this->Value(this->Value() + __abs(this->StepValue())); }
				if (m_pButtonNegative->Active()) { this->Value(this->Value() - __abs(this->StepValue())); }
				this->TimerStop(1);
				this->TimerStop(2);
			}	

			pButton->MouseCapture(FALSE);
			pButton->ActionDeactivate();

			m_pButtonPositive->Active(FALSE);
			m_pButtonPositive->Redraw(TRUE);
			m_pButtonNegative->Active(FALSE);
			m_pButtonNegative->Redraw(TRUE);
		
			m_DragState.bDragging = FALSE;
			return FALSE;
		}
	case EventMouseMove:
		{
			this->TimerStop(1);
			this->TimerStop(2);

			CButtonPush* pButton = (CButtonPush*)&(rEvent.Source());
			
			if (pButton->MouseCapture())
			{
				if (!m_DragState.bDragging)
				{
					m_DragState.fValueStart = m_Value.fValue;
					m_DragState.bDragging   = TRUE;

					m_pButtonPositive->Active(TRUE);
					m_pButtonPositive->Redraw(TRUE);
					m_pButtonNegative->Active(TRUE);
					m_pButtonNegative->Redraw(TRUE);
				}
				else
				{
					float fScale = (float)(m_DragState.MouseStartPos.iY - ((CEventMouse&)rEvent).Position().iY);

					this->Value(m_DragState.fValueStart + __abs(this->StepValue()) * fScale);
				}
			}
 			return FALSE;
		}
	}

	//This tells that all unprocessed events should be processed further
	return TRUE;
}

void CSpinner::OnWindowSize(_IN CEventWindow& rEvent)
{
	SCFGraphics::Rect4i rect = rEvent.RectWindow();
	rect.iX = 0;
	rect.iY = 0;
	rect.iHeight /= 2;

	if (m_pButtonPositive) 
	{
		m_pButtonPositive->RectWindow(rect); 
	}

	rect.iY += rEvent.RectWindow().iHeight - rect.iHeight;	

	if (m_pButtonNegative) { m_pButtonNegative->RectWindow(rect); }
}

bool CSpinner::OnWindowSizing(_IN CEventWindow& rEvent)
{
	SCFGraphics::Rect4i rect = rEvent.RectWindow();
	rect.iX = 0;
	rect.iY = 0;
	rect.iHeight /= 2;

	if (m_pButtonPositive) { m_pButtonPositive->RectWindow(rect); }

	rect.iY += rEvent.RectWindow().iHeight - rect.iHeight;	

	if (m_pButtonNegative) { m_pButtonNegative->RectWindow(rect); }

	return TRUE;
}

void CSpinner::OnWindowPaint(_IN CEventWindow& rEvent)
{
	UNREFERENCED_PARAMETER(rEvent);
	//The 2 sub-buttons draw themselves -> no work needs to be done here
} 
#include "EventWindow.h"

using namespace SCFUI;

CEventWindow::CEventWindow(_IN ULONG uiID, _INOUT CWindowAbstract* pSource)
{
	m_uiID    = uiID;
	m_pSource = pSource;

	m_pCanvas = NULL;
}

CEventWindow::~CEventWindow()
{
	if (m_pCanvas) { delete m_pCanvas; }
}

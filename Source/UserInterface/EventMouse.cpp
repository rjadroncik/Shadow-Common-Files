#include "EventMouse.h"

using namespace SCFUI;

CEventMouse::CEventMouse(_IN ULONG uiID, _INOUT CWindowAbstract* pSource)
{
	m_uiID    = uiID;
	m_pSource = pSource;

	m_iDelta  = 0;
	m_dwFlags = 0;

	m_Position.iX = INT_MIN;
	m_Position.iY = INT_MIN;
}

CEventMouse::~CEventMouse()
{
} 
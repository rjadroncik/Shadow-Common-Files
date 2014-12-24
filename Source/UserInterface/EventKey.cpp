#include "EventKey.h"

using namespace SCFUI;

CEventKey::CEventKey(_IN ULONG uiID, _INOUT CWindowAbstract* pSource)
{
	m_uiID    = uiID;
	m_pSource = pSource;

	m_uiKeyCode     = 0;
	m_uiRepeatCount = 0;
}

CEventKey::~CEventKey()
{
}

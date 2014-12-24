#include "Action.h"

using namespace SCFUI;

CAction::CAction(_IN ULONG uiID, _INOUT CWindowAbstract* pSource)
{
	m_uiID    = uiID;
	m_pSource = pSource;
}

CAction::~CAction()
{
}

LPTSTR CAction::IDText() _GET
{
	switch (m_uiID)
	{
		//Button actions
		case ActionClicked:		{ return TEXT("ActionClicked"); }
		case ActionActivated:	{ return TEXT("ActionActivated"); }
		case ActionDeactivated:	{ return TEXT("ActionDeactivated"); }

		//Window actions
		case ActionClosed:		{ return TEXT("ActionClosed"); }
	}

	return NULL;
}

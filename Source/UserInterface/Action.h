#pragma once

#include "Message.h"

namespace SCFUI
{
	class SCFUI_API CAction : public CMessage
	{
	public:

		CAction(_IN ULONG uiID, _INOUT CWindowAbstract* pSource);
		virtual ~CAction();

		LPTSTR IDText() _GET;
	};
};
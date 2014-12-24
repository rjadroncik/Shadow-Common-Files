#pragma once

#include "Definitions.h"

namespace SCFUI
{
	class CWindowAbstract;

	class SCFUI_API CMessage
	{
	public:

		CWindowAbstract& Source() _GET { return *m_pSource; }
		
		ULONG          ID()     _GET { return m_uiID; }
		virtual LPTSTR IDText() _GET = 0;

	protected:
		CMessage();
		virtual ~CMessage();

	protected:
		ULONG				m_uiID;
		CWindowAbstract*	m_pSource;
	};
};
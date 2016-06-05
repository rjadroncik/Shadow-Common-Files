#pragma once

#include "EnumeratorRaw.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorListRaw : public CEnumeratorRaw
	{
		friend class OBJECT_EXTENSIONS_API CListRaw;

	public:
		CString ToString() _GET { return STRING("{EnumeratorList}"); }

	public:
		CEnumeratorListRaw(_IN CListRaw& rList);
		virtual ~CEnumeratorListRaw();

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd();

	protected:
		UINT m_uiIndex;
	};
};
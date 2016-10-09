#pragma once

#include "EnumeratorRaw.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorVectorRaw : public CEnumeratorRaw
	{
		friend class OBJECT_EXTENSIONS_API CVectorRaw;

	public:
		CString ToString() _GET { return STRING("{EnumeratorVector}"); }

	public:
		CEnumeratorVectorRaw(_IN CVectorRaw& rVector);
		virtual ~CEnumeratorVectorRaw();

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd();

	protected:
		UINT m_uiIndex;
	};
};
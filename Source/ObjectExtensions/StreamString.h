#pragma once
#include "Stream.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamString : public CStream
	{
	public:
		CStreamString(_INOUT CString& rString);
		virtual ~CStreamString();

	protected:
		CString* m_pString;
		UINT m_uiOffset;
	};
};
#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CArray : public CValue
	{
	public:
		virtual ~CArray();
		
	public:
		CValue& At         (_IN UINT uiIndex) _GET;
		CValue& operator [](_IN UINT uiIndex) _GET;
	
	public:
		virtual void AtPut(_IN UINT uiIndex, _IN CValue& rValue) _SET = 0;

	public:
		CString ToString() _GET;
	
	public:
		inline UINT Size() _GET { return m_uiCount; }

	protected:
		void LastAdd(_IN _REF CValue& rValue);

	protected:
		CArray() { m_ppValues = nullptr; m_uiCount = 0; }

	protected:
		CValue** m_ppValues;
		UINT m_uiCount;
	};
};

#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CArray : public CValue
	{
	public:
		virtual ~CArray();

	public:
		CValue& At         (_IN SCF::UINT uiIndex) _GET;
		CValue& operator [](_IN SCF::UINT uiIndex) _GET;
	
	public:
		virtual void AtPut(_IN SCF::UINT uiIndex, _IN CValue& rValue) _SET = 0;

	public:
		CString ToString() _GET;
	
	public:
		inline SCF::UINT Size() _GET { return m_uiCount; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);
		
		void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const;
		void DependentsDeserialize(_INOUT IStreamReadObject&  rStream);

	protected:
		void LastAdd(_IN _REF CValue& rValue);

	protected:
		CArray() { m_ppValues = NULL; m_uiCount = 0; }

	protected:
		CValue** m_ppValues;
		SCF::UINT m_uiCount;
	};
};

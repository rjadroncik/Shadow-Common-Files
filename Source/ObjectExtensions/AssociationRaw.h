#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CAssociationRaw : public CObject
	{
	public:
		CString ToString() _GET;

	public:
		CAssociationRaw(_IN CAssociationRaw& rAssociation);

		CAssociationRaw(_IN _REF CObject& rKey, _IN _REF CObject& rValue);
		virtual ~CAssociationRaw();

	public:
		CObject& Key()   _GET { return *m_pKey; }
		CObject& Value() _GET { return *m_pValue; }

		void Key(_IN _REF CObject& rKey)   _SET { RELEASE(*m_pKey);   m_pKey = (CObject*)&rKey;   ADDREF(*m_pKey); }
		void Value(_IN _REF CObject& rValue) _SET { RELEASE(*m_pValue); m_pValue = (CObject*)&rValue; ADDREF(*m_pValue); }

	public:
		void DeleteWithObjects();

	protected:
		CAssociationRaw() {}

	private:
		CObject* m_pKey;
		CObject* m_pValue;
	};
};

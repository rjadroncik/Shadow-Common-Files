#pragma once
#include "Value.h"

namespace SCFPrivate
{
	//This is a private default constructor used for deserialization of associations
	SCFBase::CObjectSerializable* NewAssociation();
};

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CAssociation : public CObjectSerializable
	{
		//This is a private default constructor used for deserialization of associations
		friend CObjectSerializable* SCFPrivate::NewAssociation();

	public:
		SCF::ENUM ClassKey() _GET { return ClassAssociation; }
		CString   ToString() _GET;

	public:
		CAssociation(_IN CAssociation& rAssociation);

		CAssociation(_IN _REF CObject& rKey, _IN _REF CObject& rValue);
		virtual ~CAssociation();

		CObject& Key()   _GET { return *m_pKey; }
		CObject& Value() _GET { return *m_pValue; }
		
		void Key  (_IN _REF CObject& rKey)   _SET { RELEASE(*m_pKey);   m_pKey   = (CObject*)&rKey;   ADDREF(*m_pKey); }
		void Value(_IN _REF CObject& rValue) _SET { RELEASE(*m_pValue); m_pValue = (CObject*)&rValue; ADDREF(*m_pValue); }

	public:
		void DeleteWithObjects();

	public:
		void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const;
		void DependentsDeserialize(_INOUT IStreamReadObject&  rStream);
 
	protected:
		CAssociation() {}

	private:
		CObject* m_pKey;
		CObject* m_pValue;
	};
};

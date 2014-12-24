#include "Association.h"
#include "String.h"

using namespace SCFBase;


CAssociation::CAssociation(_IN CAssociation& rAssociation)
{
	m_pKey   = (CObject*)&rAssociation.m_pKey;
	m_pValue = (CObject*)&rAssociation.m_pValue;

	ADDREF(*(m_pKey));
	ADDREF(*(m_pValue));
}

CAssociation::CAssociation(_IN _REF CObject& rKey, _IN _REF CObject& rValue)
{
	m_pKey   = (CObject*)&rKey;
	m_pValue = (CObject*)&rValue;

	ADDREF(*(m_pKey));
	ADDREF(*(m_pValue));
}

CAssociation::~CAssociation()
{
	RELEASE(*(m_pKey));
	RELEASE(*(m_pValue));
}

CString CAssociation::ToString() _GET
{
	if (m_pValue) { return CString((m_pKey->ToString() + STRING(" -> ")), m_pValue->ToString()); }

	return m_pKey->ToString();
}

void CAssociation::DeleteWithObjects()
{
	CObject* pKey   = m_pKey;
	CObject* pValue = m_pValue;

	delete this;

	delete pKey;
	delete pValue;
}

void CAssociation::DependentsSerialize(_INOUT IStreamWriteObject& rStream) const
{
	rStream.Next(*(CObjectSerializable*)m_pKey);
	rStream.Next(*(CObjectSerializable*)m_pValue);
}

void CAssociation::DependentsDeserialize(_INOUT IStreamReadObject& rStream)
{
	rStream.Next();
	m_pKey = rStream.Current();
	ADDREF(*(m_pKey));

	rStream.Next();
	m_pValue = rStream.Current();
	ADDREF(*(m_pValue));
}
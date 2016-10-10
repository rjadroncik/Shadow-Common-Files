#include "AssociationRaw.h"
#include "String.h"

using namespace SCFBase;

CAssociationRaw::CAssociationRaw(_IN CAssociationRaw& rAssociation)
{
	m_pKey   = (CObject*)&rAssociation.m_pKey;
	m_pValue = (CObject*)&rAssociation.m_pValue;

	ADDREF(*(m_pKey));
	ADDREF(*(m_pValue));
}

CAssociationRaw::CAssociationRaw(_IN _REF CObject& rKey, _IN _REF CObject& rValue)
{
	m_pKey   = (CObject*)&rKey;
	m_pValue = (CObject*)&rValue;

	ADDREF(*(m_pKey));
	ADDREF(*(m_pValue));
}

CAssociationRaw::~CAssociationRaw()
{
	RELEASE(*(m_pKey));
	RELEASE(*(m_pValue));
}

CString CAssociationRaw::ToString() _GET
{
	if (m_pValue) { return CString((m_pKey->ToString() + STRING(" -> ")), m_pValue->ToString()); }

	return m_pKey->ToString();
}

void CAssociationRaw::DeleteWithObjects()
{
	CObject* pKey   = m_pKey;
	CObject* pValue = m_pValue;

	delete this;

	delete pKey;
	delete pValue;
}
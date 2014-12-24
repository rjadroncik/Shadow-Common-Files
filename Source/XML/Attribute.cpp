#include "Attribute.h"
#include "Writer.h"

using namespace SCF;
using namespace SCFXML;

CXMLAttribute::CXMLAttribute()
{
}

CXMLAttribute::~CXMLAttribute()
{
}

CString CXMLAttribute::ToString() _GET
{
	CString              Result;
	CStreamStringWrite   ResultStreamWrite(Result);
	CStreamWriteTextUCS2 ResultStreamWriteText(ResultStreamWrite, FALSE);

	CXMLWriter(ResultStreamWriteText).WriteAttribute(*this);

	return Result;
}

bool CXMLAttribute::operator ==(_IN CXMLAttribute& rAttribute)
{
	if (m_pValue->IsSameTypeAs(*(rAttribute.m_pValue))) { return m_pValue->IsEqualTo(*(rAttribute.m_pValue)); }

	if (m_pValue->ClassKey() == ClassString)
	{
		return ((*(CString*)m_pValue) == rAttribute.m_pValue->ToString());
	}
	if (rAttribute.m_pValue->ClassKey() == ClassString)
	{
		return ((*(CString*)(rAttribute.m_pValue)) == m_pValue->ToString());
	}

	return FALSE;
}
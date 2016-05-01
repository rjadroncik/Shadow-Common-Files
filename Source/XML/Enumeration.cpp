#include "Enumeration.h"

using namespace SCFBase;
using namespace SCFXML;

CDictionaryString<CXMLEnumeration> Enumeration_EnumerationsByName;

void CXMLEnumeration::Register(_IN CString& rName, _IN CXMLEnumeration& rEnumeration)
{
	Enumeration_EnumerationsByName.AtPut(rName, rEnumeration);
}

CXMLEnumeration* CXMLEnumeration::ByName(_IN CString& rName)
{
	return Enumeration_EnumerationsByName.At(rName);
}

CEnum* CXMLEnumeration::Translate(_IN CString& rEnumeration, _IN CString& rValue)
{
	CXMLEnumeration* pEnum = Enumeration_EnumerationsByName.At(rEnumeration);
	if (pEnum)
	{
		return pEnum->ValueOf(rValue);
	}

	return NULL;
}

CString* CXMLEnumeration::Translate(_IN CString& rEnumeration, _IN ENUM eValue)
{
	CXMLEnumeration* pEnum = Enumeration_EnumerationsByName.At(rEnumeration);
	if (pEnum)
	{
		return pEnum->ValueName(eValue);
	}

	return NULL;
}

CXMLEnumeration::CXMLEnumeration()
{
}

CXMLEnumeration::~CXMLEnumeration()
{
	m_Values.AllDelete();
	m_Names.AllDelete();
}

void CXMLEnumeration::ValueDefine(_IN CString& rName, _IN ENUM eValue) _SET
{
	m_Values.AtPut(rName, *(new CEnum(eValue)));
	m_Names.AtPut(eValue, *(new CString(rName)));
}

CEnum* CXMLEnumeration::ValueOf(_IN CString& rName) _GET
{
	return (CEnum*)m_Values.At(rName);
}

CString* CXMLEnumeration::ValueName(_IN ENUM eValue) _GET
{
	return (CString*)m_Names.At(eValue);
}

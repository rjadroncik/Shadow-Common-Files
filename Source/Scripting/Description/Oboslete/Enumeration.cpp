#include "Enumeration.h"

using namespace SCFBase;
using namespace SCFScripting;

CDictionaryString Enumeration_EnumerationsByName;

void CEnumeration::Register(_IN CString& rName, _IN CEnumeration& rEnumeration)
{
	Enumeration_EnumerationsByName.AtPut(rName, rEnumeration);
}

CEnumeration* CEnumeration::ByName(_IN CString& rName)
{
	return (CEnumeration*)Enumeration_EnumerationsByName.At(rName);
}

CEnum* CEnumeration::Translate(_IN CString& rEnumeration, _IN CString& rValue)
{
	CEnumeration* pEnum = (CEnumeration*)Enumeration_EnumerationsByName.At(rEnumeration);
	if (pEnum)
	{
		return pEnum->Value(rValue);
	}

	return NULL;
}

CString* CEnumeration::Translate(_IN CString& rEnumeration, _IN SCF::ENUM eValue)
{
	CEnumeration* pEnum = (CEnumeration*)Enumeration_EnumerationsByName.At(rEnumeration);
	if (pEnum)
	{
		return pEnum->ValueName(eValue);
	}

	return NULL;
}

CEnumeration::CEnumeration()
{
}

CEnumeration::~CEnumeration()
{
	m_Values.AllDelete();
	m_Names.AllDelete();
}

void CEnumeration::ValueDefine(_IN CString& rName, _IN SCF::ENUM eValue) _SET
{
	m_Values.AtPut(rName, *(new CEnum(eValue)));
	m_Names.AtPut(eValue, *(new CString(rName)));
}

CEnum* CEnumeration::Value(_IN CString& rName) _GET
{
	return (CEnum*)m_Values.At(rName);
}

CString* CEnumeration::ValueName(_IN SCF::ENUM eValue) _GET
{
	return (CString*)m_Names.At(eValue);
}

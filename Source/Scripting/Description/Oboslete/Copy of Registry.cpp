#include "Registry.h"

using namespace SCFObjectExtensions;
using namespace SCFScripting;

CDictionaryString CRegistry::s_Enumerations;

CDictionaryInt64  CRegistry::s_Classes;

CDictionaryString CRegistry::s_IDsClasses;
CDictionaryString CRegistry::s_IDsMethods;
CDictionaryString CRegistry::s_IDsProperties;

CDictionaryInt64 CRegistry::s_NamesClasses;
CDictionaryInt64 CRegistry::s_NamesMethods;
CDictionaryInt64 CRegistry::s_NamesProperties;

SCF::UINT CRegistry::s_uiNextIDMethod   = 1;
SCF::UINT CRegistry::s_uiNextIDProperty = 1;

void CRegistry::EnumerationRegister(_IN CString& rName, _IN CEnumeration& rEnumeration)
{
	s_Enumerations.AtPut(rName, rEnumeration);
}

CEnumeration* CRegistry::Enumeration(_IN CString& rName)
{
	return (CEnumeration*)s_Enumerations.NameToObject(rName);
}

CEnum* CRegistry::EnumerationTranslate(_IN CString& rEnumeration, _IN CString& rValue)
{
	CEnumeration* pEnum = (CEnumeration*)s_Enumerations.NameToObject(rEnumeration);
	if (pEnum)
	{
		return pEnum->Value(rValue);
	}

	return NULL;
}

CString* CRegistry::EnumerationTranslate(_IN CString& rEnumeration, _IN SCF::ENUM eValue)
{
	CEnumeration* pEnum = (CEnumeration*)s_Enumerations.NameToObject(rEnumeration);
	if (pEnum)
	{
		return pEnum->ValueName(eValue);
	}

	return NULL;
}

void CRegistry::ClassRegister(_IN CString& rName, _IN SCF::ENUM eClassKey, _IN CClass& rClass)
{
	s_Classes.AtPut(eClassKey, rClass);
	MapClass(rName, eClassKey);

	CEnumeratorDictionaryString EnumeratorMethods(rClass.Methods());
	while (EnumeratorMethods.Next()) { MapMethod(EnumeratorMethods.CurrentPath()); }

	CEnumeratorDictionaryString EnumeratorProperties(rClass.Properties());
	while (EnumeratorProperties.Next()) { MapProperty(EnumeratorProperties.CurrentPath()); }
}

void CRegistry::MapClass(_IN CString& rName, _IN SCF::ENUM eClassKey)
{
	if (!s_IDsClasses.ContainsName(rName))
	{
		s_IDsClasses.AtPut(rName, *(new CInt(eClassKey)));
		s_NamesClasses.AtPut(eClassKey, *(new CString(rName)));
	}
}

void CRegistry::MapMethod(_IN CString& rName)
{
	if (!s_IDsMethods.ContainsName(rName))
	{
		s_IDsMethods.AtPut(rName, *(new CInt(s_uiNextIDMethod)));
		s_NamesMethods.AtPut(s_uiNextIDMethod, *(new CString(rName)));
		s_uiNextIDMethod++;
	}
}

void CRegistry::MapProperty(_IN CString& rName)
{
	if (!s_IDsProperties.ContainsName(rName))
	{
		s_IDsProperties.AtPut(rName, *(new CInt(s_uiNextIDProperty)));
		s_NamesProperties.AtPut(s_uiNextIDProperty, *(new CString(rName)));
		s_uiNextIDProperty++;
	}
}

SCF::UINT CRegistry::IDOfClass(_IN CString& rName)
{
	CInt* pID = (CInt*)s_IDsClasses.NameToObject(rName);

	if (pID) { return pID->Value(); }
	else     { return 0; }
}

SCF::UINT CRegistry::IDOfMethod(_IN CString& rName)
{
	CInt* pID = (CInt*)s_IDsMethods.NameToObject(rName);

	if (pID) { return pID->Value(); }
	else     { return 0; }
}

SCF::UINT CRegistry::IDOfProperty(_IN CString& rName)
{
	CInt* pID = (CInt*)s_IDsProperties.NameToObject(rName);

	if (pID) { return pID->Value(); }
	else     { return 0; }
}

//These methods query class, method & property names
CString CRegistry::NameOfClass(_IN SCF::UINT uiID)
{
	CString* pName = (CString*)s_NamesClasses.KeyToObject(uiID);

	if (pName) { return *pName; }
	else       { return CString(); }
}

CString CRegistry::NameOfMethod(_IN SCF::UINT uiID)
{
	CString* pName = (CString*)s_NamesMethods.KeyToObject(uiID);

	if (pName) { return *pName; }
	else       { return CString(); }
}

CString CRegistry::NameOfProperty(_IN SCF::UINT uiID)
{
	CString* pName = (CString*)s_NamesProperties.KeyToObject(uiID);

	if (pName) { return *pName; }
	else       { return CString(); }
}

CClass* CRegistry::Class(_IN CString& rName) 
{ 
	CInt* pClassKey = (CInt*)s_IDsClasses.NameToObject(rName);
	
	if (pClassKey)
	{
		return (CClass*)s_Classes.KeyToObject(pClassKey->Value()); 
	}
	else { return NULL; } 
}

CClass* CRegistry::Class(_IN SCF::ENUM eClassKey)
{
	return (CClass*)s_Classes.KeyToObject(eClassKey); 
}

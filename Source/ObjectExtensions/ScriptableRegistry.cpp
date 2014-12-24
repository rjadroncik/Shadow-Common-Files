#include "StdAfx.h"
#include "ScriptableRegistry.h"

#include "Int.h"
#include "EnumeratorDictionaryString.h"

using namespace SCFObjectExtensions;

CDictionaryString CScriptableRegistry::s_Enumerations;

CDictionaryInt64  CScriptableRegistry::s_Classes;

CDictionaryString CScriptableRegistry::s_IDsClasses;
CDictionaryString CScriptableRegistry::s_IDsMethods;
CDictionaryString CScriptableRegistry::s_IDsProperties;

CDictionaryInt64 CScriptableRegistry::s_NamesClasses;
CDictionaryInt64 CScriptableRegistry::s_NamesMethods;
CDictionaryInt64 CScriptableRegistry::s_NamesProperties;

SCF::UINT CScriptableRegistry::s_uiNextIDMethod   = 1;
SCF::UINT CScriptableRegistry::s_uiNextIDProperty = 1;

void CScriptableRegistry::EnumerationRegister(_IN CString& rName, _IN CScriptableEnumeration& rEnumeration)
{
	s_Enumerations.AtPut(rName, rEnumeration);
}

CScriptableEnumeration* CScriptableRegistry::Enumeration(_IN CString& rName)
{
	return (CScriptableEnumeration*)s_Enumerations.NameToObject(rName);
}

CEnum* CScriptableRegistry::EnumerationTranslate(_IN CString& rEnumeration, _IN CString& rValue)
{
	CScriptableEnumeration* pEnum = (CScriptableEnumeration*)s_Enumerations.NameToObject(rEnumeration);
	if (pEnum)
	{
		return pEnum->Value(rValue);
	}

	return NULL;
}

CString* CScriptableRegistry::EnumerationTranslate(_IN CString& rEnumeration, _IN SCF::ENUM eValue)
{
	CScriptableEnumeration* pEnum = (CScriptableEnumeration*)s_Enumerations.NameToObject(rEnumeration);
	if (pEnum)
	{
		return pEnum->ValueName(eValue);
	}

	return NULL;
}

void CScriptableRegistry::ClassRegister(_IN CString& rName, _IN SCF::ENUM eClassKey, _IN CScriptableClass& rClass)
{
	s_Classes.AtPut(eClassKey, rClass);
	MapClass(rName, eClassKey);

	CEnumeratorDictionaryString EnumeratorMethods(rClass.Methods());
	while (EnumeratorMethods.Next()) { MapMethod(EnumeratorMethods.CurrentPath()); }

	CEnumeratorDictionaryString EnumeratorProperties(rClass.Properties());
	while (EnumeratorProperties.Next()) { MapProperty(EnumeratorProperties.CurrentPath()); }
}

void CScriptableRegistry::MapClass(_IN CString& rName, _IN SCF::ENUM eClassKey)
{
	if (!s_IDsClasses.ContainsName(rName))
	{
		s_IDsClasses.AtPut(rName, *(new CInt(eClassKey)));
		s_NamesClasses.AtPut(eClassKey, *(new CString(rName)));
	}
}

void CScriptableRegistry::MapMethod(_IN CString& rName)
{
	if (!s_IDsMethods.ContainsName(rName))
	{
		s_IDsMethods.AtPut(rName, *(new CInt(s_uiNextIDMethod)));
		s_NamesMethods.AtPut(s_uiNextIDMethod, *(new CString(rName)));
		s_uiNextIDMethod++;
	}
}

void CScriptableRegistry::MapProperty(_IN CString& rName)
{
	if (!s_IDsProperties.ContainsName(rName))
	{
		s_IDsProperties.AtPut(rName, *(new CInt(s_uiNextIDProperty)));
		s_NamesProperties.AtPut(s_uiNextIDProperty, *(new CString(rName)));
		s_uiNextIDProperty++;
	}
}

SCF::UINT CScriptableRegistry::IDOfClass(_IN CString& rName)
{
	CInt* pID = (CInt*)s_IDsClasses.NameToObject(rName);

	if (pID) { return pID->Value(); }
	else     { return 0; }
}

SCF::UINT CScriptableRegistry::IDOfMethod(_IN CString& rName)
{
	CInt* pID = (CInt*)s_IDsMethods.NameToObject(rName);

	if (pID) { return pID->Value(); }
	else     { return 0; }
}

SCF::UINT CScriptableRegistry::IDOfProperty(_IN CString& rName)
{
	CInt* pID = (CInt*)s_IDsProperties.NameToObject(rName);

	if (pID) { return pID->Value(); }
	else     { return 0; }
}

//These methods query class, method & property names
CString CScriptableRegistry::NameOfClass(_IN SCF::UINT uiID)
{
	CString* pName = (CString*)s_NamesClasses.KeyToObject(uiID);

	if (pName) { return *pName; }
	else       { return CString(); }
}

CString CScriptableRegistry::NameOfMethod(_IN SCF::UINT uiID)
{
	CString* pName = (CString*)s_NamesMethods.KeyToObject(uiID);

	if (pName) { return *pName; }
	else       { return CString(); }
}

CString CScriptableRegistry::NameOfProperty(_IN SCF::UINT uiID)
{
	CString* pName = (CString*)s_NamesProperties.KeyToObject(uiID);

	if (pName) { return *pName; }
	else       { return CString(); }
}

CScriptableClass* CScriptableRegistry::Class(_IN CString& rName) 
{ 
	CInt* pClassKey = (CInt*)s_IDsClasses.NameToObject(rName);
	
	if (pClassKey)
	{
		return (CScriptableClass*)s_Classes.KeyToObject(pClassKey->Value()); 
	}
	else { return NULL; } 
}

CScriptableClass* CScriptableRegistry::Class(_IN SCF::ENUM eClassKey)
{
	return (CScriptableClass*)s_Classes.KeyToObject(eClassKey); 
}

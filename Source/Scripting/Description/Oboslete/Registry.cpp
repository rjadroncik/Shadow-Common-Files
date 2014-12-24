#include "Registry.h"

using namespace SCFBase;
using namespace SCFScripting;

CDictionaryString s_Enumerations;

CDictionaryString s_ClassesByName;
CDictionaryInt64  s_ClassesByKey;

CDictionaryString s_ClassKeys;
CDictionaryInt64  s_ClassNames;

void CRegistry::EnumerationRegister(_IN CString& rName, _IN CEnumeration& rEnumeration)
{
	s_Enumerations.AtPut(rName, rEnumeration);
}

CEnumeration* CRegistry::Enumeration(_IN CString& rName)
{
	return (CEnumeration*)s_Enumerations.At(rName);
}

CEnum* CRegistry::EnumerationTranslate(_IN CString& rEnumeration, _IN CString& rValue)
{
	CEnumeration* pEnum = (CEnumeration*)s_Enumerations.At(rEnumeration);
	if (pEnum)
	{
		return pEnum->Value(rValue);
	}

	return NULL;
}

CString* CRegistry::EnumerationTranslate(_IN CString& rEnumeration, _IN SCF::ENUM eValue)
{
	CEnumeration* pEnum = (CEnumeration*)s_Enumerations.At(rEnumeration);
	if (pEnum)
	{
		return pEnum->ValueName(eValue);
	}

	return NULL;
}

void CRegistry::ClassRegister(_IN SCF::ENUM eClassKey, _IN CString& rFullClassName, _INOUT CClass& rClass)
{
	if (!s_ClassNames.ContainsKey(eClassKey) && !s_ClassKeys.ContainsName(rFullClassName))
	{
		CObject::ClassRegister(eClassKey, rFullClassName, rClass.ObjectSize());

		s_ClassKeys.AtPut (rFullClassName, *(new CInt(eClassKey)));
		s_ClassNames.AtPut(eClassKey,      *(new CString(rFullClassName)));

		s_ClassesByName.AtPut(rFullClassName, rClass);
		s_ClassesByKey.AtPut (eClassKey,      rClass);
	}
}

CClass* CRegistry::Class(_IN CString& rFullClassName) 
{ 
	return (CClass*)s_ClassesByName.At(rFullClassName); 
}

CClass* CRegistry::Class(_IN SCF::ENUM eClassKey)
{
	return (CClass*)s_ClassesByKey.At(eClassKey); 
}

SCF::ENUM CRegistry::ClassKey(_IN CString& rFullClassName)
{
	CInt* pID = (CInt*)s_ClassKeys.At(rFullClassName);

	if (pID) { return pID->Value(); }
	else     { return 0; }
}

CString CRegistry::ClassName(_IN SCF::ENUM eClassKey)
{
	CString* pName = (CString*)s_ClassNames.At(eClassKey);

	if (pName) { return *pName; }
	else       { return CString(); }
}
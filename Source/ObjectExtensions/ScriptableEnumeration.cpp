#include "StdAfx.h"
#include "ScriptableEnumeration.h"
#include "EnumeratorDictionaryString.h"

using namespace SCFObjectExtensions;

CScriptableEnumeration::CScriptableEnumeration()
{
}

CScriptableEnumeration::~CScriptableEnumeration()
{
	m_Values.AllDelete();
	m_Names.AllDelete();
}

void CScriptableEnumeration::ValueDefine(_IN CString& rName, _IN SCF::ENUM eValue) _SET
{
	m_Values.AtPut(rName, *(new CEnum(eValue)));
	m_Names.AtPut(eValue, *(new CString(rName)));
}

CEnum* CScriptableEnumeration::Value(_IN CString& rName) _GET
{
	return (CEnum*)m_Values.NameToObject(rName);
}

CString* CScriptableEnumeration::ValueName(_IN SCF::ENUM eValue) _GET
{
	return (CString*)m_Names.KeyToObject(eValue);
}

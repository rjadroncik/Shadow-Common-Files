#include "StdAfx.h"
#include "ScriptableMethod.h"
#include "Int.h"

using namespace SCFObjectExtensions;

CScriptableMethod::CScriptableMethod(_IN CScriptableArgument* pReturnValue)
{
	m_pReturnValue = pReturnValue;
}

CScriptableMethod::~CScriptableMethod()
{
	if (m_pReturnValue) { delete m_pReturnValue; }

	m_Arguments.AllDelete();
	m_ArgumentIndices.AllDelete();
}

void CScriptableMethod::ArgumentAdd(_IN CString& rName, _IN _REF CScriptableArgument& rArgument) _SET
{
	m_ArgumentIndices.AtPut(rName, *(new CInt(m_Arguments.Size())));

	m_Arguments.LastAdd(rArgument);
}

const CScriptableArgument* CScriptableMethod::ArgumentNamed(_IN CString& rName) _GET
{
	CInt* pIndex = (CInt*)m_ArgumentIndices.NameToObject(rName);
	if (pIndex)
	{
		return (CScriptableArgument*)&m_Arguments.At(pIndex->Value());
	}

	return NULL;
}


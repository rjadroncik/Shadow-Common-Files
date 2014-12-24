#include "StdAfx.h"
#include "ScriptableProperty.h"

using namespace SCFObjectExtensions;

CScriptableProperty::CScriptableProperty(_IN CString& rEnumeration, _IN bool bReference, _IN bool bSettable, _IN bool bImmediate, _IN bool bHidden)
{
	m_eClassKey = ClassEnum;
	m_pEnumeration = new CString(rEnumeration);

	m_bReference = bReference;
	m_bSettable  = bSettable;
	m_bImmediate = bImmediate;
	m_bHidden    = bHidden;
}

CScriptableProperty::CScriptableProperty(_IN SCF::ENUM eClassKey, _IN bool bReference, _IN bool bSettable, _IN bool bImmediate, _IN bool bHidden)
{
	m_eClassKey = eClassKey;
	m_pEnumeration = NULL;

	m_bReference = bReference;
	m_bSettable  = bSettable;
	m_bImmediate = bImmediate;
	m_bHidden    = bHidden;
}

CScriptableProperty::~CScriptableProperty()
{
	if (m_pEnumeration) { delete m_pEnumeration; }
}
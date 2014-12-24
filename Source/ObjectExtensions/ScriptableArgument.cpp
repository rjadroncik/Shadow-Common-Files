#include "StdAfx.h"
#include "ScriptableArgument.h"

using namespace SCFObjectExtensions;

CScriptableArgument::CScriptableArgument(_IN SCF::ENUM eClassKey, _IN bool bReference)
{
	m_eClassKey = eClassKey;
	m_pEnumeration = NULL;

	m_bReference = bReference;
}

CScriptableArgument::~CScriptableArgument()
{
	if (m_pEnumeration) { delete m_pEnumeration; }
}
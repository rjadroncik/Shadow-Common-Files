#include "StdAfx.h"
#include "ScriptableClass.h"
#include "Memory.h"

using namespace SCFObjectExtensions;

CScriptableClass::CScriptableClass(_IN SCF::ENUM eClassKey)
{
	m_eClassKey  = eClassKey;

	m_pConstructors = NULL;
	m_uiConstructorCount = 0;
}

CScriptableClass::~CScriptableClass()
{
}

void CScriptableClass::AddProperty(_IN CString& rName, _IN CScriptableProperty& rProperty)
{
	m_Properties.AtPut(rName, rProperty);
}

void CScriptableClass::ConstructorAdd(_IN CScriptableMethod& rConstructor, _IN ACCESSIBLE_OBJECT_CONSTRUCTOR constructor) _SET
{
	m_uiConstructorCount++;

	CMemory::Reallocate((void**)&m_pConstructors, sizeof(SConstructor) * m_uiConstructorCount);

	m_pConstructors[m_uiConstructorCount - 1].pDescriptor = &rConstructor;
	m_pConstructors[m_uiConstructorCount - 1].Function    = constructor;
}

const CScriptableMethod* CScriptableClass::Constructor(_IN SCF::UINT uiIndex) _GET { return m_pConstructors[uiIndex].pDescriptor; }

CScriptableObject* CScriptableClass::Construct(_IN SCF::UINT uiIndex, _IN CVector& rArguments) { return m_pConstructors[uiIndex].Function(rArguments); }

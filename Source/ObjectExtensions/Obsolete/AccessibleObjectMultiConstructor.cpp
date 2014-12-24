#include "StdAfx.h"
#include "AccessibleObjectMultiConstructor.h"
#include "Int.h"
#include "AccessibleRegistry.h"

CAccessibleObjectMultiConstructor::SProperties CAccessibleObjectMultiConstructor::s_Properties;

CAccessibleObjectMultiConstructor::CAccessibleObjectMultiConstructor(_IN UINT uiConstructorIndex)
{
	m_uiConstructorIndex = uiConstructorIndex;
}

CAccessibleObjectMultiConstructor::~CAccessibleObjectMultiConstructor()
{

}

void CAccessibleObjectMultiConstructor::PropertiesAdd(_OUT CAccessibleClass& rOutClass)
{
	CAccessibleProperty* pProperty;

	pProperty = new CAccessibleProperty(ClassInt, FALSE, FALSE, FALSE, TRUE);
	rOutClass.AddProperty(CString(STRING("ci")), *pProperty);
}

void CAccessibleObjectMultiConstructor::Register()
{
	CAccessibleClass* pClass = new CAccessibleClass(ClassAccessibleObjectMultiConstructor);
	{
		CAccessibleObjectMultiConstructor::PropertiesAdd(*pClass);
	}
	CAccessibleRegistry::ClassRegister(CString(STRING("AccessibleObjectMultiConstructor")), ClassAccessibleObjectMultiConstructor, *pClass);

	s_Properties.uiConstructorIndex = CAccessibleRegistry::IDOfProperty(CString(STRING("ci")));
}

CObject* CAccessibleObjectMultiConstructor::PropertyGet(_IN ENUM eName) _GET
{
	if (eName == s_Properties.uiConstructorIndex)
	{
		return new CInt(m_uiConstructorIndex);
	}

	return NULL;
}
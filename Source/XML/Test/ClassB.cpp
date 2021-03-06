#include "ClassA.h"

CClassB::CClassB()
{
	m_TestInt = 0;
	m_TestEnum = EnumValue03;
	m_pTestClassA = nullptr;
	m_pTestClassA2 = nullptr;
}

CClassB::~CClassB()
{
	if (m_pTestClassA)  { RELEASE(*m_pTestClassA); }
	if (m_pTestClassA2) { RELEASE(*m_pTestClassA2); }
}

void CClassB::Dispose()
{
	if (m_pTestClassA)  { RELEASE(*m_pTestClassA);  m_pTestClassA  = nullptr; }
	if (m_pTestClassA2) { RELEASE(*m_pTestClassA2); m_pTestClassA2 = nullptr; }
}

void CClassB::XMLSerialize(_INOUT IXMLStreamWrite& rWriter) const
{
	rWriter.PutValue(STRING("TestString"), *(new CString(m_TestString)));
	rWriter.PutValue(STRING("TestInt"),    CInt(m_TestInt).ToString().ToNew());
	rWriter.PutValue(STRING("TestEnum"),   *(new CString(*CXMLEnumeration::Translate(STRING("ClassBEnumeration"), m_TestEnum))));
}

void CClassB::XMLDeserialize(_INOUT IXMLStreamRead& rReader)
{
	CString* pValue = rReader.GetValue();
	if (pValue) { m_TestString = *pValue; }

	pValue = rReader.GetValue();
	if (pValue) { m_TestInt = CInt(*pValue).Value(); }

	pValue = rReader.GetValue();
	if (pValue) { m_TestEnum = CXMLEnumeration::Translate(STRING("ClassBEnumeration"), *pValue)->Value(); }
}

void CClassB::XMLDependentsSerialize(_INOUT IXMLStreamWriteObject& rWriter) const
{
	rWriter.Next(m_pTestClassA);
	rWriter.Next(m_pTestClassA2);
}

void CClassB::XMLDependentsDeserialize(_INOUT IXMLStreamReadObject& rReader)
{
	rReader.Next();
	m_pTestClassA  = (TestNS::CClassA*)rReader.Current();
	rReader.Next();
	m_pTestClassA2 = (TestNS::CClassA*)rReader.Current();

	//This is implementation dependent but it's nice to hold onto the objects & detect their improper deletion
	if (m_pTestClassA)  { ADDREF(*m_pTestClassA); }
	if (m_pTestClassA2) { ADDREF(*m_pTestClassA2); }
}

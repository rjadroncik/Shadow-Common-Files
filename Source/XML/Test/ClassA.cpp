#include "ClassA.h"

CClassA::CClassA(_IN int iValue)
{
	m_TestInt = iValue;
	m_pTestClassA = NULL;
	m_pTestClassBSub = new CClassB();
}

CClassA::~CClassA()
{
	if (m_pTestClassA) { RELEASE(*m_pTestClassA); }

	delete m_pTestClassBSub;
}

void CClassA::Dispose()
{
	if (m_pTestClassA) { RELEASE(*m_pTestClassA); m_pTestClassA = NULL; }
}

void CClassA::XMLSerialize(_INOUT IXMLStreamWrite& rWriter) const
{
	rWriter.PutValue(STRING("TestString"), *(new CString(m_TestString)));
	rWriter.PutValue(STRING("TestInt"),    *(new CInt   (m_TestInt)));
}

void CClassA::XMLDeserialize(_INOUT IXMLStreamRead& rReader)
{
	CString* pValue = rReader.GetValue();
	if (pValue) { m_TestString = *pValue; }

	pValue = rReader.GetValue();
	if (pValue) { m_TestInt = CInt(*pValue).Value(); }
}

void CClassA::XMLDependentsSerialize(_INOUT IXMLStreamWriteObject& rWriter) const
{
	rWriter.Next(m_pTestClassA);
	rWriter.Next(m_pTestClassBSub);
}

void CClassA::XMLDependentsDeserialize(_INOUT IXMLStreamReadObject& rReader)
{
	rReader.Next();
	m_pTestClassA    = (CClassA*)rReader.Current();
	rReader.Next();
	m_pTestClassBSub = (CClassB*)rReader.Current();

	if (m_pTestClassA) { ADDREF(*m_pTestClassA); }
}




#include "ClassA.h"

CClassB::CClassB()
{
	m_TestInt = 0;
	m_TestEnum = EnumValue03;
	m_pTestClassA = NULL;
	m_pTestClassA2 = NULL;
}

CClassB::~CClassB()
{
	if (m_pTestClassA)  { RELEASE(*(m_pTestClassA)); }
	if (m_pTestClassA2) { RELEASE(*(m_pTestClassA2)); }
}

void CClassB::Dispose()
{
	if (m_pTestClassA)  { RELEASE(*(m_pTestClassA));  m_pTestClassA  = NULL; }
	if (m_pTestClassA2) { RELEASE(*(m_pTestClassA2)); m_pTestClassA2 = NULL; }
}
#include "ClassA.h"

using namespace SCFBase;

CClassA::CClassA(_IN int iValue)
{
	m_TestInt = iValue;
	m_pTestClassA = nullptr;
	m_pTestClassBSub = new CClassB();
}

CClassA::~CClassA()
{
	if (m_pTestClassA) { RELEASE(*(m_pTestClassA)); }

	delete m_pTestClassBSub;
}

void CClassA::Dispose()
{
	if (m_pTestClassA) { RELEASE(*(m_pTestClassA)); m_pTestClassA = nullptr; }
}
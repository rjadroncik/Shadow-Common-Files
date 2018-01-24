#include "stdafx.h"
#include "TestBags.h"

CTestBags::CTestBags(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of bags"), &rErrorStream)
{
	m_pBag = nullptr;
}

CTestBags::~CTestBags()
{

}

bool CTestBags::Prepare() 
{
	m_pBag   = new CSetInt64();

	return true;
}
bool CTestBags::Run()    
{ 
	//CSetInt64
	{
		//Bags don't support storing 0 :/
		//m_pBag->Add(0); 

		//if (m_pBag->Contains(0))
		//{
		//	m_pBag->Add(1);
		//}

		for (UINT64 i = 0x00cdcdcd; i < 0x00cdddcd; i += 0x100)
		{
			m_pBag->Add(i);
		}

		CEnumeratorSetInt64 Enumerator(*m_pBag);

		while (Enumerator.Next())
		{
			CError::Stream()->PutLine(CPointer((void*)Enumerator.Current()).ToString());
			//Enumerator.CurrentRemove();
		}

		for (UINT64 i = 0x00cdcdcd; i < 0x01cdcdcd; i += 0x100)
		{
			m_pBag->Remove(i);
		}
	}
	
	return true;
}
bool CTestBags::Check()  
{
	if (!m_pBag->IsEmpty())   { return false; }

	return true; 
}
bool CTestBags::CleanUp() 
{
	delete m_pBag;

	return true;
}
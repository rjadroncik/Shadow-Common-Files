#include "stdafx.h"
#include "TestBags.h"

CTestBags::CTestBags(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of bags"), &rErrorStream)
{
	m_pBag = NULL;
}

CTestBags::~CTestBags()
{

}

bool CTestBags::Prepare() 
{
	m_pBag   = new CBagInt64();
	m_pBag16 = new CBagInt16();

	return TRUE;
}
bool CTestBags::Run()    
{ 
	//CBagInt64
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

		CEnumeratorBagInt64 Enumerator(*m_pBag);

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

	//CBagInt16
	{
		for (USHORT i = 0; i < 16384; i += 128)
		{
			m_pBag16->Add(i);
		}

		CEnumeratorBagInt16 Enumerator(*m_pBag16);

		while (Enumerator.Next())
		{
			CError::Stream()->PutLine(CInt(Enumerator.Current()).ToString());
			Enumerator.CurrentRemove();
		}

		for (USHORT i = 0; i < 16384; i += 128)
		{
			m_pBag16->Remove(i);
		}
	}

	return TRUE;
}
bool CTestBags::Check()  
{
	if (!m_pBag->IsEmpty())   { return FALSE; }
	if (!m_pBag16->IsEmpty()) { return FALSE; }
	//if (!m_pBag16->IsEmpty()) { return FALSE; }

	return TRUE; 
}
bool CTestBags::CleanUp() 
{
	delete m_pBag;
	delete m_pBag16;

	return TRUE;
}
#include "stdafx.h"
#include "TestCollections.h"

CTestCollections::CTestCollections(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of collections"), &rErrorStream)
{
	m_pArrayInt   = nullptr;
	m_pArrayFloat = nullptr;
	m_pVector     = nullptr;
	m_pStack      = nullptr;
	m_pList       = nullptr;

	m_pEnumeratorList = nullptr;
}
CTestCollections::~CTestCollections()
{
}

bool CTestCollections::Prepare() 
{
	m_pArrayInt = &CArrayInt::Parse(STRING("   25 0xf0000041 78 97 98 88"), nullptr);

	CFloat::FractionDigits(2, 2);
	m_pArrayFloat = &CArrayFloat::Parse(STRING("   25.6 65.9 78.7 97 98.0 88.10465"), nullptr);

	m_pVector = new CVector<CObject>();
	m_pStack  = new CStack<CString>();
	m_pList   = new CList<CObject>();

	m_pEnumeratorList = new CEnumeratorList<CObject>(*m_pList);

	return true;
}
bool CTestCollections::Run()    
{ 
	CError::Stream()->PutLine(m_pArrayInt->ToString());
	CError::Stream()->PutLine(m_pArrayFloat->ToString());

	//Vector1.LastAdd(STRING("foo")); //Exception!!! -> Adding temporary object to collection
	m_pVector->LastAdd(*(new STRING("String")));
	m_pVector->LastAdd(*(new CInt(2)));
	m_pVector->LastAdd(*m_pArrayInt);
	m_pVector->LastAdd(*(new CBool(true)));
	m_pVector->LastAdd(*(new CFloat(1.5f)));
	m_pVector->LastAdd(*m_pArrayFloat);

	m_pVector->Remove(*m_pArrayInt);
	m_pVector->Remove(*m_pArrayFloat);

	m_pStack->Push(*(new STRING("String1")));
	m_pStack->Push(*(new STRING("String2")));
	m_pStack->Push(*(new STRING("String3")));

	m_pStack->PopAndDelete();
	m_pStack->PopAndDelete();
	m_pStack->PopAndDelete();

	m_pList->LastAdd(*(new STRING("String")));
	m_pList->LastAdd(*(new STRING("String")));
	m_pList->FirstAdd(*(new CInt(5)));
	m_pList->FirstAdd(*(new CInt(5)));
	m_pList->LastAdd(*(new STRING("String")));
	m_pList->LastAdd(*(new STRING("String")));

	m_pList->FirstAdd(*(new CBool(true)));
	m_pList->LastAdd(*(new CFloat(1.5f)));
	m_pList->FirstAdd(*(new CBool(true)));
	m_pList->LastAdd(*(new CFloat(1.5f)));
	m_pList->FirstAdd(*(new CBool(true)));
	m_pList->LastAdd(*(new CFloat(1.5f)));
	m_pList->FirstAdd(*(new CBool(true)));
	m_pList->LastAdd(*(new CFloat(1.5f)));
	m_pList->FirstAdd(*(new CBool(true)));
	m_pList->LastAdd(*(new CFloat(1.5f)));
	m_pList->FirstAdd(*(new CBool(true)));
	m_pList->LastAdd(*(new CFloat(1.5f)));

	m_pList->At(0);
	m_pList->At(1);
	m_pList->At(m_pList->Size() - 2);
	m_pList->At(m_pList->Size() - 1);

	m_pList->Insert(17, *(new CFloat(1000.0f)));
	
	CInt i(0); 

	while (m_pEnumeratorList->Next())
	{
		CError::Stream()->PutString(i.ToString());
		CError::Stream()->PutString(STRING(": "));
		CError::Stream()->PutString(m_pEnumeratorList->Current()->ToString());
		CError::Stream()->PutString(STRING(" | "));
		CError::Stream()->PutLine((*m_pList)[i.Value()].ToString());

		i += 1;
	}
	
	m_pList->Delete(17);

	m_pList->FirstDelete();
	m_pList->FirstDelete();
	m_pList->LastDelete();
	m_pList->LastDelete();


	return true;
}
bool CTestCollections::Check()  
{
	if (m_pArrayInt->Size()   != 6)  { return false; }
	if (m_pArrayFloat->Size() != 6)  { return false; }
	if (m_pVector->Size()     != 4)  { return false; }
	if (m_pStack->Size()      != 0)  { return false; }
	if (m_pList->Size()       != 14) { return false; }

	return true; 
}
bool CTestCollections::CleanUp() 
{
	m_pVector->AllDelete();
	m_pList->AllDelete();

	delete m_pEnumeratorList;

	delete m_pArrayInt;
	delete m_pArrayFloat;
	delete m_pVector;
	delete m_pStack;
	delete m_pList;

	return true;
}


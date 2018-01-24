#include "stdafx.h"
#include "TestDictionaries.h"

CTestDictionaries::CTestDictionaries(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of dictionaries"), &rErrorStream)
{
	m_pDictionaryInt64 = nullptr;
	m_pDictionaryObject = nullptr;
	m_pDictionaryString = nullptr;

	m_pEnumeratorDictionaryInt64 = nullptr;
	m_pEnumeratorDictionaryObject = nullptr;
	m_pEnumeratorDictionaryString = nullptr;
}

CTestDictionaries::~CTestDictionaries()
{

}

bool CTestDictionaries::Prepare() 
{
	m_pDictionaryInt64  = new CDictionaryInt64<CInt>();
	m_pDictionaryObject = new CDictionaryObject<CInt, CString>(*(new CComparerInt()));
	m_pDictionaryString = new CDictionaryString<CInt>();

	return true;
}
bool CTestDictionaries::Run()    
{ 
	CDictionaryInt64<CInt> dict;

	{
		for (UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryInt64->AtPut(i, *(new CInt(i)));
			if (!m_pDictionaryInt64->At(i)) { return false; } 
		}

		m_pEnumeratorDictionaryInt64 = new CEnumeratorDictionaryInt64<CInt>(*m_pDictionaryInt64);
		while (m_pEnumeratorDictionaryInt64->Next())
		{
			CError::Stream()->PutLine(CInt64(m_pEnumeratorDictionaryInt64->CurrentKey()).ToString() + STRING(" -> ") + m_pEnumeratorDictionaryInt64->Current()->ToString());
		}

		for (UINT i = 0; i < 20; i++)
		{
			delete m_pDictionaryInt64->Remove(i);
			if (m_pDictionaryInt64->At(i)) { return false; }
		}
	}
	{
		for (UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryObject->AtPut((*new CInt(i)), *(new CString(CInt(i).ToString()))); 
			if (!m_pDictionaryObject->At(CInt(i))) { return false; } 
		}

		m_pEnumeratorDictionaryObject = new CEnumeratorDictionaryObject<CInt, CString>(*m_pDictionaryObject);
		while (m_pEnumeratorDictionaryObject->Next())
		{
			CError::Stream()->PutLine(m_pEnumeratorDictionaryObject->CurrentKey()->ToString() + STRING(" -> ") + m_pEnumeratorDictionaryObject->Current()->ToString());
		}

		for (UINT i = 0; i < 20; i++)
		{
			delete m_pDictionaryObject->RemoveAndDeleteKey(CInt(i));
			if (m_pDictionaryObject->At(CInt(i))) { return false; }
		}
	}
	{
		for (UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryString->AtPut(CInt(i).ToString(), *(new CInt(i))); 
			if (!m_pDictionaryString->At(CInt(i).ToString())) { return false; } 
		}

		m_pEnumeratorDictionaryString = new CEnumeratorDictionaryString<CInt>(*m_pDictionaryString, STRING("1"));
		while (m_pEnumeratorDictionaryString->Next())
		{
			CError::Stream()->PutLine(m_pEnumeratorDictionaryString->CurrentPath() + STRING(" -> ") + m_pEnumeratorDictionaryString->Current()->ToString());
		}

		for (UINT i = 0; i < 20; i++)
		{
			delete m_pDictionaryString->RemoveKey(CInt(i).ToString());
			if (m_pDictionaryString->At(CInt(i).ToString())) { return false; }
		}
	}
	return true;
}
bool CTestDictionaries::Check()  
{
	if (!m_pDictionaryInt64->IsEmpty())  { return false; }
	if (!m_pDictionaryObject->IsEmpty()) { return false; }
	if (!m_pDictionaryString->IsEmpty()) { return false; }
	return true; 
}
bool CTestDictionaries::CleanUp() 
{
	delete m_pEnumeratorDictionaryInt64;
	delete m_pEnumeratorDictionaryObject;
	delete m_pEnumeratorDictionaryString;

	m_pDictionaryInt64->AllDelete();
	m_pDictionaryObject->AllDeleteWithKeys();
	m_pDictionaryString->AllDelete();

	delete m_pDictionaryInt64;
	delete m_pDictionaryObject;
	delete m_pDictionaryString;

	return true;
}


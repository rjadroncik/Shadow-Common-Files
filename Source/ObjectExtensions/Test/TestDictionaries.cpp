#include "stdafx.h"
#include "TestDictionaries.h"

CTestDictionaries::CTestDictionaries(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of dictionaries"), &rErrorStream)
{
	m_pDictionaryInt64 = NULL;
	m_pDictionaryObject = NULL;
	m_pDictionaryString = NULL;

	m_pEnumeratorDictionaryInt64 = NULL;
	m_pEnumeratorDictionaryObject = NULL;
	m_pEnumeratorDictionaryString = NULL;
}

CTestDictionaries::~CTestDictionaries()
{

}

bool CTestDictionaries::Prepare() 
{
	m_pDictionaryInt64  = new CDictionaryInt64();
	m_pDictionaryObject = new CDictionaryObject<CInt, CString>(*(new CComparerInt()));
	m_pDictionaryString = new CDictionaryString<CInt>();

	return TRUE;
}
bool CTestDictionaries::Run()    
{ 
	{
		for (SCF::UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryInt64->AtPut(i, *(new CString(CInt(i).ToString()))); 
			if (!m_pDictionaryInt64->At(i)) { return FALSE; } 
		}

		m_pEnumeratorDictionaryInt64 = new CEnumeratorDictionaryInt64(*m_pDictionaryInt64);
		while (m_pEnumeratorDictionaryInt64->Next())
		{
			CError::Stream()->PutLine(CInt64(m_pEnumeratorDictionaryInt64->CurrentKey()).ToString() + STRING(" -> ") + m_pEnumeratorDictionaryInt64->Current()->ToString());
		}

		for (SCF::UINT i = 0; i < 20; i++)
		{
			delete m_pDictionaryInt64->Remove(i);
			if (m_pDictionaryInt64->At(i)) { return FALSE; }
		}
	}
	{
		for (SCF::UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryObject->AtPut((*new CInt(i)), *(new CString(CInt(i).ToString()))); 
			if (!m_pDictionaryObject->At(CInt(i))) { return FALSE; } 
		}

		m_pEnumeratorDictionaryObject = new CEnumeratorDictionaryObject(*m_pDictionaryObject);
		while (m_pEnumeratorDictionaryObject->Next())
		{
			CError::Stream()->PutLine(m_pEnumeratorDictionaryObject->CurrentKey()->ToString() + STRING(" -> ") + m_pEnumeratorDictionaryObject->Current()->ToString());
		}

		for (SCF::UINT i = 0; i < 20; i++)
		{
			delete m_pDictionaryObject->RemoveAndDeleteKey(CInt(i));
			if (m_pDictionaryObject->At(CInt(i))) { return FALSE; }
		}
	}
	{
		for (SCF::UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryString->AtPut(CInt(i).ToString(), *(new CInt(i))); 
			if (!m_pDictionaryString->At(CInt(i).ToString())) { return FALSE; } 
		}

		m_pEnumeratorDictionaryString = new CEnumeratorDictionaryString(*m_pDictionaryString, STRING("1"));
		while (m_pEnumeratorDictionaryString->Next())
		{
			CError::Stream()->PutLine(m_pEnumeratorDictionaryString->CurrentPath() + STRING(" -> ") + m_pEnumeratorDictionaryString->Current()->ToString());
		}

		for (SCF::UINT i = 0; i < 20; i++)
		{
			delete m_pDictionaryString->RemoveKey(CInt(i).ToString());
			if (m_pDictionaryString->At(CInt(i).ToString())) { return FALSE; }
		}
	}
	return TRUE;
}
bool CTestDictionaries::Check()  
{
	if (!m_pDictionaryInt64->IsEmpty())  { return FALSE; }
	if (!m_pDictionaryObject->IsEmpty()) { return FALSE; }
	if (!m_pDictionaryString->IsEmpty()) { return FALSE; }
	return TRUE; 
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

	return TRUE;
}


#include "stdafx.h"
#include "TestShallowDelete.h"

CTestShallowDelete::CTestShallowDelete(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of shallow deletion"), &rErrorStream)
{
	m_pDictionaryInt64 = nullptr;
	m_pDictionaryObject = nullptr;
	m_pDictionaryString = nullptr;

	m_pEnumeratorDictionaryInt64 = nullptr;
	m_pEnumeratorDictionaryObject = nullptr;
	m_pEnumeratorDictionaryString = nullptr;
}
CTestShallowDelete::~CTestShallowDelete()
{
}

bool CTestShallowDelete::Prepare() 
{
	m_pDictionaryInt64  = new CDictionaryInt64<CInt>();
	m_pDictionaryObject = new CDictionaryObject<CInt, CString>(*(new CComparerInt()));
	m_pDictionaryString = new CDictionaryString<CInt>();

	return true;
}
bool CTestShallowDelete::Run()
{
	{
		for (UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryInt64->AtPut(i, *(new CInt(i)));
			if (!m_pDictionaryInt64->At(i)) { return false; } 
		}

		m_pEnumeratorDictionaryInt64 = new CEnumeratorDictionaryInt64<CInt>(*m_pDictionaryInt64);
		while (m_pEnumeratorDictionaryInt64->Next())
		{
			m_pEnumeratorDictionaryInt64->CurrentShallowDelete();
		}
	}
	{
		for (UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryObject->AtPut((*new CInt(i)), *(new STRING("Text"))); 
			if (!m_pDictionaryObject->At(CInt(i))) { return false; } 
		}

		m_pEnumeratorDictionaryObject = new CEnumeratorDictionaryObject<CInt, CString>(*m_pDictionaryObject);
		while (m_pEnumeratorDictionaryObject->Next())
		{
			m_pEnumeratorDictionaryObject->CurrentShallowDelete();
		}
	}
	{
		for (UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryString->AtPut(CInt(i).ToString(), *(new CInt(i))); 
			if (!m_pDictionaryString->At(CInt(i).ToString())) { return false; } 
		}

		m_pEnumeratorDictionaryString = new CEnumeratorDictionaryString<CInt>(*m_pDictionaryString);
		while (m_pEnumeratorDictionaryString->Next())
		{
			m_pEnumeratorDictionaryString->CurrentShallowDelete();
		}
	}

	return true;
}
bool CTestShallowDelete::Check()
{
	{
		if (!m_pDictionaryInt64->IsEmpty()) { return false; }

		for (UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryInt64->AtPut(i, *(new CInt(i)));
			if (!m_pDictionaryInt64->At(i)) { return false; } 
		}

		for (UINT i = 0; i < 20; i++)
		{
			delete m_pDictionaryInt64->Remove(i);
			if (m_pDictionaryInt64->At(i)) { return false; }
		}

		if (!m_pDictionaryInt64->IsEmpty()) { return false; }
	}
	{
		if (!m_pDictionaryObject->IsEmpty()) { return false; }

		for (UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryObject->AtPut(CInt(i), *(new STRING("Text"))); 
			if (!m_pDictionaryObject->At(CInt(i))) { return false; } 
		}

		for (UINT i = 0; i < 20; i++)
		{
			delete m_pDictionaryObject->RemoveAndDeleteKey(CInt(i));
			if (m_pDictionaryObject->At(CInt(i))) { return false; }
		}

		if (!m_pDictionaryObject->IsEmpty()) { return false; }
	}
	{
		if (!m_pDictionaryString->IsEmpty()) { return false; }

		for (UINT i = 0; i < 20; i++) 
		{ 
			m_pDictionaryString->AtPut(CInt(i).ToString(), *(new CInt(i))); 
			if (!m_pDictionaryString->At(CInt(i).ToString())) { return false; } 
		}

		for (UINT i = 0; i < 20; i++)
		{
			delete m_pDictionaryString->RemoveKey(CInt(i).ToString());
			if (m_pDictionaryString->At(CInt(i).ToString())) { return false; }
		}

		if (!m_pDictionaryString->IsEmpty()) { return false; }
	}
	return true; 
}
bool CTestShallowDelete::CleanUp() 
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


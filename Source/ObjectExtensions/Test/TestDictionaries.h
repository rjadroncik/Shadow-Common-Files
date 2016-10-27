#pragma once

#include <SCFDebugging.h>

using namespace SCFDebugging;
using namespace SCFBase;

class CTestDictionaries : public CTestCase
{
public:
	CTestDictionaries(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestDictionaries();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CDictionaryInt64<CInt>*  m_pDictionaryInt64;
	CDictionaryObject<CInt, CString>* m_pDictionaryObject;
	CDictionaryString<CInt>*          m_pDictionaryString;

	CEnumeratorDictionaryInt64<CInt>*  m_pEnumeratorDictionaryInt64;
	CEnumeratorDictionaryObject<CInt, CString>* m_pEnumeratorDictionaryObject;
	CEnumeratorDictionaryString<CInt>* m_pEnumeratorDictionaryString;
};

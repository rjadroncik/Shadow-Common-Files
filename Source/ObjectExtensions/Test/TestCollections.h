#pragma once

#include <SCFDebugging.h>

using namespace SCFDebugging;
using namespace SCFBase;

class CTestCollections : public CTestCase
{
public:
	CTestCollections(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestCollections();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CArrayInt*   m_pArrayInt;
	CArrayFloat* m_pArrayFloat;
	
	CVector<CObject>* m_pVector;
	CList<CObject>*   m_pList;

	CStack<CString>* m_pStack;

	CEnumeratorList<CObject>* m_pEnumeratorList;
};

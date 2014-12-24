#pragma once

#include <SCFDebugging.h>

using namespace SCFDebugging;
using namespace SCFBase;

class CTestSerialization : public CTestCase2Phase
{
public:
	CTestSerialization(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestSerialization();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

protected:
	bool Prepare2();
	bool Run2();
	bool Check2();
	bool CleanUp2();

private:
	CArrayInt*   m_pArrayInt;
	CArrayFloat* m_pArrayFloat;

	CAssociation*     m_pAssociation;
	CVector<CObject>* m_pVector;
	CList<CObject>*   m_pList;

	CDictionaryString<CFloat>* m_pDictionaryString;
	//CDictionaryInt64*  m_pDictionaryInt64;

private:
	CVector<CObject> m_ReadObjects;

private:
	CStreamFileWrite*   m_pStreamWrite;
	CStreamWriteObject* m_pStreamWriteObject;

	CStreamFileRead*   m_pStreamRead;
	CStreamReadObject* m_pStreamReadObject;
};

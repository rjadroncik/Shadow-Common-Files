#pragma once

#include <SCFDebugging.h>
#include <SCFXML.h>

using namespace SCFDebugging;
using namespace SCFBase;
using namespace SCFXML;

#include "ClassA.h"
#include "ClassB.h"

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
	CXMLDocumentFile* m_pDocument;
	
	CXMLStreamWriteObject* m_pStreamWrite;
	CXMLStreamReadObject* m_pStreamRead;

private:
	CDictionaryString<CObject> m_ObjectsPersistent;
	CVector<CObject> m_Objects;
	
	CClassA* m_pObjectA;
	CClassA* m_pObjectA2;
	CClassB* m_pObjectB;

private:
	static bool s_bRegistered;
};

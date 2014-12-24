#pragma once

#include <SCFDebugging.h>
#include <SCFXML.h>

using namespace SCFDebugging;
using namespace SCFBase;
using namespace SCFXML;

class CTestDOM : public CTestCase
{
public:
	CTestDOM(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestDOM();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CXMLNode* m_pRoot;
	CXMLEnumerator* m_pEnumerator;
};

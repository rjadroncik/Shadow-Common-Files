#pragma once

#include <SCFDebugging.h>
#include <SCFXML.h>

using namespace SCFDebugging;
using namespace SCFBase;
using namespace SCFXML;

class CTestDocument : public CTestCase
{
public:
	CTestDocument(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestDocument();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CXMLDocumentFile* m_pDocument;
	CXMLEnumerator* m_pEnumerator;
};

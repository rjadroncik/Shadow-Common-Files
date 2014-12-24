#pragma once

#include <SCFDebugging.h>
#include <SCFXML.h>

using namespace SCFDebugging;
using namespace SCFBase;
using namespace SCFXML;

class CTestStreams : public CTestCase
{
public:
	CTestStreams(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestStreams();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CXMLDocumentFile* m_pDocument;
	CXMLStreamRead* m_pStreamRead;
};

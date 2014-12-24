#pragma once

#include <SCFDebugging.h>
#include <SCFXML.h>

using namespace SCFDebugging;
using namespace SCFBase;
using namespace SCFXML;

class CTestPath : public CTestCase
{
public:
	CTestPath(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestPath();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

protected:
	void ProcessResults(_IN SCF::UINT uiExpectedResultCount);

private:
	CXMLDocumentFile* m_pDocument;
	CXMLPath* m_pPath;
};

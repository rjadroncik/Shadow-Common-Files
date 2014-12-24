#pragma once

#include <SCFDebugging.h>
#include <SCFXML.h>

using namespace SCFDebugging;
using namespace SCFBase;
using namespace SCFXML;

class CTestReader : public CTestCase
{
public:
	CTestReader(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestReader();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CXMLReader* m_pReader;

	CXMLNode* m_pRootNew;
	CXMLNode* m_pRootOld;
};

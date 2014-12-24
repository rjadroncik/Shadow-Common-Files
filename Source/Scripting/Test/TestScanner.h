#pragma once

#include <SCFDebugging.h>
#include <SCFScripting.h>

using namespace SCFBase;
using namespace SCFDebugging;
using namespace SCFScripting;
using namespace SCFCompiler;

class CTestScanner : public CTestCase
{
public:
	CTestScanner(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestScanner();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CScanner* m_pScanner;
};

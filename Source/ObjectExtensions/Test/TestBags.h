#pragma once

#include <SCFDebugging.h>
#include <SCFObjectExtensionsPrivate.h>

using namespace SCFDebugging;
using namespace SCFBase;

class CTestBags : public CTestCase
{
public:
	CTestBags(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestBags();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CBagInt64*   m_pBag;
};

#pragma once

#include <SCFDebugging.h>

using namespace SCFDebugging;
using namespace SCFBase;

class CTestCSV : public CTestCase
{
public:
	CTestCSV(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestCSV();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CStreamFileRead*        m_pStreamFileRead;
	CStreamReadTextGeneric* m_pStreamReadText;
	CStreamReadCSV*         m_pStreamReadCSV;
};

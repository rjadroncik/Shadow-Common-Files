#pragma once

#include <SCFDebugging.h>

using namespace SCFDebugging;
using namespace SCFBase;

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
	CStreamFileWrite*        m_pStreamWrite;
	CStreamWriteTextGeneric* m_pStreamWriteText;

	CStreamFileRead*        m_pStreamRead;
	CStreamReadTextGeneric* m_pStreamReadText;

	CString m_csReadBackString;

private:
	static ENUM s_eEncoding;
};

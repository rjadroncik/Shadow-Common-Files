#pragma once

#include <SCFDebugging.h>
#include <SCFScripting.h>

using namespace SCFDebugging;
using namespace SCFScripting;
using namespace SCFObjectExtensions;

class CTestAssembler : public CTestCase
{
public:
	CTestAssembler(_INOUT CStreamWriteText& rErrorStream);
	virtual ~CTestAssembler();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool Dispose();

private:
	CExecutable* m_pExecutable;

	CStreamFileRead*  m_pStreamFileRead;
	CStreamReadText*  m_pStreamReadText;
};

#pragma once

#include <SCFDebugging.h>
#include <SCFObjectExtensionsPrivate.h>

using namespace SCFDebugging;
using namespace SCFBase;

#define RUN_COUNT 100000

class CTestFSBHeap : public CTestCase
{
public:
	CTestFSBHeap(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestFSBHeap();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	SCFPrivate::CFSBHeap* m_pHeap;

	void* m_vpBlocks[RUN_COUNT];
};

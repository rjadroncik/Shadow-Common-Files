#pragma once

#include <SCFDebugging.h>
#include <SCFObjectExtensionsPrivate.h>

using namespace SCFDebugging;
using namespace SCFBase;

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

	void* m_vpData01;
	void* m_vpData02;
	void* m_vpData03;
	void* m_vpData04;
	void* m_vpData05;
	void* m_vpData06;
	void* m_vpData07;
};

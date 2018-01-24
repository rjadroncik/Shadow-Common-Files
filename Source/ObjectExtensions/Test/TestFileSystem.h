#pragma once

#include <SCFDebugging.h>

using namespace SCFDebugging;
using namespace SCFBase;

class CTestFileSystem : public CTestCase
{
public:
	CTestFileSystem(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestFileSystem();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

protected:
	void PrintDir(_INOUT CDirectory& rDirectory, _IN UINT uiIndent = 0);

private:
	CDirectory* m_pDirectorySource;
	CDirectory* m_pDirectoryDestination;

	CFile* m_pFileSource;
	CFile* m_pFileDestination;
};

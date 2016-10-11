#pragma once

#include <SCFDebugging.h>
#include <SCFDatafileIO.h>

using namespace SCFDebugging;
using namespace SCFDatafileIO;
using namespace SCFBase;

class CTestDatafile : public CTestCase
{
public:
	CTestDatafile(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestDatafile();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
	CVector<CDFDirectory> m_Directories;
	CVector<CDFFile>      m_Files;

	CDatafile* m_pDatafile;
};

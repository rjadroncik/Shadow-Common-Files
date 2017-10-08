#pragma once

#include <SCFDebugging.h>
#include <SCFScripting.h>

using namespace SCFBase;
using namespace SCFDebugging;
using namespace SCFScripting;
using namespace SCFCompiler;

class CTestParser : public CTestCase
{
public:
	CTestParser(_INOUT IStreamWriteText& rErrorStream);
	virtual ~CTestParser();

protected:
	bool Prepare();
	bool Run();
	bool Check();
	bool CleanUp();

private:
   CCompiler*     m_pCompiler;
	CParser*       m_pParser;
	CScanner*      m_pScanner;

private:
	CPackage* m_pSCFBase;
	CPackage* m_pSCFXML;

	CClass* m_pSCFBase_Object;
	CClass* m_pSCFXML_Parser;
};

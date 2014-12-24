#include "TestParser.h"

CTestParser::CTestParser(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of scanner"), &rErrorStream)
{
	m_pParser  = NULL;

	m_pSCFBase = NULL;
	m_pSCFXML  = NULL;

	m_pSCFBase_Object = NULL;
	m_pSCFXML_Parser  = NULL;
}
CTestParser::~CTestParser()
{

}

bool CTestParser::Prepare() 
{
	CFile File(STRING("Data\\Scripting\\Compiler\\Test01.ssf.scanout"));
	if (!File.Exists()) 
	{
		CError::Stream()->PutLine(STRING("error : ") + File.ToString() + STRING(" : file not found"));
		return FALSE; 
	}

	CStreamFileRead StreamRead(File);
	CStreamReadObject ReadStream(StreamRead);

	ReadStream.Next();

	m_pTokens = (CList<CToken>*)ReadStream.Current();

    m_pCompiler = new CCompiler();
	m_pParser  = new CParser(*m_pCompiler);
	
	m_pSCFBase_Object = new CClass();
	m_pSCFBase_Object->Name(STRING("Object"));

	m_pSCFXML_Parser = new CClass();
	m_pSCFXML_Parser->Name(STRING("Parser"));

	m_pSCFBase = new CPackage();
	m_pSCFBase->Name(STRING("SCFBase"));
	m_pSCFBase->ClassAdd(*m_pSCFBase_Object);

	m_pSCFXML  = new CPackage();
	m_pSCFXML->Name(STRING("SCFXML"));
	m_pSCFXML->ClassAdd(*m_pSCFXML_Parser);

	m_pCompiler->Packages().AtPut(m_pSCFBase->Name(), *m_pSCFBase);
	m_pCompiler->Packages().AtPut(m_pSCFXML->Name(), *m_pSCFXML);

	return TRUE;
}
bool CTestParser::Run()    
{ 
	CCompilationUnit CompilationUnit;
	
	m_pParser->Parse(*m_pTokens, CompilationUnit);

	return TRUE;
}
bool CTestParser::Check()  
{
	return TRUE; 
}
bool CTestParser::CleanUp() 
{
    delete m_pCompiler;
	delete m_pParser;

	m_pTokens->AllDelete();
	delete m_pTokens;

	delete m_pSCFBase;
	delete m_pSCFXML;

	delete m_pSCFBase_Object;
	delete m_pSCFXML_Parser;

	return TRUE;
}


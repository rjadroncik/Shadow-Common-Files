#include "TestScanner.h"

CTestScanner::CTestScanner(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of scanner"), &rErrorStream)
{
	m_pScanner = NULL;
	m_pParser  = NULL;

	m_pSCFBase = NULL;
	m_pSCFXML  = NULL;

	m_pSCFBase_Object = NULL;
	m_pSCFXML_Parser  = NULL;
}
CTestScanner::~CTestScanner()
{

}

bool CTestScanner::Prepare() 
{
	CFile File(STRING("Data\\Scripting\\Compiler\\Test01.ssf"));
	if (!File.Exists()) 
	{
		CError::Stream()->PutLine(STRING("error : 'Data\\Scripting\\Compiler\\Test01.ssf' : file not found"));
		return FALSE; 
	}

	m_pScanner = new CScanner();
	m_pParser  = new CParser();

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

//	m_pParser->RegisterPackage()

	return TRUE;
}
bool CTestScanner::Run()    
{ 
	CFile File(STRING("Data\\Scripting\\Compiler\\Test01.ssf"));

	CString Text;
	CStreamFileRead StreamRead(File);
	CStreamReadTextGeneric StreamReadText(StreamRead);

	StreamReadText.GetString(Text, 0);

	m_pScanner->Scan(Text);

	CString PrintedWordTypes;
	CString TextReconstructed;
	m_pScanner->TextReconstruct(TextReconstructed);
	m_pScanner->PrintWordTypes(PrintedWordTypes);

	CError::Stream()->PutLine(TextReconstructed);
	CError::Stream()->PutLine(PrintedWordTypes);

	m_pParser->Parse(m_pScanner->Tokens());

	return TRUE;
}
bool CTestScanner::Check()  
{
	return TRUE; 
}
bool CTestScanner::CleanUp() 
{
	delete m_pScanner;
	delete m_pParser;

	delete m_pSCFBase;
	delete m_pSCFXML;

	delete m_pSCFBase_Object;
	delete m_pSCFXML_Parser;

	return TRUE;
}


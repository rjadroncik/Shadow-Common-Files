#include "TestScanner.h"

CTestScanner::CTestScanner(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of scanner"), &rErrorStream)
{
	m_pScanner = NULL;
}
CTestScanner::~CTestScanner()
{

}

bool CTestScanner::Prepare() 
{
	CFile File(STRING("Data\\Scripting\\Compiler\\Test01.ssf"));
	if (!File.Exists()) 
	{
		CError::Stream()->PutLine(STRING("error : ") + File.ToString() + STRING(" : file not found"));
		return FALSE; 
	}

	m_pScanner = new CScanner();

	return TRUE;
}
bool CTestScanner::Run()    
{ 
	CFile FileRead(STRING("Data\\Scripting\\Compiler\\Test01.ssf"));
	CStreamFileRead StreamRead(FileRead);
	CStreamReadTextGeneric StreamReadText(StreamRead);

	CString Text;
	StreamReadText.GetString(Text, 0);

	m_pScanner->Scan(Text);

	CString PrintedWordTypes;
	CString PrintedWords;
	m_pScanner->PrintWords(PrintedWords);
	m_pScanner->PrintWordTypes(PrintedWordTypes);

	CError::Stream()->PutLine("--------------------------------------------");

	for (UINT i = 0; i < PrintedWords.Length(); i += 80)
	{
		UINT uiLength = __min(80, PrintedWords.Length() - i);

		CError::Stream()->PutString(CStringRange(PrintedWords, i, uiLength));
		if (uiLength < 80) { CError::Stream()->PutLine(); }

		CError::Stream()->PutLine  (CStringRange(PrintedWordTypes, i, __min(80, PrintedWordTypes.Length() - i)));
		if (uiLength < 80) { CError::Stream()->PutLine(); }
	}

	CString TextReconstructed;
	m_pScanner->TextReconstruct(TextReconstructed);

	CError::Stream()->PutLine("--------------------------------------------");
	CError::Stream()->PutString(TextReconstructed);
	CError::Stream()->PutLine();
	CError::Stream()->PutLine("--------------------------------------------");

	CFile FileWrite(FileRead.Path(), FileRead.NameFull(), STRING("scanout"));
	CStreamFileWrite StreamWrite(FileWrite);
	CStreamWriteObject WriteStream(StreamWrite);

	WriteStream.Next(m_pScanner->Tokens());

	return TRUE;
}
bool CTestScanner::Check()  
{
	return TRUE; 
}
bool CTestScanner::CleanUp() 
{
	delete m_pScanner;

	return TRUE;
}


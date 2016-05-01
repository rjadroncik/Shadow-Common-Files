#include "stdafx.h"
#include "TestStreams.h"

ENUM CTestStreams::s_eEncoding = EncodingUCS2;

CTestStreams::CTestStreams(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of streams"), &rErrorStream)
{
	m_pStreamWrite = NULL;
	m_pStreamWriteText = NULL;

	m_pStreamRead = NULL;
	m_pStreamReadText = NULL;
}
CTestStreams::~CTestStreams()
{

}

bool CTestStreams::Prepare() 
{
	m_pStreamWrite = new CStreamFileWrite(CFile(STRING("Data\\ObjectExtensions\\Tmp.txt")), false);
	m_pStreamWriteText = new CStreamWriteTextGeneric(*m_pStreamWrite, s_eEncoding);

	s_eEncoding = 2 - s_eEncoding;
	return TRUE;
}
bool CTestStreams::Run()    
{ 
	m_pStreamWriteText->PutLine(STRING("Some text..."));
	m_pStreamWriteText->PutLine(STRING("Some more text..."));
	m_pStreamWriteText->PutLine(STRING("Textik v slovenèine... jupíí!... úžasné..."));
	m_pStreamWriteText->Close();

	//Can't be placed into [Prepare()] (must write & close stream first..)
	m_pStreamRead = new CStreamFileRead(CFile(STRING("Data\\ObjectExtensions\\Tmp.txt")));
	m_pStreamReadText = new CStreamReadTextGeneric(*m_pStreamRead);

	CString ReadBuffer;

	while (m_pStreamReadText->GetLine(ReadBuffer))
	{
		m_csReadBackString += ReadBuffer;
		m_csReadBackString += STRING("\n");
	}
	
	m_pStreamRead->Close();

	return TRUE;
}
bool CTestStreams::Check()  
{
	static CString s_Text(STRING("Some text...\n") + STRING("Some more text...\n") + STRING("Textik v slovenèine... jupíí!... úžasné...\n"));

	return (m_csReadBackString == s_Text);
}
bool CTestStreams::CleanUp() 
{
	delete m_pStreamWriteText;
	delete m_pStreamWrite;

	delete m_pStreamReadText;
	delete m_pStreamRead;

	return TRUE;
}


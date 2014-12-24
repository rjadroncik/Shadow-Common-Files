#include "stdafx.h"
#include "TestAssembler.h"

CTestAssembler::CTestAssembler(_INOUT CStreamWriteText& rErrorStream) : CTestCase(STRING("Test of assembler"), &rErrorStream)
{
	m_pExecutable = NULL;

	m_pStreamFileRead = NULL;
	m_pStreamReadText = NULL;
}
CTestAssembler::~CTestAssembler()
{

}

bool CTestAssembler::Prepare() 
{
	m_pExecutable = new CExecutable(4096);

	CFile File(STRING("Data\\Scripting\\Test.asm"));
	if (!File.Exists()) 
	{
		CError::Stream()->PutLine(STRING("error : 'Data\\Scripting\\Test.asm' : file not found"));
		return FALSE; 
	}

	m_pStreamFileRead = new CStreamFileRead(File);
	m_pStreamReadText = new CStreamReadCSV(*m_pStreamFileRead);

	return TRUE;
}
bool CTestAssembler::Run()    
{ 
	CAssembler::Assemble(*m_pStreamReadText ,*m_pExecutable);

	m_pExecutable->DumpCode(STRING("Data\\Scripting\\Test.dmp"));

/*	CString Value;

	while (m_pStreamReadCSV->BlockNext())
	{
		while (m_pStreamReadCSV->GetValue(Value))
		{
			CError::Stream()->PutString(Value);
			CError::Stream()->PutString(TEXT(" "));
		}
		CError::Stream()->PutString(TEXT("\n"));
	}*/

	return TRUE;
}
bool CTestAssembler::Check()  
{
	return TRUE; 
}
bool CTestAssembler::Dispose() 
{
	delete m_pExecutable;

	delete m_pStreamReadText;
	delete m_pStreamFileRead;

	return TRUE;
}


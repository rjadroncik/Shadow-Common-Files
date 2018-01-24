#include "stdafx.h"
#include "TestCSV.h"

CTestCSV::CTestCSV(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of CSV streams"), &rErrorStream)
{
	m_pStreamFileRead = nullptr;
	m_pStreamReadText = nullptr;
	m_pStreamReadCSV  = nullptr;
}
CTestCSV::~CTestCSV()
{

}

bool CTestCSV::Prepare() 
{
	CFile File(STRING("Data\\ObjectExtensions\\Test.csv"));
	if (!File.Exists()) 
	{
		CError::Stream()->PutString(STRING("Error : 'Data\\ObjectExtensions\\Test.csv' : file not found"));
		return false; 
	}

	m_pStreamFileRead = new CStreamFileRead(File);
	m_pStreamReadText = new CStreamReadTextGeneric(*m_pStreamFileRead);
	m_pStreamReadCSV  = new CStreamReadCSV(*m_pStreamReadText);

	return true;
}

bool CTestCSV::Run()    
{ 
	CString Value;

	while (m_pStreamReadCSV->BlockNext())
	{
		while (m_pStreamReadCSV->GetValue(Value))
		{
			CError::Stream()->PutString(Value);
			CError::Stream()->PutString(STRING(" "));
		}
		CError::Stream()->PutLine();
	}

	return true;
}
bool CTestCSV::Check()  
{
	return true; 
}
bool CTestCSV::CleanUp() 
{
	delete m_pStreamReadCSV;
	delete m_pStreamReadText;
	delete m_pStreamFileRead;

	return true;
}


#include "TestStreams.h"

CTestStreams::CTestStreams(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of streams"), &rErrorStream)
{
	m_pDocument = NULL;
	m_pStreamRead = NULL;
}
CTestStreams::~CTestStreams()
{

}

bool CTestStreams::Prepare() 
{
	m_pDocument = new CXMLDocumentFile(*(new CFile(STRING("Data\\XML\\Test.xml"))));
	m_pStreamRead = new CXMLStreamRead(*m_pDocument);

	return TRUE;
}
bool CTestStreams::Run()    
{ 
	CString* pBlock = NULL;
	CString* pKey   = NULL;
	CString* pValue = NULL;

	while ((pBlock = m_pStreamRead->GetBlock()) != 0)
	{
		while ((pValue = m_pStreamRead->GetValue(&pKey)) != 0)
		{
			CError::Stream()->PutString(STRING("("));
			CError::Stream()->PutString(*pBlock);
			CError::Stream()->PutString(STRING(") "));
			CError::Stream()->PutString(*pKey);
			CError::Stream()->PutString(STRING(" -> "));
			CError::Stream()->PutLine(*pValue);
		}
	}

	return TRUE;
}
bool CTestStreams::Check()  
{
	return TRUE;
}
bool CTestStreams::CleanUp() 
{
	delete m_pStreamRead;
	delete m_pDocument;

	return TRUE;
}


#include "TestDocument.h"

using namespace SCF;

CTestDocument::CTestDocument(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of XML document"), &rErrorStream)
{
	m_pDocument = NULL;
	m_pEnumerator = NULL;
}
CTestDocument::~CTestDocument()
{

}

bool CTestDocument::Prepare() 
{
	m_pDocument = new CXMLDocumentFile(*(new CFile(STRING("Data\\XML\\Test.xml"))));
	m_pEnumerator = new CXMLEnumerator(*m_pDocument);

	return TRUE;
}

bool CTestDocument::Run()    
{ 
	while (m_pEnumerator->Next())
	{
		for (UINT i = 0; i <= m_pEnumerator->Current()->Level(); i++)
		{ 
			CError::Stream()->PutString(STRING("  "));
		}
		CError::Stream()->PutLine(m_pEnumerator->Current()->Name());
	}

	return m_pDocument->Write(*(new CFile(STRING("Data\\XML\\Written.xml"))));
}

bool CTestDocument::Check()  
{
	return TRUE;
}
bool CTestDocument::CleanUp() 
{
	delete m_pEnumerator;
	delete m_pDocument;

	return TRUE;
}


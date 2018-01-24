#include "TestPath.h"

CTestPath::CTestPath(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of XML paths"), &rErrorStream)
{
	m_pDocument = nullptr;
	m_pPath     = nullptr;
}
CTestPath::~CTestPath()
{

}

bool CTestPath::Prepare() 
{
	m_pDocument = new CXMLDocumentFile(*(new CFile(STRING("Data\\XML\\Test.xml"))));
	m_pPath = new CXMLPath(STRING("materials//Rendering:Material"));

	return true;
}

void CTestPath::ProcessResults(_IN UINT uiExpectedResultCount)
{
	CList<CXMLNode> Results;
	m_pPath->Match(*m_pDocument, Results);

	if (uiExpectedResultCount != Results.Size()) 
	{
		
	}

	CError::Stream()->PutLine();
	CError::Stream()->PutString(STRING("XML path: \""));
	CError::Stream()->PutString(m_pPath->Expression());
	CError::Stream()->PutLine(STRING("\" returned:"));
	CError::Stream()->PutLine();
	{
		CInt i(0);
		CEnumeratorList<CXMLNode> EnumeratorResults(Results);
		while (EnumeratorResults.Next()) 
		{ 
			i += 1;
			CError::Stream()->PutString(i.ToString());
			CError::Stream()->PutString(STRING(": "));
			CError::Stream()->PutLine(EnumeratorResults.Current()->ToString()); 
		} 
	}
}

bool CTestPath::Run()    
{ 
	ProcessResults(9);

	m_pPath->Expression(STRING("/materials/Rendering:Material"));
	ProcessResults(9);

	m_pPath->Expression(STRING("//Rendering:Material"));
	ProcessResults(9);

	m_pPath->Expression(STRING("materials//Rendering:Material//@*"));
	ProcessResults(186);

	m_pPath->Expression(STRING("materials//Rendering:Material/@*"));
	ProcessResults(67);

	m_pPath->Expression(STRING("//*"));
	ProcessResults(102);

	m_pPath->Expression(STRING("//@*"));
	ProcessResults(186);

	m_pPath->Expression(STRING("//@count"));
	ProcessResults(27);

	m_pPath->Expression(STRING("//usage/.."));
	ProcessResults(9);

	m_pPath->Expression(STRING("//usages/usage[2]"));
	ProcessResults(9);

	//TODO: These 3 types of paths
	m_pPath->Expression(STRING("//usages[@count=2]"));
	ProcessResults(9);

	m_pPath->Expression(STRING("//texture[@source='material.diffuseColor']"));
	ProcessResults(16);
	
	m_pPath->Expression(STRING("//texture[@source='material.diffuseColor' and @id='Smoke_Diffuse']"));
	ProcessResults(2);

	m_pPath->Expression(STRING("//texture[(@source='material.diffuseColor') and (@id='Smoke_Diffuse')]"));
	ProcessResults(2);

	m_pPath->Expression(STRING("//texture[(@source='material.diffuseColor' or @source='material.normalMap') and (@id='Smoke_Diffuse' or @id='Wood_Normal_Map')]"));
	ProcessResults(4);

	m_pPath->Expression(STRING("//texture[(((@source='material.diffuseColor') or (@source='material.normalMap'))) and (((@id='Smoke_Diffuse') or (@id='Wood_Normal_Map')))]"));
	ProcessResults(4);

	return true;
}

bool CTestPath::Check()  
{
	
	return true;
}
bool CTestPath::CleanUp() 
{
	delete m_pDocument;
	delete m_pPath;

	return true;
}


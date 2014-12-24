#include "TestDOM.h"

using namespace SCF;

CTestDOM::CTestDOM(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of XML Document Object Model"), &rErrorStream)
{
	m_pRoot = NULL;
	m_pEnumerator = NULL;
}
CTestDOM::~CTestDOM()
{

}

bool CTestDOM::Prepare() 
{
	m_pRoot = new CXMLElement();
	m_pEnumerator = new CXMLEnumerator(*m_pRoot);

	return TRUE;
}

bool CTestDOM::Run()    
{
	m_pRoot->QName(STRING("root"));

	CXMLNode* pNode1 = new CXMLElement();
	CXMLNode* pNode2 = new CXMLElement();
	CXMLNode* pNode3 = new CXMLElement();
	CXMLNode* pNode4 = new CXMLElement();
	CXMLNode* pNode5 = new CXMLElement();

	m_pRoot->ChildAdd(*pNode1);
	m_pRoot->ChildAdd(*pNode2);
	m_pRoot->ChildAdd(*pNode3);
	m_pRoot->ChildAdd(*pNode4);
	m_pRoot->ChildAdd(*pNode5);

	pNode1->QName(STRING("namespace1:node1"), 10);
	pNode2->QName(STRING("namespace2"), STRING("node2"));

	pNode3->Prefix(STRING("ns3"));
	pNode3->Prefix(STRING("namespaceRenamed3"));
	pNode3->Name(STRING("node3"));
	pNode3->Prefix(STRING("namespace3"));

	pNode4->QName(STRING("namespace4:node4"), 10);
	pNode4->Prefix(STRING(""));
	pNode4->Prefix(STRING("ns4"));
	pNode4->Prefix(STRING(""));
	pNode4->Name(STRING("nodeRenamed4"));

	pNode5->QName(STRING("namespace5:node5"), 10);
	pNode5->Prefix(STRING("namespaceRenamed5"));
	pNode5->Name(STRING("nr5"));

	//while (m_pEnumerator->Next())
	//{
	//	for (UINT i = 0; i <= m_pEnumerator->Current()->Level(); i++)
	//	{ 
	//		CError::Stream()->PutString(STRING("  "));
	//	}
	//	CError::Stream()->PutLine(m_pEnumerator->Current()->QName());
	//}

	return TRUE;
}

bool CTestDOM::Check()  
{
	CXMLNode* pNode1 = m_pRoot->ChildNamed(STRING("namespace1:node1"));
	CXMLNode* pNode2 = m_pRoot->ChildNamed(STRING("namespace2:node2"));
	CXMLNode* pNode3 = m_pRoot->ChildNamed(STRING("namespace3:node3"));
	CXMLNode* pNode4 = m_pRoot->ChildNamed(STRING("nodeRenamed4"));
	CXMLNode* pNode5 = m_pRoot->ChildNamed(STRING("namespaceRenamed5:nr5"));

	if (!pNode1 || !pNode2 || !pNode3 || !pNode4 || !pNode5) { return FALSE; }

	if (!(pNode1->Prefix() == STRING("namespace1"))) { return FALSE; }
	if (!(pNode1->Name() == STRING("node1")))      { return FALSE; }

	if (!(pNode2->Prefix() == STRING("namespace2"))) { return FALSE; }
	if (!(pNode2->Name() == STRING("node2")))      { return FALSE; }

	if (!(pNode3->Prefix() == STRING("namespace3"))) { return FALSE; }
	if (!(pNode3->Name() == STRING("node3")))      { return FALSE; }

	if (!(pNode4->Prefix() == STRING("")))             { return FALSE; }
	if (!(pNode4->Name() == STRING("nodeRenamed4"))) { return FALSE; }

	if (!(pNode5->Prefix() == STRING("namespaceRenamed5"))) { return FALSE; }
	if (!(pNode5->Name() == STRING("nr5")))               { return FALSE; }

	return TRUE;
}
bool CTestDOM::CleanUp() 
{
	delete m_pEnumerator;
	delete m_pRoot;

	return TRUE;
}


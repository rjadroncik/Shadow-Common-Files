#include "Path.h"
#include "Private/PathParser.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPath::CXMLPath()
{
	m_pPathStepFirst = NULL;
}

CXMLPath::CXMLPath(_IN CString& rExpression)
{
	m_pPathStepFirst = NULL;
	Expression(rExpression);
}

CXMLPath::~CXMLPath()
{
	if (m_pPathStepFirst) { delete m_pPathStepFirst; }
}

void CXMLPath::Expression(_IN CString& rExpression) _SET
{
	m_Expression = rExpression;

	if (m_pPathStepFirst) { delete m_pPathStepFirst; }

	CXMLPathParser Parser;
	m_pPathStepFirst = Parser.Parse(rExpression);
}

bool CXMLPath::Match(_IN CXMLNode& rRoot, _OUT CList<CXMLNode>& rOutList)
{
	CList<CXMLNode> InList;
	CList<CXMLNode> OutList;

	InList.LastAdd(rRoot);

	CList<CXMLNode>* pInList  = &InList;
	CList<CXMLNode>* pOutList = &OutList;

	CXMLPathStep* pStep = m_pPathStepFirst;
	while (pStep)
	{
		CEnumeratorList<CXMLNode> EnumeratorInList(*pInList);
		while (EnumeratorInList.Next())
		{
			pStep->Match(*(CXMLNode*)(EnumeratorInList.Current()), *pOutList);
		}

		pInList->AllRemove();
	
		//Swap in & out containers for next step
		CList<CXMLNode>* pTmp = pInList;
		pInList = pOutList;
		pOutList = pTmp;

		pStep = pStep->Next();
	}	

	//Containers swapped so [pInList] contains the last result
	CEnumeratorList<CXMLNode> EnumeratorOutList(*pInList);
	rOutList.AllAdd(EnumeratorOutList);

	return TRUE;
}
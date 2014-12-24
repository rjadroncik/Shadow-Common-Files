#include "Path.h"
#include "Private/PathParser.h"

using namespace SCF;
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

bool CXMLPath::Match(_IN SCFXML::CXMLNode& rRoot, _OUT CList<SCFXML::CXMLNode>& rOutList)
{
	CList<SCFXML::CXMLNode> InList;
	CList<SCFXML::CXMLNode> OutList;

	InList.LastAdd(rRoot);

	CList<SCFXML::CXMLNode>* pInList  = &InList;
	CList<SCFXML::CXMLNode>* pOutList = &OutList;

	CXMLPathStep* pStep = m_pPathStepFirst;
	while (pStep)
	{
		CEnumeratorList EnumeratorInList(*pInList);
		while (EnumeratorInList.Next())
		{
			pStep->Match(*(CXMLNode*)(EnumeratorInList.Current()), *pOutList);
		}

		pInList->AllRemove();
	
		//Swap in & out containers for next step
		CList<SCFXML::CXMLNode>* pTmp = pInList;
		pInList = pOutList;
		pOutList = pTmp;

		pStep = pStep->Next();
	}	

	//Containers swapped so [pInList] contains the last result
	CEnumeratorList EnumeratorOutList(*pInList);
	rOutList.AllAdd(EnumeratorOutList);

	return TRUE;
}
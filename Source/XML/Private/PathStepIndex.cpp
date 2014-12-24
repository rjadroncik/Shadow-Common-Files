#include "PathStepIndex.h"
#include "../Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepIndex::CXMLPathStepIndex()
{
}

CXMLPathStepIndex::~CXMLPathStepIndex()
{
}

void CXMLPathStepIndex::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	register const CXMLNode* pNode = &rCurrent;
	register int iIndex = 1;

	//Go to the x-th child starting with 1
	while (pNode && iIndex < m_iIndex) { pNode = pNode->Next(); iIndex++; }

	if (pNode) { rOutList.LastAdd(*pNode); }
}

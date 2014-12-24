#include "PathStepChildrenAll.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepChildrenAll::CXMLPathStepChildrenAll()
{
}

CXMLPathStepChildrenAll::~CXMLPathStepChildrenAll()
{
}

void CXMLPathStepChildrenAll::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	register CXMLNode* pNode = rCurrent.ChildFirst();

	//Go through all children & add them all
	while (pNode) { rOutList.LastAdd(*pNode); pNode = pNode->Next(); }
}

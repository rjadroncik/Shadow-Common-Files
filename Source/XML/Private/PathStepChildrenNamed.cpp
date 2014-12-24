#include "PathStepChildrenNamed.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepChildrenNamed::CXMLPathStepChildrenNamed()
{
}

CXMLPathStepChildrenNamed::~CXMLPathStepChildrenNamed()
{
}

void CXMLPathStepChildrenNamed::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	register CXMLNode* pNode = rCurrent.ChildFirst();

	//Go through all children  & compare names
	while (pNode)
	{
		if (pNode->QName() == m_QName) { rOutList.LastAdd(*pNode); }

		pNode = pNode->Next();
	}
}

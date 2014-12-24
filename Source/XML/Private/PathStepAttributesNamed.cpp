#include "PathStepAttributesNamed.h"
#include "../Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepAttributesNamed::CXMLPathStepAttributesNamed()
{
}

CXMLPathStepAttributesNamed::~CXMLPathStepAttributesNamed()
{
}

void CXMLPathStepAttributesNamed::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	//Go through all children & compare names
	CXMLNode* pNode = rCurrent.ChildFirst();
	while (pNode)
	{
		if (pNode->ClassKey() == ClassXMLElement)
		{
			CXMLAttribute* pAttribute = ((CXMLElement*)pNode)->AttributeFirst();
			while (pAttribute)
			{
				if (pAttribute->Name() == m_QName) { rOutList.LastAdd(*pAttribute); }

				pAttribute = pAttribute->Next();
			}
		}

		pNode = pNode->Next();
	}
}

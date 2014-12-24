#include "PathStepAttributesAll.h"
#include "../Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepAttributesAll::CXMLPathStepAttributesAll()
{
}

CXMLPathStepAttributesAll::~CXMLPathStepAttributesAll()
{
}

void CXMLPathStepAttributesAll::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	if (rCurrent.ClassKey() == ClassXMLElement)
	{
		CXMLAttribute* pAttribute = ((CXMLElement&)rCurrent).AttributeFirst();
		while (pAttribute)
		{
			rOutList.LastAdd(*pAttribute);

			pAttribute = pAttribute->Next();
		}
	}
}

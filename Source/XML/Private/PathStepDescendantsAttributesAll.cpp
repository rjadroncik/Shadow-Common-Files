#include "PathStepDescendantsAttributesAll.h"
#include "../Enumerator.h"
#include "../Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepDescendantsAttributesAll::CXMLPathStepDescendantsAttributesAll()
{
}

CXMLPathStepDescendantsAttributesAll::~CXMLPathStepDescendantsAttributesAll()
{
}

void CXMLPathStepDescendantsAttributesAll::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	//Create descendants enumerator (DONT skip root)
	CXMLEnumerator Enumerator(rCurrent);
//	Enumerator.Next();

	while (Enumerator.Next())
	{
		if (Enumerator.Current()->Type() == XmlElement)
		{
			CXMLAttribute* pAttribute = ((CXMLElement*)Enumerator.Current())->AttributeFirst();
			while (pAttribute)
			{
				rOutList.LastAdd(*pAttribute);

				pAttribute = pAttribute->Next();
			}
		}
	}
}
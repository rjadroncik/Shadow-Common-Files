#include "PathStepDescendantsAttributesNamed.h"
#include "../Enumerator.h"
#include "../Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepDescendantsAttributesNamed::CXMLPathStepDescendantsAttributesNamed()
{
}

CXMLPathStepDescendantsAttributesNamed::~CXMLPathStepDescendantsAttributesNamed()
{
}

void CXMLPathStepDescendantsAttributesNamed::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	//Create descendants enumerator (DONT skip root)
	CXMLEnumerator Enumerator(rCurrent);
//	Enumerator.Next();

	while (Enumerator.Next())
	{
		if (Enumerator.Current()->ClassKey() == ClassXMLElement)
		{
			CXMLAttribute* pAttribute = ((CXMLElement*)Enumerator.Current())->AttributeFirst();
			while (pAttribute)
			{
				if (pAttribute->Name() == m_QName) { rOutList.LastAdd(*pAttribute); }

				pAttribute = pAttribute->Next();
			}
		}
	}
}
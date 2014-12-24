#include "PathStepDescendantsAttributeValued.h"
#include "Enumerator.h"
#include "Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepDescendantsAttributeValued::CXMLPathStepDescendantsAttributeValued()
{
}

CXMLPathStepDescendantsAttributeValued::~CXMLPathStepDescendantsAttributeValued()
{
}

bool CXMLPathStepDescendantsAttributeValued::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList& rOutList) const
{
	//Create descendants enumerator (skip root)
	CXMLEnumerator Enumerator(rCurrent);
	Enumerator.Next();

	while (Enumerator.Next())
	{
		if (Enumerator.Current()->ClassKey() == ClassXMLElement)
		{
			CXMLAttribute* pAttribute = ((CXMLElement*)Enumerator.Current())->AttributeFirst();
			while (pAttribute)
			{
				if (pAttribute->Name() == m_QName) 
				{
					if (pAttribute->ValueString() == m_Value) { rOutList.LastAdd(*Enumerator.Current()); }
					break; //We can end attribute looping once we find the attribute with the given name regardless of whether it has the sought value
				}

				pAttribute = pAttribute->Next();
			}
		}
	}

	return (!rOutList.IsEmpty());
}
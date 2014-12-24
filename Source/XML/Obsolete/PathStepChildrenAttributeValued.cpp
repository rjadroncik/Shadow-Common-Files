#include "PathStepChildrenAttributeValued.h"
#include "Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepChildrenAttributeValued::CXMLPathStepChildrenAttributeValued()
{
}

CXMLPathStepChildrenAttributeValued::~CXMLPathStepChildrenAttributeValued()
{
}

bool CXMLPathStepChildrenAttributeValued::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList& rOutList) const
{
	register CXMLNode* pNode = rCurrent.ChildFirst();

	//Go through all children & compare attributes
	while (pNode)
	{
		if (pNode->ClassKey() == ClassXMLElement)
		{
			CXMLAttribute* pAttribute = ((CXMLElement*)pNode)->AttributeFirst();
			while (pAttribute)
			{
				if (pAttribute->Name() == m_QName) 
				{
					if (pAttribute->ValueString() == m_Value) { rOutList.LastAdd(*pNode); }
					break; //We can end attribute looping once we find the attribute with the given name regardless of whether it has the sought value
				}

				pAttribute = pAttribute->Next();
			}
		}

		pNode = pNode->Next();
	}

	return (!rOutList.IsEmpty());
}

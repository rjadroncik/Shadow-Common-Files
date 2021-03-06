#include "BoundValueChildNode.h"

#include "../Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

extern bool CharIsWhiteSpace(_IN TCHAR cChar);

CValue* BoundValueChildNode_pValue;

CXMLBoundValueChildNode::CXMLBoundValueChildNode()
{
}

CXMLBoundValueChildNode::~CXMLBoundValueChildNode()
{
	if (BoundValueChildNode_pValue) { delete BoundValueChildNode_pValue; BoundValueChildNode_pValue = nullptr; }

}

const CValue* CXMLBoundValueChildNode::Evaluate(_IN SCFXML::CXMLNode& rCurrent) const
{
	if (BoundValueChildNode_pValue) { delete BoundValueChildNode_pValue; BoundValueChildNode_pValue = nullptr; }

	if (rCurrent.Type() == XmlElement)
	{
		CXMLNode* pChild = ((CXMLElement&)rCurrent).ChildFirst();
		while (pChild)
		{
			if (pChild->Name() == m_Name)
			{
				UINT uiCharsParsed = 0;

				CString valueString = pChild->Value() ? pChild->Value()->ToString() : "";

				BoundValueChildNode_pValue = &CValue::Parse(valueString, &uiCharsParsed);

				for (UINT i = uiCharsParsed; i < valueString.Length(); i++)
				{
					if (!CharIsWhiteSpace(valueString[i])) {

						if (BoundValueChildNode_pValue) { delete BoundValueChildNode_pValue; BoundValueChildNode_pValue = nullptr; }

						BoundValueChildNode_pValue = new STRING_RETURN(valueString); 
					}
				}

				return BoundValueChildNode_pValue;
			}
		}
	}

	return nullptr;
}

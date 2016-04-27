#include "BoundValueChildNode.h"

#include "../Attribute.h"

using namespace SCF;
using namespace SCFXML;
using namespace SCFXMLPrivate;

extern bool CharIsWhiteSpace(_IN SCF::TCHAR cChar);

CValue* BoundValueChildNode_pValue;

CXMLBoundValueChildNode::CXMLBoundValueChildNode()
{
}

CXMLBoundValueChildNode::~CXMLBoundValueChildNode()
{
	if (BoundValueChildNode_pValue) { delete BoundValueChildNode_pValue; BoundValueChildNode_pValue = NULL; }

}

const CValue* CXMLBoundValueChildNode::Evaluate(_IN SCFXML::CXMLNode& rCurrent) const
{
	if (BoundValueChildNode_pValue) { delete BoundValueChildNode_pValue; BoundValueChildNode_pValue = NULL; }

	if (rCurrent.ClassKey() == ClassXMLElement)
	{
		CXMLNode* pChild = ((CXMLElement&)rCurrent).ChildFirst();
		while (pChild)
		{
			if (pChild->Name() == m_Name)
			{
				SCF::UINT uiCharsParsed = 0;

				CString valueString = pChild->Value() ? pChild->Value()->ToString() : "";

				BoundValueChildNode_pValue = &CValue::Parse(valueString, &uiCharsParsed);

				for (SCF::UINT i = uiCharsParsed; i < valueString.Length(); i++)
				{
					if (!CharIsWhiteSpace(valueString[i])) {

						if (BoundValueChildNode_pValue) { delete BoundValueChildNode_pValue; BoundValueChildNode_pValue = NULL; }

						BoundValueChildNode_pValue = new STRING_RETURN(valueString); 
					}
				}

				return BoundValueChildNode_pValue;
			}
		}
	}

	return NULL;
}

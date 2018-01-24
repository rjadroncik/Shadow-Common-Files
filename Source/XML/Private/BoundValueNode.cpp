#include "BoundValueNode.h"

#include "../Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

extern bool CharIsWhiteSpace(_IN TCHAR cChar);

CValue* BoundValueNode_pValue;

CXMLBoundValueNode::CXMLBoundValueNode()
{
}

CXMLBoundValueNode::~CXMLBoundValueNode()
{
	if (BoundValueNode_pValue) { delete BoundValueNode_pValue; BoundValueNode_pValue = nullptr; }
}

const CValue* CXMLBoundValueNode::Evaluate(_IN SCFXML::CXMLNode& rCurrent) const
{
	if (BoundValueNode_pValue) { delete BoundValueNode_pValue; BoundValueNode_pValue = nullptr; }

	UINT uiCharsParsed = 0;

	CString valueString = rCurrent.Value() ? rCurrent.Value()->ToString() : "";

	BoundValueNode_pValue = &CValue::Parse(valueString, &uiCharsParsed);

	for (UINT i = uiCharsParsed; i < valueString.Length(); i++)
	{
		if (!CharIsWhiteSpace(valueString[i])) {
			
			if (BoundValueNode_pValue) { delete BoundValueNode_pValue; BoundValueNode_pValue = nullptr; }

			BoundValueNode_pValue = new STRING_RETURN(valueString); 
		}
	}

	return BoundValueNode_pValue;
}

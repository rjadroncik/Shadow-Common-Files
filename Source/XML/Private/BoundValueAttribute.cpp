#include "BoundValueAttribute.h"

#include "../Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLBoundValueAttribute::CXMLBoundValueAttribute()
{
}

CXMLBoundValueAttribute::~CXMLBoundValueAttribute()
{
}

const CValue* CXMLBoundValueAttribute::Evaluate(_IN SCFXML::CXMLNode& rCurrent) const
{
	if (rCurrent.Type() == XmlElement)
	{
		CXMLAttribute* pAttribute = ((CXMLElement&)rCurrent).AttributeNamed(m_Name);
		if (pAttribute)
		{
			return pAttribute->Value();
		}
	}

	return NULL;
}

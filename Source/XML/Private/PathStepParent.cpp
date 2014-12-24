#include "PathStepParent.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepParent::CXMLPathStepParent()
{
}

CXMLPathStepParent::~CXMLPathStepParent()
{
}

void CXMLPathStepParent::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	_ASSERT(rCurrent.Parent());

	if (!rOutList.Contains(*rCurrent.Parent())) { rOutList.LastAdd(*rCurrent.Parent()); }
}

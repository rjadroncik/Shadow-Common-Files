#include "PathStepDescendantsAll.h"
#include "../Enumerator.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepDescendantsAll::CXMLPathStepDescendantsAll()
{
}

CXMLPathStepDescendantsAll::~CXMLPathStepDescendantsAll()
{
}

void CXMLPathStepDescendantsAll::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	//Create descendants enumerator (skip root)
	CXMLEnumerator Enumerator(rCurrent);
	Enumerator.Next();

	while (Enumerator.Next()) { rOutList.LastAdd(*Enumerator.Current()); }
}
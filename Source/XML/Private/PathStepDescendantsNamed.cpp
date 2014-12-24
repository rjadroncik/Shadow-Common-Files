#include "PathStepDescendantsNamed.h"
#include "../Enumerator.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepDescendantsNamed::CXMLPathStepDescendantsNamed()
{
}

CXMLPathStepDescendantsNamed::~CXMLPathStepDescendantsNamed()
{
}

void CXMLPathStepDescendantsNamed::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	//Create descendants enumerator (skip root)
	CXMLEnumerator Enumerator(rCurrent);
	Enumerator.Next();

	while (Enumerator.Next())
	{
		if (Enumerator.Current()->QName() == m_QName) { rOutList.LastAdd(*Enumerator.Current()); }
	}
}
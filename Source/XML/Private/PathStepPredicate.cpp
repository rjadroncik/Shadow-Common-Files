#include "PathStepPredicate.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathStepPredicate::CXMLPathStepPredicate(_IN CXMLPathPredicate* pPredicateRoot)
{
	m_pPredicateRoot = pPredicateRoot;
}

CXMLPathStepPredicate::~CXMLPathStepPredicate()
{
	if (m_pPredicateRoot) { delete m_pPredicateRoot; }
}

void CXMLPathStepPredicate::Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const
{
	if (rCurrent.Type() == XmlElement)
	{
		if (m_pPredicateRoot->Match(rCurrent))
		{
			rOutList.LastAdd(rCurrent);
		}
	}
}

#pragma once

#include "PathStepNamed.h"
#include "PathPredicate.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepPredicate : public CXMLPathStep
	{
	public:
		CXMLPathStepPredicate(_IN CXMLPathPredicate* pPredicateRoot);
		virtual ~CXMLPathStepPredicate();

	public:
		inline void                     PredicateRoot(_IN CXMLPathPredicate& rPredicate) _SET { m_pPredicateRoot = &rPredicate; }
		inline const CXMLPathPredicate* PredicateRoot()                                  _GET { return m_pPredicateRoot; }

	public:
		void Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList<SCFXML::CXMLNode>& rOutList) const;

	protected:
		const CXMLPathPredicate* m_pPredicateRoot;
	};
};

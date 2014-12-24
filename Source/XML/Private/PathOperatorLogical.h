#pragma once

#include "PathPredicate.h"

namespace SCFXMLPrivate
{
	class CXMLPathOperatorLogical : public CXMLPathPredicate
	{
	public:
		SCF::ENUM ClassKey() _GET { return (SCF::ENUM)ClassXMLPathOperatorLogical; }

	public:
		typedef bool (__thiscall SCFXMLPrivate::CXMLPathOperatorLogical::*COMPARE_FUNCTION) (_IN SCFXML::CXMLNode&, _IN CXMLPathPredicate&, _IN CXMLPathPredicate&) const;

	public:
		CXMLPathOperatorLogical(_IN COMPARE_FUNCTION compareFunction);
		virtual ~CXMLPathOperatorLogical();

	public:
		inline void                     PredicateLeft(_IN CXMLPathPredicate& rPredicate) _SET { m_pPredicateLeft = &rPredicate; }
		inline const CXMLPathPredicate* PredicateLeft()                                  _GET { return m_pPredicateLeft; }

	public:
		inline void                     PredicateRight(_IN CXMLPathPredicate& rPredicate) _SET { m_pPredicateRight = &rPredicate; }
		inline const CXMLPathPredicate* PredicateRight()                                  _GET { return m_pPredicateRight; }

	public:
		inline void             CompareFunction(_IN COMPARE_FUNCTION compareFunction) _SET  { m_CompareFunction = compareFunction; }
		inline COMPARE_FUNCTION CompareFunction()                                     _GET  { return m_CompareFunction; }

	public:
		bool And(_IN SCFXML::CXMLNode& rCurrent, _IN CXMLPathPredicate& r1st, _IN CXMLPathPredicate& r2nd) const;
		bool Or (_IN SCFXML::CXMLNode& rCurrent, _IN CXMLPathPredicate& r1st, _IN CXMLPathPredicate& r2nd) const;

	public:
		bool Match(_IN SCFXML::CXMLNode& rNode) const;

	private:
		const CXMLPathPredicate* m_pPredicateLeft;
		const CXMLPathPredicate* m_pPredicateRight;

	private:
		COMPARE_FUNCTION m_CompareFunction;
	};
};

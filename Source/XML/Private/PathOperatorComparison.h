#pragma once

#include "PathPredicate.h"
#include "BoundValue.h"

namespace SCFXMLPrivate
{
	class CXMLPathOperatorComparison : public CXMLPathPredicate
	{
	public:
		SCF::ENUM ClassKey() _GET { return (SCF::ENUM)ClassXMLPathOperatorComparison; }

	public:
		typedef bool (__thiscall SCFXMLPrivate::CXMLPathOperatorComparison::*COMPARE_FUNCTION) (_IN SCFXML::CXMLNode&, _IN CXMLBoundValue&, _IN CXMLBoundValue&) const;

	public:
		CXMLPathOperatorComparison(_IN COMPARE_FUNCTION compareFunction, _IN CComparer& rComparer);
		virtual ~CXMLPathOperatorComparison();

	public:
		inline void                  ValueLeft(_IN CXMLBoundValue& rValue) _SET { m_pValueLeft = &rValue; }
		inline const CXMLBoundValue* ValueLeft()                           _GET { return m_pValueLeft; }

	public:
		inline void                  ValueRight(_IN CXMLBoundValue& rValue) _SET { m_pValueRight = &rValue; }
		inline const CXMLBoundValue* ValueRight()                           _GET { return m_pValueRight; }

	public:
		inline void             Comparer(_IN CComparer* pComparer) _SET { m_pComparer = pComparer; }
		inline const CComparer* Comparer()                         _GET { return m_pComparer; }

	public:
		inline void             CompareFunction(_IN COMPARE_FUNCTION compareFunction) _SET  { m_CompareFunction = compareFunction; }
		inline COMPARE_FUNCTION CompareFunction()                                     _GET  { return m_CompareFunction; }

	public:
		bool Less          (_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const;
		bool LessOrEqual   (_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const;
		bool Equal         (_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const;

		bool Greater       (_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const;
		bool GreaterOrEqual(_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const;
		bool NotEqual      (_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const;

	public:
		bool Match(_IN SCFXML::CXMLNode& rNode) const;

	private:
		const CXMLBoundValue* m_pValueLeft;
		const CXMLBoundValue* m_pValueRight;

	private:
		const CComparer* m_pComparer;
		COMPARE_FUNCTION m_CompareFunction;
	};
};

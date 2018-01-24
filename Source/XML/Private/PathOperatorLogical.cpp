#include "PathOperatorLogical.h"
#include "../Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathOperatorLogical::CXMLPathOperatorLogical(_IN COMPARE_FUNCTION compareFunction)
{
	m_CompareFunction = compareFunction;

	m_pPredicateLeft = nullptr;
	m_pPredicateRight = nullptr;
}

CXMLPathOperatorLogical::~CXMLPathOperatorLogical()
{
	if (m_pPredicateLeft)  { delete m_pPredicateLeft; }
	if (m_pPredicateRight) { delete m_pPredicateRight; }
}

bool CXMLPathOperatorLogical::And(_IN SCFXML::CXMLNode& rCurrent, _IN CXMLPathPredicate& r1st, _IN CXMLPathPredicate& r2nd) const
{
	return r1st.Match(rCurrent) && r2nd.Match(rCurrent);
}

bool CXMLPathOperatorLogical::Or (_IN SCFXML::CXMLNode& rCurrent, _IN CXMLPathPredicate& r1st, _IN CXMLPathPredicate& r2nd) const
{
	return r1st.Match(rCurrent) || r2nd.Match(rCurrent);
}

bool CXMLPathOperatorLogical::Match(_IN SCFXML::CXMLNode& rNode) const
{
	if (rNode.Type() == XmlElement)
	{
		//Call one of the comparison functions which will perform the comparison using the supplied comparer as it was parsed by the parser
		//Comparison functions - decide which compare operator is emulated
		//Comparer - decides how to handle to comparing itself - i.e. compare as integers, floats, strings + performs type conversion as necessary
		return CALL_MEMBER(*this, m_CompareFunction)(rNode, *m_pPredicateLeft, *m_pPredicateRight);
	}

	return false; 
}

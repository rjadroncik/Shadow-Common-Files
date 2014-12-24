#include "PathOperatorComparison.h"
#include "../Attribute.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

CXMLPathOperatorComparison::CXMLPathOperatorComparison(_IN COMPARE_FUNCTION compareFunction, _IN CComparer& rComparer)
{
	m_pComparer = &rComparer;
	m_CompareFunction = compareFunction;
}

CXMLPathOperatorComparison::~CXMLPathOperatorComparison()
{
	if (m_pValueLeft) { delete m_pValueLeft; }
	if (m_pValueRight) { delete m_pValueRight; }

	delete m_pComparer;
}

bool CXMLPathOperatorComparison::Less(_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const 
{ 
	register const CValue* p1stValue = r1st.Evaluate(rCurrent);
	register const CValue* p2ndValue = r2nd.Evaluate(rCurrent);

	if (!p1stValue || !p2ndValue) { return FALSE; }

	return m_pComparer->LessOrEqual(*p1stValue, *p2ndValue) && !m_pComparer->Equal(*p1stValue, *p2ndValue);
}

bool CXMLPathOperatorComparison::LessOrEqual(_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const 
{ 
	register const CValue* p1stValue = r1st.Evaluate(rCurrent);
	register const CValue* p2ndValue = r2nd.Evaluate(rCurrent);

	if (!p1stValue || !p2ndValue) { return FALSE; }

	return m_pComparer->LessOrEqual(*p1stValue, *p2ndValue); 
}

bool CXMLPathOperatorComparison::Equal(_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const 
{ 
	register const CValue* p1stValue = r1st.Evaluate(rCurrent);
	register const CValue* p2ndValue = r2nd.Evaluate(rCurrent);

	if (!p1stValue && !p2ndValue) { return TRUE; }
	if (!p1stValue || !p2ndValue) { return FALSE; }

	return m_pComparer->Equal(*p1stValue, *p2ndValue); 
}

bool CXMLPathOperatorComparison::Greater(_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const
{ 
	return !LessOrEqual(rCurrent, r1st, r2nd);
}
bool CXMLPathOperatorComparison::GreaterOrEqual(_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const 
{ 
	return !Less(rCurrent, r1st, r2nd);
}
bool CXMLPathOperatorComparison::NotEqual(_IN SCFXML::CXMLNode& rCurrent, _IN CXMLBoundValue& r1st, _IN CXMLBoundValue& r2nd) const 
{ 
	return !Equal(rCurrent, r1st, r2nd);
}

bool CXMLPathOperatorComparison::Match(_IN SCFXML::CXMLNode& rNode) const
{
	if (rNode.ClassKey() == ClassXMLElement)
	{
		//Call one of the comparison functions which will perform the comparison using the supplied comparer as it was parsed by the parser
		//Comparison functions - decide which compare operator is emulated
		//Comparer - decides how to handle to comparing itself - i.e. compare as integers, floats, strings + performs type conversion as necessary
		return CALL_MEMBER(*this, m_CompareFunction)(rNode, *m_pValueLeft, *m_pValueRight);
	}

	return FALSE; 
}

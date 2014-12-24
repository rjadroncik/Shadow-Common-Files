#include "BoundValueNumber.h"

using namespace SCF;
using namespace SCFXML;
using namespace SCFXMLPrivate;

extern bool CharIsWhiteSpace(_IN SCF::TCHAR cChar);

CXMLBoundValueNumber::CXMLBoundValueNumber()
{
}

CXMLBoundValueNumber::~CXMLBoundValueNumber()
{
	if (m_pValue) { delete m_pValue; }
}

void CXMLBoundValueNumber::Value(_IN CString& rValue) _SET
{
	m_pValue = &CValue::Parse(rValue, NULL);
}

const CValue* CXMLBoundValueNumber::Evaluate(_IN SCFXML::CXMLNode& rCurrent) const
{
	SCF_UNREFERENCED_PARAMETER(rCurrent);
	return m_pValue;
}
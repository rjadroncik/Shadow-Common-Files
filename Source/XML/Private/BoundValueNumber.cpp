#include "BoundValueNumber.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

extern bool CharIsWhiteSpace(_IN TCHAR cChar);

CXMLBoundValueNumber::CXMLBoundValueNumber()
{
}

CXMLBoundValueNumber::~CXMLBoundValueNumber()
{
	if (m_pValue) { delete m_pValue; }
}

void CXMLBoundValueNumber::Value(_IN CString& rValue) _SET
{
	m_pValue = &CValue::Parse(rValue, nullptr);
}

const CValue* CXMLBoundValueNumber::Evaluate(_IN SCFXML::CXMLNode& rCurrent) const
{
	SCF_UNREFERENCED_PARAMETER(rCurrent);
	return m_pValue;
}
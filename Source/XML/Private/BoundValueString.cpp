#include "BoundValueString.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

extern bool CharIsWhiteSpace(_IN TCHAR cChar);

CXMLBoundValueString::CXMLBoundValueString()
{
}

CXMLBoundValueString::~CXMLBoundValueString()
{
	if (m_pValue) { delete m_pValue; }
}

void CXMLBoundValueString::Value(_IN CString& rValue) _SET
{
	m_pValue = new CString(rValue);
}

const CValue* CXMLBoundValueString::Evaluate(_IN SCFXML::CXMLNode& rCurrent) const
{
	SCF_UNREFERENCED_PARAMETER(rCurrent);
	return m_pValue;
}
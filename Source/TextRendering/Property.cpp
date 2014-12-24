#include "Property.h"

using namespace SCFBase;
using namespace SCFTextRendering;

CProperty::CProperty(_IN int iID, _IN CValue& rValue) : m_iID(iID), m_pValue((CValue*)&rValue)
{
}

CProperty::~CProperty()
{
	delete m_pValue;
}

void CProperty::Value(_IN _REF CValue& rValue) _SET 
{ 
	delete m_pValue;
	m_pValue = (CValue*)&rValue; 
}

#include "Constants.h"
#include "Style.h"

using namespace SCFTextRendering;

CStyle::CStyle(SCF::ENUM eType)
{
//Reset variables
	m_iID = 0;

	m_iBaseStyleID = 0;
	m_bHasBaseStyle = FALSE;

//Assign type
	m_eType = eType;
}

CStyle::~CStyle()
{
	m_Properties.AllDelete();
}

bool CStyle::Property(_IN int iID, _IN CValue& rValue) _SET
{
	CProperty* pProperty = (CProperty*)m_Properties.At(iID);
	if (pProperty) 
	{ 
		pProperty->Value(rValue); 
	}
	else
	{
		pProperty = new CProperty(iID, rValue);
		m_Properties.AtPut(iID, *pProperty);
	}

	return TRUE;
}

CProperty* CStyle::Property(_IN int iID) _GET
{
	return (CProperty*)m_Properties.At(iID);
}

bool CStyle::PropertyDelete(_IN int iID)
{
	CObject* pProperty = m_Properties.Remove(iID);
	if (pProperty) { delete pProperty; return TRUE; }

	return FALSE;
}

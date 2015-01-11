#include "TokenNumber.h"

using namespace SCFCompiler;

const CString CTokenNumber::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	if (m_pValue->ClassKey() == ClassInt)   { for (SCF::UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'i'); } }
	if (m_pValue->ClassKey() == ClassFloat) { for (SCF::UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'f'); } }

	return RetVal;
}

CTokenNumber::CTokenNumber(_INOUT _REF CStringRange& rText, SCF::UINT uiLine, SCF::UINT uiColumn) : CToken(rText, uiLine, uiColumn)
{
	m_pValue = &CValue::Parse(rText, NULL);
}

CTokenNumber::~CTokenNumber()
{
	delete m_pValue;
}

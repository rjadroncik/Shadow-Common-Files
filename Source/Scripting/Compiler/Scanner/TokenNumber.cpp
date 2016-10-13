#include "TokenNumber.h"

using namespace SCFCompiler;

const CString CTokenNumber::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	if (m_pValue->Kind() == ValueInt)   { for (UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'i'); } }
	if (m_pValue->Kind() == ValueFloat) { for (UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'f'); } }

	return RetVal;
}

CTokenNumber::CTokenNumber(_INOUT _REF CStringRange& rText, UINT uiLine, UINT uiColumn) 
	: CToken(rText, TokenNumber, uiLine, uiColumn)
{
	m_pValue = &CValue::Parse(rText, NULL);
}

CTokenNumber::~CTokenNumber()
{
	delete m_pValue;
}

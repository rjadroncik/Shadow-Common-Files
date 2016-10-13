#include "TokenType.h"

using namespace SCFCompiler;

const CString CTokenType::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	for (UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 't'); }

	return RetVal;
}

CTokenType::CTokenType(_INOUT _REF CStringRange& rText, ENUM eType, UINT uiLine, UINT uiColumn)
	: CToken(rText, TokenType, uiLine, uiColumn)
{
	m_eType = eType;
}

CTokenType::~CTokenType()
{
}

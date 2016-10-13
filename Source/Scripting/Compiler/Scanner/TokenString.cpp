#include "TokenString.h"

using namespace SCFCompiler;

const CString CTokenString::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	for (UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 's'); }

	return RetVal;
}

CTokenString::CTokenString(_INOUT _REF CStringRange& rText, UINT uiLine, UINT uiColumn) 
	: CToken(rText, TokenString, uiLine, uiColumn)
{
}

CTokenString::~CTokenString()
{
}

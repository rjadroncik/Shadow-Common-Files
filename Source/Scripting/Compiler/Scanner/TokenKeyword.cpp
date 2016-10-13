#include "TokenKeyword.h"

using namespace SCFCompiler;

const CString CTokenKeyword::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	for (UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'k'); }

	return RetVal;
}

CTokenKeyword::CTokenKeyword(_INOUT _REF CStringRange& rText, ENUM eKeyword, UINT uiLine, UINT uiColumn) 
	: CToken(rText, TokenKeyword, uiLine, uiColumn)
{
	m_eKeyword = eKeyword;
}

CTokenKeyword::~CTokenKeyword()
{
}

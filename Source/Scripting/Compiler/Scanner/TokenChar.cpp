#include "TokenChar.h"

using namespace SCFCompiler;

const CString CTokenChar::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	for (UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'c'); }

	return RetVal;
}

CTokenChar::CTokenChar(_INOUT _REF CStringRange& rText, UINT uiLine, UINT uiColumn) : CToken(rText, uiLine, uiColumn)
{
}

CTokenChar::~CTokenChar()
{
}

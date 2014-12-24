#include "TokenChar.h"

using namespace SCFCompiler;

const CString CTokenChar::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	for (SCF::UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'c'); }

	return RetVal;
}

CTokenChar::CTokenChar(_INOUT _REF CStringRange& rText) : CToken(rText)
{
}

CTokenChar::~CTokenChar()
{
}

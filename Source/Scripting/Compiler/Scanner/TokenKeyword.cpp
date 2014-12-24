#include "TokenKeyword.h"

using namespace SCFCompiler;

const CString CTokenKeyword::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	for (SCF::UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'k'); }

	return RetVal;
}

CTokenKeyword::CTokenKeyword(_INOUT _REF CStringRange& rText, SCF::ENUM eKeyword) : CToken(rText)
{
	m_eKeyword = eKeyword;
}

CTokenKeyword::~CTokenKeyword()
{
}

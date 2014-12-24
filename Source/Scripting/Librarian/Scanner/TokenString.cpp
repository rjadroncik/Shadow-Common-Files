#include "TokenString.h"

using namespace SCFCompiler;

CString CTokenString::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	for (SCF::UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 's'); }

	return RetVal;
}

CTokenString::CTokenString(_INOUT _REF CStringRange& rText) : CToken(rText)
{
}

CTokenString::~CTokenString()
{
}

bool CTokenString::Update()
{

	return TRUE;
}

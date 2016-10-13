#include "TokenIdentifier.h"

using namespace SCFCompiler;

const CString CTokenIdentifier::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	for (UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'w'); }

	return RetVal;
}

CTokenIdentifier::CTokenIdentifier(_INOUT _REF CStringRange& rText, UINT uiLine, UINT uiColumn) 
	: CToken(rText, TokenIdentifier, uiLine, uiColumn)
{
}

CTokenIdentifier::~CTokenIdentifier()
{
}


#include "Token.h"

using namespace SCFCompiler;


CToken::CToken(_INOUT _REF CStringRange& rText)
{
	m_pText = &rText;
}

CToken::~CToken()
{
	delete m_pText;
}

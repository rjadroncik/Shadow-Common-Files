#include "Token.h"

using namespace SCFCompiler;


CToken::CToken(_INOUT _REF CStringRange& rText, UINT uiLine, UINT uiColumn) : m_pText(&rText), m_uiLine(uiLine), m_uiColumn(uiColumn - rText.Length())
{
}

CToken::~CToken()
{
	delete m_pText;
}

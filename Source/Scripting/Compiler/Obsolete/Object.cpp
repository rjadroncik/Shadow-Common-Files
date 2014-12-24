#include "Word.h"

using namespace SCFCompiler;


CWord::CWord(_IN CStringRange& rText, _IN SCF::ENUM eType)
{
	m_pText = new CStringRange(rText);
	m_eType = eType;
}

CWord::CWord(_IN CWord& rWord)
{
	m_pText = new CStringRange(*rWord.m_pText);
	m_eType = rWord.m_eType;
}

CWord::~CWord()
{
	delete m_pText;
}

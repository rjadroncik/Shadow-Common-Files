#include "StreamReadText.h"

using namespace SCFBase;

CStreamReadText::CStreamReadText(_INOUT IStreamRead& rStreamRead)
{
	m_pStream = &rStreamRead;
}

CStreamReadText::~CStreamReadText()
{
}
#include "StreamWriteText.h"

using namespace SCFBase;

CStreamWriteText::CStreamWriteText(_INOUT IStreamWrite& rStreamWrite)
{
	m_pStream = &rStreamWrite;
}

CStreamWriteText::~CStreamWriteText()
{
}
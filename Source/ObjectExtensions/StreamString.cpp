#include "StreamString.h"

using namespace SCFBase;

CStreamString::CStreamString(_INOUT CString& rString)
{
	m_pString = &rString;
	m_uiOffset = 0;

	ADDREF(*(m_pString))
}

CStreamString::~CStreamString()
{
	RELEASE(*(m_pString))
}
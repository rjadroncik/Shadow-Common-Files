#include "FormatInt.h"

using namespace SCFBase;

CFormatInt::CFormatInt()
{
	m_uiDigitsMin = 1;
	m_ucBase = 10;

	m_bBasePrefix = true;
}

CFormatInt::CFormatInt(_IN UINT uiDigitsMin, _IN BYTE ucBase, _IN bool bBasePrefix)
{
	m_uiDigitsMin = uiDigitsMin;
	m_ucBase = ucBase;
	m_bBasePrefix = bBasePrefix;
}

CFormatInt::~CFormatInt()
{
}

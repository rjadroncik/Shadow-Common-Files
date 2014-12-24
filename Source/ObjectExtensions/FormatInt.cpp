#include "FormatInt.h"

using namespace SCFBase;
using namespace SCF;

CFormatInt::CFormatInt()
{
	m_uiDigitsMin = 1;
	m_ucBase = 10;

	m_bBasePrefix = TRUE;
}

CFormatInt::CFormatInt(_IN SCF::UINT uiDigitsMin, _IN SCF::BYTE ucBase, _IN bool bBasePrefix)
{
	m_uiDigitsMin = uiDigitsMin;
	m_ucBase = ucBase;
	m_bBasePrefix = bBasePrefix;
}

CFormatInt::~CFormatInt()
{
}

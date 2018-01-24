#include "EnumeratorRaw.h"

using namespace SCFBase;

CEnumeratorRaw::CEnumeratorRaw(_IN _REF CObject& rSource)
{
	m_pCurrent = nullptr;
	m_pSource = &(CObject&)rSource;

	ADDREF(*m_pSource);
}

CEnumeratorRaw::~CEnumeratorRaw()
{
	RELEASE(*m_pSource);
}

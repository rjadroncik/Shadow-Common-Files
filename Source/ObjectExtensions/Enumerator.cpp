#include "Enumerator.h"

using namespace SCFBase;

CEnumerator::CEnumerator(_IN _REF CObject& rSource)
{
	m_pCurrent = NULL;
	m_pSource = &(CObject&)rSource;
	
	m_bFinished = FALSE;

	ADDREF(*m_pSource);
}

CEnumerator::~CEnumerator()
{
	RELEASE(*m_pSource);
} 
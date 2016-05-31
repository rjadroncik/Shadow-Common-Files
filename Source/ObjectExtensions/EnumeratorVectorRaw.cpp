#include "EnumeratorVectorRaw.h"
#include "VectorRaw.h"

using namespace SCFBase;
using namespace SCFPrivate;

CEnumeratorVectorRaw::CEnumeratorVectorRaw(_IN CVectorRaw& rVector) : CEnumeratorRaw(rVector)
{
	m_uiIndex = 0;

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorVectorRaw::NextStart;

	m_bHasNext = rVector.m_uiCount > 0;
}

CEnumeratorVectorRaw::~CEnumeratorVectorRaw()
{
}

bool CEnumeratorVectorRaw::NextStart()
{
	if (m_uiIndex < ((CVectorRaw*)m_pSource)->m_uiCount)
	{
		m_pCurrent = &((CVectorRaw*)m_pSource)->At(m_uiIndex);
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorVectorRaw::NextContinue;
		return TRUE;
	}
	else
	{
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorVectorRaw::NextEnd; m_bHasNext = FALSE;
		return FALSE;
	}
}

bool CEnumeratorVectorRaw::NextContinue()
{
	m_uiIndex++;

	if (m_uiIndex == ((CVectorRaw*)m_pSource)->m_uiCount)
	{
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorVectorRaw::NextEnd; m_bHasNext = FALSE;
		return FALSE;
	}

	m_pCurrent = &((CVectorRaw*)m_pSource)->At(m_uiIndex);
	return TRUE;
}

bool CEnumeratorVectorRaw::NextEnd()
{
	m_bFinished = TRUE; 
	return FALSE;
}
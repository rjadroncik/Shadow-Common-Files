#include "VectorRangeRaw.h"

using namespace SCFBase;

void CVectorRangeRaw::ChangeStart(_IN SCF::UINT uiStart) _SET { m_ppObjects = &m_pParent->m_ppObjects[uiStart]; }
void CVectorRangeRaw::ChangeSize (_IN SCF::UINT uiSize)  _SET { m_uiCount = uiSize; }

CVectorRangeRaw::CVectorRangeRaw(_IN CVectorRaw& rVector, _IN SCF::UINT uiStart)
{
	m_pParent = (CVectorRaw*)&rVector;
	//BETAONLY(m_pParent->LockAdd());

	m_ppObjects = &m_pParent->m_ppObjects[uiStart];
	m_uiCount = __min(m_pParent->m_uiCount, m_pParent->m_uiCount - uiStart);
}

CVectorRangeRaw::CVectorRangeRaw(_IN CVectorRaw& rVector, _IN SCF::UINT uiStart, _IN SCF::UINT uiSize)
{
	m_pParent = (CVectorRaw*)&rVector;
	//BETAONLY(m_pParent->LockAdd());

	m_ppObjects = &m_pParent->m_ppObjects[uiStart];
	m_uiCount = __min(m_pParent->m_uiCount, uiSize);
}

CVectorRangeRaw::~CVectorRangeRaw()
{
	m_ppObjects = NULL;
	m_uiCount = 0;
	//BETAONLY(m_pParent->LockRelease());
}

CVectorRangeRaw::CVectorRangeRaw(_IN CVectorRangeRaw& rRange)
{
	m_pParent = &rRange.Parent();
	//BETAONLY(m_pParent->LockAdd());

	m_ppObjects = rRange.m_ppObjects;
	m_uiCount = rRange.Size();
}

void CVectorRangeRaw::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt(m_uiCount);

	rStream.PutInt((int)(m_ppObjects - m_pParent->m_ppObjects));
}

void CVectorRangeRaw::Deserialize(_INOUT IStreamRead& rStream)
{
	_ASSERTE(m_ppObjects == NULL);
	m_uiCount = rStream.GetInt();

	m_ppObjects = (CObject**)rStream.GetInt();
}

void CVectorRangeRaw::DependentsSerialize(_INOUT IStreamWriteObject& rStream) const
{
	rStream.Next(*m_pParent);
}

void CVectorRangeRaw::DependentsDeserialize(_INOUT IStreamReadObject& rStream)
{
	rStream.Next();

	m_pParent = (CVectorRaw*)rStream.Current();
	//BETAONLY(m_pParent->LockAdd());

	m_ppObjects = &m_pParent->m_ppObjects[(int)m_ppObjects];
}

//void CVectorRangeRaw::operator =(_IN CVector& rVector)
//{
//	BETAONLY(m_pParent->LockRelease());
//	m_pParent = (CVector*)&rVector;
//	BETAONLY(m_pParent->LockAdd());
//
//	m_ppObjects  = m_pParent->m_ppObjects;
//	m_uiCount = m_pParent->m_uiCount;
//}
//
//void CVectorRangeRaw::operator =(_IN CVectorRangeRaw& rRange)
//{
//	BETAONLY(m_pParent->LockRelease());
//	m_pParent = (CVector*)&rRange.Parent();
//	BETAONLY(m_pParent->LockAdd());
//
//	m_ppObjects = rRange.m_ppObjects;
//	m_uiCount   = rRange.m_uiCount;
//}


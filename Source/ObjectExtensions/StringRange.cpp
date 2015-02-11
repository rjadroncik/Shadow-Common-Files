#include "StringRange.h"

using namespace SCFBase;

#ifdef WIN32
#else
#include <unistd.h>
#endif

void CStringRange::ChangeStart (_IN SCF::UINT uiStart)  _SET { m_szValue = &m_pParent->m_szValue[uiStart]; }
void CStringRange::ChangeLength(_IN SCF::UINT uiLength) _SET { m_uiLength = uiLength; }

void CStringRange::Rebind(_IN _REF CString& rString) _SET
{
	BETAONLY(m_pParent->LockRelease());
	m_pParent = (CString*)&rString;
	BETAONLY(m_pParent->LockAdd());
}

CStringRange::CStringRange(_IN CString& rString, _IN SCF::UINT uiStart)
{
	m_pParent = (CString*)&rString;
	BETAONLY(m_pParent->LockAdd());

	m_szValue  = &m_pParent->m_szValue[uiStart];
	m_uiLength = __min(m_pParent->m_uiLength, m_pParent->m_uiLength - uiStart);
}

CStringRange::CStringRange(_IN CString& rString, _IN SCF::UINT uiStart, _IN SCF::UINT uiLength)
{
	m_pParent = (CString*)&rString;
	BETAONLY(m_pParent->LockAdd());

	m_szValue  = &m_pParent->m_szValue[uiStart];
	m_uiLength = __min(m_pParent->m_uiLength, uiLength);
}

CStringRange::CStringRange(_IN _REF CString& rString, _IN CString& rWhitespaceCharacters, _IN bool bTrimAtBeginning, _IN bool bTrimAtEnd)
{
	m_pParent = (CString*)&rString;
	BETAONLY(m_pParent->LockAdd());

	SCF::UINT uiStart = 0;
	SCF::UINT uiLength = rString.m_uiLength;

	if (bTrimAtBeginning)
	{
		for (; uiStart < uiLength; uiStart++)
		{
			SCF::UINT i = 0;
			for (; i < rWhitespaceCharacters.m_uiLength; i++)
			{
				if (rString[uiStart] == rWhitespaceCharacters[i]) { break; }
			}
			if (i == rWhitespaceCharacters.m_uiLength) { break; }
		}
	}
	if (bTrimAtEnd)
	{
		for (; uiLength > uiStart; uiLength--)
		{
			SCF::UINT i = 0;
			for (; i < rWhitespaceCharacters.m_uiLength; i++)
			{
				if (rString[uiLength - 1] == rWhitespaceCharacters[i]) { break; }
			}
			if (i == rWhitespaceCharacters.m_uiLength) { break; }
		}

		//Here we adjust the length so it corresponds with the
		uiLength -= uiStart;
	}

	m_szValue  = &m_pParent->m_szValue[uiStart];
	m_uiLength = __min(m_pParent->m_uiLength, uiLength);
}

CStringRange::~CStringRange()
{
	m_szValue = NULL;
	BETAONLY(m_pParent->LockRelease());
}

CStringRange::CStringRange(_IN CStringRange& rRange)
{
	m_pParent = &rRange.Parent();
	BETAONLY(m_pParent->LockAdd());

	m_szValue  = rRange.m_szValue;
	m_uiLength = rRange.m_uiLength;
}

void CStringRange::operator =(_IN CString& rString)
{
	BETAONLY(m_pParent->LockRelease());
	m_pParent = (CString*)&rString;
	BETAONLY(m_pParent->LockAdd());

	m_szValue  = m_pParent->m_szValue;
	m_uiLength = m_pParent->m_uiLength;
}
void CStringRange::operator =(_IN CStringRange& rRange)
{
	BETAONLY(m_pParent->LockRelease());
	m_pParent = (CString*)&rRange.Parent();
	BETAONLY(m_pParent->LockAdd());

	m_szValue  = rRange.m_szValue;
	m_uiLength = rRange.m_uiLength;
}

void CStringRange::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt(m_uiLength);

	rStream.PutInt((int)(m_szValue - m_pParent->m_szValue));
}

void CStringRange::Deserialize(_INOUT IStreamRead& rStream)
{
	_ASSERTE(m_szValue == NULL);
	m_uiLength = rStream.GetInt();

	m_szValue = (SCF::LPTSTR)rStream.GetInt();
}

void CStringRange::DependentsSerialize(_INOUT IStreamWriteObject& rStream) const
{
	rStream.Next(*m_pParent);
}

void CStringRange::DependentsDeserialize(_INOUT IStreamReadObject& rStream)
{
	rStream.Next();

	m_pParent = (CString*)rStream.Current();
	BETAONLY(m_pParent->LockAdd());

	m_szValue = &m_pParent->m_szValue[(intptr_t)m_szValue];
}



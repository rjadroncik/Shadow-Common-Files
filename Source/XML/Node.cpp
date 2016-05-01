#include "Node.h"
#include "Writer.h"

using namespace SCFXML;

CXMLNode::CXMLNode()
{
	m_uiPrefix = 0;
}

CXMLNode::~CXMLNode()
{
	if (m_pValue) { RELEASE(*(m_pValue)); delete m_pValue; m_pValue = NULL; }
}

CXMLNode* CXMLNode::ChildNamed(_IN CString rName) _GET
{
	register CXMLNode* pNode = ChildFirst();

	//Go through all children  & compare names
	while (pNode)
	{
		if (pNode->QName() == rName) { return pNode; }

		pNode = pNode->Next();
	}

	return NULL;
}

CXMLNode* CXMLNode::ChildOrSelfNamed(_IN CString rName) _GET
{
	if (this->QName() == rName) { return (CXMLNode*)this; }

	return ChildNamed(rName);
}

CXMLNode* CXMLNode::NextNamed(_IN CString rName) _GET
{
	register CXMLNode* pNode = (CXMLNode*)m_pNext;

	//Go through all siblings & compare names
	while (pNode)
	{
		if (pNode->QName() == rName) { return pNode; }

		pNode = pNode->Next();
	}

	return NULL;
}

CXMLNode* CXMLNode::DescendantsNamed(_IN CString rName) _GET
{
	//If the name fits this node
	if (m_QName == rName) { return (CXMLNode*)this; }

	CXMLNode* pNode = ChildFirst();
	CXMLNode* pFoundNode = NULL;

	//Go through all children
	while (pNode)
	{
		//Try finding the node within them 
		pFoundNode = pNode->DescendantsNamed(rName);
		if (pFoundNode) { return pFoundNode; }

		pNode = pNode->Next();
	}

	return NULL;
}

CXMLNode* CXMLNode::DescendantsOrSelfNamed(_IN CString rName) _GET
{
	if (this->QName() == rName) { return (CXMLNode*)this; }

	return DescendantsNamed(rName);
}

CString CXMLNode::ToString() _GET
{
	CString              Result;
	CStreamStringWrite   ResultStreamWrite(Result);
	CStreamWriteTextUCS2 ResultStreamWriteText(ResultStreamWrite, FALSE);

	CXMLWriter(ResultStreamWriteText).WriteNode(*this);

	return Result;
}

void CXMLNode::QName(_IN CString& rPrefix, _IN CString& rName) _SET
{
	if (rPrefix.Length())
	{
		m_QName.Resize(rPrefix.Length() + rName.Length() + 1);

		CMemory::Copy(m_QName.Value(), rPrefix.Value(), rPrefix.Length() * sizeof(TCHAR));

		m_QName.AtPut(rPrefix.Length(), ':');

		CMemory::Copy(m_QName.Value() + rPrefix.Length() + 1, rName.Value(), rName.Length() * sizeof(TCHAR));
	}
	else { m_QName = rName; }

	m_uiPrefix = rPrefix.Length();
}

void CXMLNode::QName(_IN CString& rName, _IN UINT uiPrefixLength) _SET
{
	m_QName = rName;
	m_uiPrefix = uiPrefixLength;
}

void CXMLNode::Name(_IN CString& rName) _SET
{
	if (m_uiPrefix)
	{
		m_QName.Resize(m_uiPrefix + rName.Length() + 1);

		CMemory::Copy(m_QName.Value() + m_uiPrefix + 1, rName.Value(), rName.Length() * sizeof(TCHAR));
	} 
	else { m_QName = rName; }
}

void CXMLNode::Prefix(_IN CString& rPrefix) _SET
{
	//Check whether we have no full name or whether the full name is only a namespace
	if (!m_QName.Length() || (m_QName.Length() == (m_uiPrefix + 1)))
	{
		m_QName = rPrefix;
		m_QName += ':';
	}
	else
	{
		UINT uiNameLength = m_uiPrefix ? m_QName.Length() - m_uiPrefix - 1 : m_QName.Length();

		if (rPrefix.Length())
		{
			if (rPrefix.Length() >= m_uiPrefix)
			{
				m_QName.Resize(rPrefix.Length() + uiNameLength + 1);

				if (m_uiPrefix) { CMemory::CopyBackwards(m_QName.Value() + rPrefix.Length() + 1, m_QName.Value() + m_uiPrefix + 1, uiNameLength * sizeof(TCHAR)); } 
				else               { CMemory::CopyBackwards(m_QName.Value() + rPrefix.Length() + 1, m_QName.Value() + m_uiPrefix,     uiNameLength * sizeof(TCHAR)); }
			}
			else
			{
				CMemory::Copy(m_QName.Value() + rPrefix.Length() + 1, m_QName.Value() + m_uiPrefix + 1, uiNameLength * sizeof(TCHAR));

				m_QName.Resize(rPrefix.Length() + uiNameLength + 1);
			}

			m_QName.AtPut(rPrefix.Length(), ':');

			CMemory::Copy(m_QName.Value(), rPrefix.Value(), rPrefix.Length() * sizeof(TCHAR));
		}
		else
		{
			CMemory::Copy(m_QName.Value(), m_QName.Value() + m_uiPrefix + 1, uiNameLength * sizeof(TCHAR));

			m_QName.Resize(uiNameLength);
		}
	}

	m_uiPrefix = rPrefix.Length();
}

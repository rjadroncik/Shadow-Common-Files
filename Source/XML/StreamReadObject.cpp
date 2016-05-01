#include "StreamReadObject.h"
#include "ObjectSerializable.h"

using namespace SCFXML;

CXMLStreamReadObject::CXMLStreamReadObject(_IN _REF CXMLNode& rNode, _IN bool bDelayDeserialization) : CXMLStreamRead(rNode)
{
	m_bDelayDeserialization = bDelayDeserialization;
	m_pCurrent = NULL;
	m_bCurrentDeserialized = FALSE;
}

CXMLStreamReadObject::CXMLStreamReadObject(_IN _REF CXMLDocument& rDocument, _IN bool bDelayDeserialization) : CXMLStreamRead(rDocument)
{
	m_bDelayDeserialization = bDelayDeserialization;
	m_pCurrent = NULL;
	m_bCurrentDeserialized = FALSE;
}

CXMLStreamReadObject::~CXMLStreamReadObject()
{
	m_Objects.AllRemove();
	m_ObjectsExternal.AllRemove();
}

bool CXMLStreamReadObject::Next(_OUT _OPT CString** ppOutID)
{
	if (!m_bCurrentDeserialized && m_pCurrent)
	{
		m_pCurrent->XMLDeserialize          (*this);
		m_pCurrent->XMLDependentsDeserialize(*this);
	}

	m_pAttribute = NULL;
	m_pCurrent = NULL;

TryAgain:
	if (m_pEnumerator->Next())
	{
		//Skip the root 
		if (m_pEnumerator->Current() == &(m_pEnumerator->Source())) { return Next(ppOutID); }

		if (m_pEnumerator->Current()->Type() == XmlElement)
		{
			m_pCurrent = Next(*m_pEnumerator->Current(), ppOutID);
			m_bCurrentDeserialized = !m_bDelayDeserialization;
			return TRUE;
		}
		else { goto TryAgain; }
	}
	else { return FALSE; }
}

CXMLObjectSerializable* CXMLStreamReadObject::Current()
{
	if (!m_pCurrent) { return NULL; }
	else
	{
		if (!m_bCurrentDeserialized)
		{
			m_pCurrent->XMLDeserialize          (*this);
			m_pCurrent->XMLDependentsDeserialize(*this);

			m_bCurrentDeserialized = TRUE;
		}

		return m_pCurrent;
	}
}

CXMLObjectSerializable* CXMLStreamReadObject::Next(_IN CXMLNode& rNode, _OUT _OPT CString** ppOutID)
{
	static CString csNull(STRING("null"));

	if (rNode.QName() == csNull) { return NULL; }

	//We need a to check whether the class is registered or else we cant construct it
	if (CXMLObjectSerializable::ClassIsRegistered(rNode.QName()))
	{
		//Get ID of the object
		CString* pID = GetValue();
		if (pID)
		{
			//Check whether we already read this object
			CXMLObjectSerializable* pOldObject = (CXMLObjectSerializable*)m_Objects.At        (*pID);
			if (!pOldObject)      { pOldObject = (CXMLObjectSerializable*)m_ObjectsExternal.At(*pID); }
			if (pOldObject)
			{
				if (ppOutID) { *ppOutID = pID; }

				return pOldObject;
			}
			else
			{
				//Make a new object for the node
				CXMLObjectSerializable* pObject = CXMLObjectSerializable::New(rNode.QName());
				if (pObject)
				{
					//Register object based on it's ID for later referencing
					m_Objects.AtPut(*pID, *pObject);

					if (!m_bDelayDeserialization)
					{
						pObject->XMLDeserialize          (*this);
						pObject->XMLDependentsDeserialize(*this);
					}
				}

				if (ppOutID) { *ppOutID = pID; }
				return pObject;
			}
		}
	}

	SCFError(ErrorXMLStreamFailedRead);
	return NULL;
}

void CXMLStreamReadObject::ObjectsReadSubmit(_IN CDictionaryInt64& rObjectIDs)
{
	CEnumeratorDictionaryInt64 Enumerator(rObjectIDs);

	while (Enumerator.Next())
	{
		m_ObjectsExternal.AtPut(*(CString*)Enumerator.Current(), *(CObject*)Enumerator.CurrentKey()); 
	}
}

void CXMLStreamReadObject::ObjectsReadSubmit(_IN CDictionaryString<CObject>& rObjects)
{
	CEnumeratorDictionaryString Enumerator(rObjects);

	while (Enumerator.Next())
	{
		m_ObjectsExternal.AtPut(Enumerator.CurrentPath(), *Enumerator.Current()); 
	}
}
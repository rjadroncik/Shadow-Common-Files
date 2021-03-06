#include "StreamWriteObject.h"
#include "ObjectSerializable.h"

using namespace SCFXML;

CXMLStreamWriteObject::CXMLStreamWriteObject(_OUT _REF CXMLElement& rElement) : CXMLStreamWrite(rElement)
{
}

CXMLStreamWriteObject::CXMLStreamWriteObject(_OUT _REF CXMLDocument& rDocument) : CXMLStreamWrite(rDocument)
{
}

CXMLStreamWriteObject::CXMLStreamWriteObject(_OUT _REF CXMLDocument& rDocument, _IN CString& rRootName, _IN CString& rRootPrefix, _IN CString& rRootNamespace)
	: CXMLStreamWrite(rDocument, rRootName, rRootPrefix, rRootNamespace)
{
}

CXMLStreamWriteObject::~CXMLStreamWriteObject()
{
	m_ObjectIDs.AllRemove();
	m_ObjectIDsExternal.AllDelete();
}

bool CXMLStreamWriteObject::Next(_IN CXMLObjectSerializable* pObject)
{
	if (pObject)
	{
		//We need a to check whether the class is registered or else we cant serialize it properly
		CString xmlName = pObject->XmlName();
		if (xmlName.Length())
		{
			BlockStart(xmlName);
			{
				CString* pID = nullptr;

				//Find out whether the object was stored (it is stored if we have an ID)
				CString* pIDStored = m_ObjectIDs.At((UINT64)pObject);
				if (!pIDStored) { pIDStored = m_ObjectIDsExternal.At((UINT64)pObject); }

				if (pIDStored)
				{
					//Just copy the ID that we already have
					 pID = new CString(*pIDStored);
				}
				else
				{
					//No ID yet, so query it from the object
					pID = new CString(pObject->ID()); 
				}

				//Add the ID attribute
				PutValue(STRING("id"), *pID);

				//If the object has an ID stored in the ID dictionaries, we consider 
				//it as if it was stored already
				if (pIDStored)
				{
					BlockEnd();
					return true;
				}
				else { m_ObjectIDs.AtPut((UINT64)pObject, *pID); }
			}

			pObject->XMLSerialize(*this);
			pObject->XMLDependentsSerialize(*this);

			BlockEnd();
		}
		else 
		{
			SCFError(ErrorXMLStreamFailedWrite);
			return false;
		}
	}
	else
	{
		static CString csNull(STRING_CREATE_ARGS("null"));

		BlockStart(csNull);
		BlockEnd();
	}

	return true;
}

void CXMLStreamWriteObject::ObjectsWrittenSubmit(_IN CDictionaryInt64<CString>& rObjectIDs)
{
	CEnumeratorDictionaryInt64<CString> Enumerator(rObjectIDs);

	register CObject* pPreviousValue = nullptr;

	while (Enumerator.Next())
	{
		pPreviousValue = m_ObjectIDsExternal.AtPut((UINT64)Enumerator.CurrentKey(), *(new CString(*Enumerator.Current())));
		if (pPreviousValue) { delete pPreviousValue; }
	}
}

void CXMLStreamWriteObject::ObjectsWrittenSubmit(_IN CDictionaryString<CObject>& rObjects)
{
	CEnumeratorDictionaryString<CObject> Enumerator(rObjects);

	register CObject* pPreviousValue = nullptr;

	while (Enumerator.Next())
	{
		pPreviousValue = m_ObjectIDsExternal.AtPut((UINT64)Enumerator.Current(), *(new CString(Enumerator.CurrentPath())));
		if (pPreviousValue) { delete pPreviousValue; }
	}
}

#include "StreamWriteObject.h"
#include "ObjectSerializable.h"

using namespace SCF;
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
		CString ClassName = CXMLObjectSerializable::ClassKeyToQName(pObject->ClassKey());
		if (ClassName.Length())
		{
			BlockStart(ClassName);
			{
				CString* pID = NULL;

				//Find out whether the object was stored (it is stored if we have an ID)
				CString* pIDStored = (CString*)m_ObjectIDs.At((SCF::UINT64)pObject);
				if (!pIDStored) { pIDStored = (CString*)m_ObjectIDsExternal.At((SCF::UINT64)pObject); }

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
					return TRUE;
				}
				else { m_ObjectIDs.AtPut((SCF::UINT64)pObject, *pID); }
			}

			pObject->XMLSerialize(*this);
			pObject->XMLDependentsSerialize(*this);

			BlockEnd();
		}
		else 
		{
			SCFError(ErrorXMLStreamFailedWrite);
			return FALSE;
		}
	}
	else
	{
		static CString csNull(STRING_CREATE_ARGS("null"));

		BlockStart(csNull);
		BlockEnd();
	}

	return TRUE;
}

void CXMLStreamWriteObject::ObjectsWrittenSubmit(_IN CDictionaryInt64& rObjectIDs)
{
	CEnumeratorDictionaryInt64 Enumerator(rObjectIDs);

	register CObject* pPreviousValue = NULL;

	while (Enumerator.Next())
	{
		pPreviousValue = m_ObjectIDsExternal.AtPut((SCF::UINT64)Enumerator.CurrentKey(), *(new CString(*(CString*)Enumerator.Current())));
		if (pPreviousValue) { delete pPreviousValue; }
	}
}

void CXMLStreamWriteObject::ObjectsWrittenSubmit(_IN CDictionaryString<CObject>& rObjects)
{
	CEnumeratorDictionaryString Enumerator(rObjects);

	register CObject* pPreviousValue = NULL;

	while (Enumerator.Next())
	{
		pPreviousValue = m_ObjectIDsExternal.AtPut((SCF::UINT64)Enumerator.Current(), *(new CString(Enumerator.CurrentPath())));
		if (pPreviousValue) { delete pPreviousValue; }
	}
}

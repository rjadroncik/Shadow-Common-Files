#include "StreamReadObject.h"

#include "Bool.h"
#include "Float.h"
#include "Char.h"
#include "Int.h"

#include "Array.h"
#include "Association.h"
#include "DictionaryString.h"

#include "Vector.h"

using namespace SCFBase;

CStreamReadObject::CStreamReadObject(_INOUT IStreamRead& rStreamRead)
{
	m_pStream = &rStreamRead;
	m_pCurrent = NULL;
}

CStreamReadObject::~CStreamReadObject()
{
}

bool CStreamReadObject::Next()
{
	if (m_pStream->BytesLeft() == 0) { return NULL; }

	SCF::ENUM   eObjectType  = (SCF::ENUM)m_pStream->GetInt();
	SCF::UINT64 ui64ObjectID = (SCF::UINT64)m_pStream->GetInt64();

	CObjectSerializable* pCurrent = (ui64ObjectID != 0) ? (CObjectSerializable*)m_ObjectsRead.At(ui64ObjectID) : NULL;

	//Where should m_pCurrent be set???
	if (pCurrent) { m_pCurrent = pCurrent; return TRUE; }
	else
	{
		pCurrent = (CObjectSerializable*)CObjectSerializable::New(eObjectType);
		_ASSERTE(pCurrent != NULL);

		if (!pCurrent) 
		{
			SCFError(ErrorSerializationNotSupported); 
			return FALSE;
		}

		m_ObjectsRead.AtPut(ui64ObjectID, *pCurrent);

		pCurrent->Deserialize          (*((IStreamRead*)m_pStream));
		pCurrent->DependentsDeserialize(*((IStreamReadObject*)this));
	}

	//Where should m_pCurrent be set???
	m_pCurrent = pCurrent;
	return (pCurrent != NULL);
}
#include "StreamWriteObject.h"

using namespace SCFBase;

CStreamWriteObject::CStreamWriteObject(_INOUT IStreamWrite& rStreamWrite)
{
	m_pStream = &rStreamWrite;
}

CStreamWriteObject::~CStreamWriteObject()
{
}

bool CStreamWriteObject::Next(_IN CObjectSerializable& rObject)
{
	if (m_ObjectsStored.ContainsKey((SCF::UINT64)&rObject))
	{
		//Store 0 as class key to indicate the object was already stored &
		//now is only being referenced
		m_pStream->PutInt(0);
		m_pStream->PutInt64((SCF::UINT64)&rObject);
	}
	else
	{
		m_ObjectsStored.AtPut((SCF::UINT64)&rObject, rObject);

		//Store the class key & the id(pointer) of the object so that it can later
		//be reconstructed & also identified for referencing
		m_pStream->PutInt(rObject.ClassKey());
		m_pStream->PutInt64((SCF::UINT64)&rObject);

		rObject.Serialize          (*((IStreamWrite*)m_pStream));
		rObject.DependentsSerialize(*((IStreamWriteObject*)this));
	}

	return TRUE;
}
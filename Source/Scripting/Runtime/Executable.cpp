#include "stdafx.h"
#include "Executable.h"

using namespace SCFScripting;

void* CExecutable::AsType(_IN ENUM eType) const
{
	switch (eType)
	{
	case ClassExecutable: { return (CExecutable*)this; }
	}

	return NULL;
}

CExecutable::CExecutable(_IN UINT uiCodeSize) : m_Code(uiCodeSize)
{
	m_pMain = (const BYTE*)m_Code.Value();
}

CExecutable::~CExecutable()
{
}

void CExecutable::DumpCode(_IN CFile& rFile)
{
	CStreamFileWrite StreamWrite(rFile);

	CStreamMemoryRead StreamRead(m_Code);

	CMemoryBlock Buffer(4096);

	while (StreamRead.BytesLeft() > 0)
	{
		UINT uiBytesToTransfer = (StreamRead.BytesLeft() > Buffer.Size()) ? (Buffer.Size()) : ((UINT)StreamRead.BytesLeft());

		StreamRead.GetBytes (Buffer.Value(), uiBytesToTransfer);
		StreamWrite.PutBytes(Buffer.Value(), uiBytesToTransfer);
	}

	StreamWrite.Close();
}



#include "Record.h"

using namespace SCFDatafileIOPrivate;

CRecord::CRecord()
{
}

CRecord::~CRecord()
{
}

bool CRecord::Encrypted() _GET { return (m_ucAttributes & AttributeEncrypted)  != 0; }

bool CRecord::Encrypted(_IN bool bEncrypted) _SET
{ 
	m_ucAttributes |= AttributeEncrypted;
	if (!bEncrypted) { m_ucAttributes ^= AttributeEncrypted; }

	return TRUE;
}

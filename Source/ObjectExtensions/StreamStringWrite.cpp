#include "StreamStringWrite.h"
#include "Memory.h"

using namespace SCFBase;

CStreamStringWrite::CStreamStringWrite(_INOUT CString& rString, _IN UINT uiOffsetStart) : CStreamString(rString)
{
	m_uiOffsetStart = uiOffsetStart;

	m_uiOffset = m_uiOffsetStart;
}

CStreamStringWrite::CStreamStringWrite(_INOUT CString& rString, _IN bool bAppend) : CStreamString(rString)
{
	if (bAppend) { m_uiOffsetStart = rString.m_uiLength * sizeof(TCHAR); m_bAppending = TRUE; }
	else         { m_uiOffsetStart = 0;                                       m_bAppending = FALSE; }

	m_uiOffset = m_uiOffsetStart;
}

CStreamStringWrite::~CStreamStringWrite()
{
}

void CStreamStringWrite::PutByte(_IN BYTE ucValue)
{
	m_pString->Resize((m_uiOffset + sizeof(ucValue) + 1) >> 1);

	*(BYTE*)((BYTE*)m_pString->m_szValue + m_uiOffset) = ucValue;
	m_uiOffset += sizeof(ucValue);
}

void CStreamStringWrite::PutWord(_IN WORD usValue)
{
	m_pString->Resize((m_uiOffset + sizeof(usValue) + 1) >> 1);

	*(WORD*)((BYTE*)m_pString->m_szValue + m_uiOffset) = usValue;
	m_uiOffset += sizeof(usValue);
}

void CStreamStringWrite::PutInt(_IN int iValue)
{
	m_pString->Resize((m_uiOffset + sizeof(iValue) + 1) >> 1);

	*(int*)((BYTE*)m_pString->m_szValue + m_uiOffset) = iValue;
	m_uiOffset += sizeof(iValue);
}

void CStreamStringWrite::PutInt64(_IN INT64 i64Value)
{
	m_pString->Resize((m_uiOffset + sizeof(i64Value) + 1) >> 1);

	*(INT64*)((BYTE*)m_pString->m_szValue + m_uiOffset) = i64Value;
	m_uiOffset += sizeof(i64Value);
}

void CStreamStringWrite::PutFloat(_IN float fValue)
{
	m_pString->Resize((m_uiOffset + sizeof(fValue) + 1) >> 1);

	*(float*)((BYTE*)m_pString->m_szValue + m_uiOffset) = fValue;
	m_uiOffset += sizeof(fValue);
}

void CStreamStringWrite::PutBool(_IN bool bValue)
{
	m_pString->Resize((m_uiOffset + sizeof(bValue) + 1) >> 1);

	*(bool*)((BYTE*)m_pString->m_szValue + m_uiOffset) = bValue;
	m_uiOffset += sizeof(bValue);
}

void CStreamStringWrite::PutChar(_IN TCHAR cValue)
{
	m_pString->Resize((m_uiOffset + sizeof(cValue) + 1) >> 1);

	*(TCHAR*)((BYTE*)m_pString->m_szValue + m_uiOffset) = cValue;
	m_uiOffset += sizeof(cValue);
}

void CStreamStringWrite::PutBytes(_IN void* vpBytes, _IN UINT uiCount)
{
	m_pString->Resize((m_uiOffset + uiCount + 1) >> 1);

	CMemory::Copy(((BYTE*)m_pString->m_szValue + m_uiOffset), vpBytes, uiCount);
	m_uiOffset += uiCount;
}

UINT64 CStreamStringWrite::BytesWritten() _GET
{
	return m_uiOffset - m_uiOffsetStart;
}

bool CStreamStringWrite::Appending() _GET
{
	return m_bAppending;
}
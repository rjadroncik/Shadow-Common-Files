#include "StreamDummyWrite.h"

using namespace SCFBase;

CStreamDummyWrite::CStreamDummyWrite()
{
	m_uiBytesWritten = 0;
}

CStreamDummyWrite::~CStreamDummyWrite()
{
}

void CStreamDummyWrite::PutByte(_IN SCF::BYTE ucValue)                     { m_uiBytesWritten += sizeof(ucValue);  SCF_UNREFERENCED_PARAMETER(ucValue); }
void CStreamDummyWrite::PutWord(_IN SCF::WORD usValue)                     { m_uiBytesWritten += sizeof(usValue);  SCF_UNREFERENCED_PARAMETER(usValue); }
void CStreamDummyWrite::PutInt(_IN int iValue)                             { m_uiBytesWritten += sizeof(iValue);   SCF_UNREFERENCED_PARAMETER(iValue); }
void CStreamDummyWrite::PutInt64(_IN SCF::INT64 i64Value)                  { m_uiBytesWritten += sizeof(i64Value); SCF_UNREFERENCED_PARAMETER(i64Value); }
void CStreamDummyWrite::PutFloat(_IN float fValue)                         { m_uiBytesWritten += sizeof(fValue);   SCF_UNREFERENCED_PARAMETER(fValue); }
void CStreamDummyWrite::PutBool(_IN bool bValue)                           { m_uiBytesWritten += sizeof(bValue);   SCF_UNREFERENCED_PARAMETER(bValue); }
void CStreamDummyWrite::PutChar(_IN SCF::TCHAR cValue)                     { m_uiBytesWritten += sizeof(cValue);   SCF_UNREFERENCED_PARAMETER(cValue); }
void CStreamDummyWrite::PutBytes(_IN void* vpBytes, _IN SCF::UINT uiCount) { m_uiBytesWritten += uiCount;          SCF_UNREFERENCED_PARAMETER(vpBytes); }

SCF::UINT64 CStreamDummyWrite::BytesWritten() _GET { return m_uiBytesWritten; }
bool        CStreamDummyWrite::Appending()    _GET { return FALSE; }
#include "String.h"
#include "Memory.h"
#include "FSBHeap.h"

#include <wchar.h>
#include <string.h>
#include <malloc.h>

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#else
#endif

using namespace SCFBase;

SCFPrivate::CFSBHeap String_Heap64 (64 + 4);
SCFPrivate::CFSBHeap String_Heap128(128 + 4);

void String_CopyChars(_OUT TCHAR* cpDestination, _IN TCHAR* cpSource, _IN UINT uiChars)
{
	if (uiChars == 0) { *cpDestination = 0; return; }

	CMemory::Copy(cpDestination, cpSource, uiChars * 2);
}

LPTSTR CString::StringAlloc(_IN UINT uiLength)
{
	//_ASSERTE(uiLength != 0);

	register UINT uiBytes = sizeof(TCHAR) * (uiLength + 1);

	if (uiBytes < 65)  { void* vpMemory = String_Heap64.Allocate();  *(void**)vpMemory = &String_Heap64;  return (LPTSTR)((void**)vpMemory + 1); }
	if (uiBytes < 129) { void* vpMemory = String_Heap128.Allocate(); *(void**)vpMemory = &String_Heap128; return (LPTSTR)((void**)vpMemory + 1); }
	                     void* vpMemory = malloc(uiBytes + 4);  *(void**)vpMemory = nullptr;       return (LPTSTR)((void**)vpMemory + 1);
}

LPTSTR CString::StringRealloc(_IN LPTSTR szString, _IN UINT uiLength, _IN UINT uiLengthNew)
{
	if (szString == nullptr)
	{
		_ASSERTE(uiLength == 0);
		//_ASSERTE(uiLengthNew != 0);

		return StringAlloc(uiLengthNew);
	}
	if (uiLength >= uiLengthNew) { return szString; }

	register UINT uiBytes    = sizeof(TCHAR) * (uiLength    + 1);
	register UINT uiBytesNew = sizeof(TCHAR) * (uiLengthNew + 1);

	SCFPrivate::CFSBHeap* pHeapNew = nullptr;
	SCFPrivate::CFSBHeap* pHeap = *((SCFPrivate::CFSBHeap**)szString - 1);
	if (pHeap)
	{
		if      (uiBytesNew < 65)  { pHeapNew = &String_Heap64; }
		else if (uiBytesNew < 129) { pHeapNew = &String_Heap128; }

		if (pHeap == pHeapNew) { return szString; }
	}

	void* vpMemory = nullptr;

	if (pHeapNew)
	{
		vpMemory = pHeapNew->Allocate();
		*(void**)vpMemory = pHeapNew;

		CMemory::Copy((LPTSTR)((void**)vpMemory + 1), szString, uiBytes);
		pHeap->Free((void**)szString - 1);
	}
	else
	{
		if (pHeap)
		{
			vpMemory = malloc(uiBytesNew + 4);
			*(void**)vpMemory = nullptr;

			CMemory::Copy((LPTSTR)((void**)vpMemory + 1), szString, uiBytes);
			pHeap->Free((void**)szString - 1);
		}
		else
		{
			vpMemory = realloc((void**)szString - 1, uiBytesNew + 4);
			*(void**)vpMemory = nullptr;
		}
	}

	return (LPTSTR)((void**)vpMemory + 1);
}

void CString::StringFree(_IN LPTSTR szString)
{
	SCFPrivate::CFSBHeap* pHeap = *((SCFPrivate::CFSBHeap**)szString - 1);
	if (pHeap)
	{
		pHeap->Free((void**)szString - 1);
	}
	else { free((void**)szString - 1); }
}

CString::CString()
{
	m_bMemoryAllocated = false;
	BETAONLY(m_uiLocks = 0);

	m_uiLength = 0;
	m_szValue  = nullptr;
}

CString::CString(_IN CString& rString)
{
	m_bMemoryAllocated = false;
	BETAONLY(m_uiLocks = 0);

	this->InternalSize(((CString&)rString).m_uiLength);
	String_CopyChars(m_szValue,((CString&)rString).m_szValue, m_uiLength);
}

CString::CString(_IN CString& rString, _IN bool bAllocateMemory)
{
	m_bMemoryAllocated = false;
	BETAONLY(m_uiLocks = 0);

	if (bAllocateMemory)
	{
		this->InternalSize(((CString&)rString).m_uiLength);
		String_CopyChars(m_szValue,((CString&)rString).m_szValue, m_uiLength);
	}
	else
	{
		m_szValue  = rString.m_szValue;
		m_uiLength = rString.m_uiLength;
	}
}

//CString::CString(_INOUT CString vString)
//{
//	BETAONLY(m_uiLocks = vString.m_uiLocks);
//
//	m_szValue          = vString.m_szValue;
//	m_uiLength         = vString.m_uiLength;
//	m_bMemoryAllocated = vString.m_bMemoryAllocated;
//
//	//Important! - this is where the source string looses control over the memory block (if it ever had any control)
//	vString.m_bMemoryAllocated = false;
//}

CString::CString(_INOUT CString& rString, _IN bool bAllocateMemory, _IN bool bTakeOwnageOfMemory)
{
	if (bAllocateMemory || !bTakeOwnageOfMemory)
	{
		SCFError(ErrorInvalidArgument);

		BETAONLY(m_uiLocks = 0);

		m_szValue          = 0;
		m_uiLength         = 0;
		m_bMemoryAllocated = false;

		return;
	}

	BETAONLY(m_uiLocks = rString.m_uiLocks);

	m_szValue          = rString.m_szValue;
	m_uiLength         = rString.m_uiLength;
	m_bMemoryAllocated = rString.m_bMemoryAllocated;

	//Important! - this is where the source string looses control over the memory block (if it ever had any control)
	rString.m_bMemoryAllocated = false;;
}

CString::CString(_IN LPTSTR szText, _IN bool bAllocateMemory)
{
	m_bMemoryAllocated = false;
	BETAONLY(m_uiLocks = 0);

	if (bAllocateMemory)
	{
		this->InternalSize((UINT)wcslen(szText));
		String_CopyChars(m_szValue, szText, m_uiLength);
	}
	else
	{
		m_szValue  = szText;
		m_uiLength = (UINT)wcslen(szText);
	}
}

CString::CString(_IN LPTSTR sText, _IN UINT uiLength, _IN bool bAllocateMemory)
{
	m_bMemoryAllocated = false;
	BETAONLY(m_uiLocks = 0);

	if (bAllocateMemory)
	{
		this->InternalSize(uiLength);
		String_CopyChars(m_szValue, sText, m_uiLength);
	}
	else
	{
		m_szValue  = sText;
		m_uiLength = uiLength;
	}
}

CString::CString(_IN char* szText)
{
	m_bMemoryAllocated = false;
	BETAONLY(m_uiLocks = 0);

	this->InternalSize((UINT)strlen(szText));

	for (int i = (m_uiLength - 1); i >= 0; i--)
	{
		m_szValue[i] = szText[i];
	}
}

CString::CString(_IN LPTSTR szText1, _IN CString& rString2)
{
	m_bMemoryAllocated = false;
	BETAONLY(m_uiLocks = 0);

	register UINT uiLength1 = (UINT)wcslen(szText1);
	register UINT uiLength2 = ((CString&)rString2).m_uiLength;

	this->InternalSize(uiLength1 + uiLength2);

	String_CopyChars(m_szValue,             szText1,                        uiLength1);
	String_CopyChars(&m_szValue[uiLength1], ((CString&)rString2).m_szValue, uiLength2);
}

CString::CString(_IN CString& rString1, _IN LPTSTR szText2)
{
	m_bMemoryAllocated = false;
	BETAONLY(m_uiLocks = 0);

	register UINT uiLength1 = ((CString&)rString1).m_uiLength;
	register UINT uiLength2 = (UINT)wcslen(szText2);

	this->InternalSize(uiLength1 + uiLength2);

	String_CopyChars(m_szValue,             ((CString&)rString1).m_szValue, uiLength1);
	String_CopyChars(&m_szValue[uiLength1], szText2,                        uiLength2);
}

CString::CString(_IN CString& rString1, _IN CString& rString2)
{
	m_bMemoryAllocated = false;
	BETAONLY(m_uiLocks = 0);

	register UINT uiLength1 = ((CString&)rString1).m_uiLength;
	register UINT uiLength2 = ((CString&)rString2).m_uiLength;

	this->InternalSize(uiLength1 + uiLength2);

	String_CopyChars(m_szValue,             ((CString&)rString1).m_szValue, uiLength1);
	String_CopyChars(&m_szValue[uiLength1], ((CString&)rString2).m_szValue, uiLength2);
}

CString::CString(_IN CString& rString, _IN TCHAR cChar)
{
	m_bMemoryAllocated = false;
	BETAONLY(m_uiLocks = 0);

	register UINT uiLength = ((CString&)rString).m_uiLength;

	this->InternalSize(uiLength + 1);

	String_CopyChars(m_szValue, ((CString&)rString).m_szValue, uiLength);

	m_szValue[uiLength] = cChar;
}

CString::~CString()
{
	BETAONLY(_ASSERTE(!this->Locked()));

	//CString range sets [m_szValue] to nullptr to prevent freeing up memory that it doesn't own
	if (m_bMemoryAllocated && m_szValue) { StringFree(m_szValue); }
}

int CString::IndexOf(_IN TCHAR cChar)
{
	for (UINT i = 0; i < m_uiLength; i++)
	{
		if (m_szValue[i] == cChar) { return i; }
	}

	return -1;
}

void CString::operator =(_IN CString& rString)
{
	BETAONLY(_ASSERTE(!this->Locked()));

	if (m_bMemoryAllocated) { this->InternalResize(rString.m_uiLength); }
	else                    { this->InternalSize  (rString.m_uiLength); }

	String_CopyChars(m_szValue, rString.m_szValue,  m_uiLength);
}

void CString::operator =(_IN LPTSTR szText)
{
	BETAONLY(_ASSERTE(!this->Locked()));

	if (m_bMemoryAllocated) { this->InternalResize((UINT)wcslen(szText)); }
	else                    { this->InternalSize  ((UINT)wcslen(szText)); }

	String_CopyChars(m_szValue, szText, m_uiLength);
}

void CString::operator =(_IN char* szText)
{
	BETAONLY(_ASSERTE(!this->Locked()));

	if (m_bMemoryAllocated) { this->InternalResize((UINT)strlen(szText)); }
	else                    { this->InternalSize  ((UINT)strlen(szText)); }

	for (int i = (m_uiLength - 1); i >= 0; i--)
	{
		m_szValue[i] = szText[i];
	}
}

void CString::operator =(_IN TCHAR cChar)
{
	BETAONLY(_ASSERTE(!this->Locked()));

	if (m_bMemoryAllocated) { this->InternalResize(1); }
	else                    { this->InternalSize  (1); }

	m_szValue[0] = cChar;
}

void CString::Assign(_IN LPTSTR sText, _IN UINT uiLength)
{
	BETAONLY(_ASSERTE(!this->Locked()));

	if (m_bMemoryAllocated) { this->InternalResize(uiLength); }
	else                    { this->InternalSize  (uiLength); }

	String_CopyChars(m_szValue, sText, m_uiLength);
}

void CString::Assign(_INOUT CString vString)
{
	BETAONLY(_ASSERTE(!this->Locked()));

	//CString range sets [m_szValue] to nullptr to prevent freeing up memory that it doesn't own
	if (m_bMemoryAllocated && m_szValue) { StringFree(m_szValue); }

	BETAONLY(m_uiLocks = vString.m_uiLocks);

	m_szValue          = vString.m_szValue;
	m_uiLength         = vString.m_uiLength;
	m_bMemoryAllocated = vString.m_bMemoryAllocated;

	//Important! - this is where the source string looses control over the memory block (if it ever had any control)
	vString.m_bMemoryAllocated = false;
}

bool CString::operator ==(_IN CString& rString) const
{
	if (m_uiLength != rString.m_uiLength) { return false; }

	return (wmemcmp(m_szValue, rString.m_szValue, m_uiLength) == 0);
}

bool CString::operator ==(_IN LPTSTR szText) const
{
	if (m_uiLength != (UINT)wcslen(szText)) { return false; }

	return (wmemcmp(m_szValue, szText, m_uiLength) == 0);
}

bool CString::operator ==(_IN char* szText) const
{
	if (m_uiLength != (UINT)strlen(szText)) { return false; }

    for (UINT i = 0; i < m_uiLength; i++)
    {
        if ((char)m_szValue[i] != szText[i]) { return false; }
    }

	return true;
}

bool CString::operator ==(_IN TCHAR cChar) const
{
	if (m_uiLength != 1) { return false; }

	return (m_szValue[0] == cChar);
}

bool CString::operator <=(_IN CString& rString) const
{
    #ifdef WIN32
	int iResult = CompareString(LOCALE_INVARIANT, 0, m_szValue, m_uiLength, rString.m_szValue, rString.m_uiLength);

	return (iResult == CSTR_EQUAL) || (iResult == CSTR_LESS_THAN);
	#else
	return wcscmp(m_szValue, rString.Value()) <= 0;
	#endif
}

bool CString::operator <=(_IN LPTSTR szText) const
{
    #ifdef WIN32
	int iResult = CompareString(LOCALE_INVARIANT, 0, m_szValue, m_uiLength, szText, m_uiLength) == CSTR_EQUAL;

	return (iResult == CSTR_EQUAL) || (iResult == CSTR_LESS_THAN);
	#else
	return wcscmp(m_szValue, szText) <= 0;
	#endif
}

bool CString::IsEqualCI(_IN CString& rString) const
{
	if (m_uiLength != rString.m_uiLength) { return false; }

    #ifdef WIN32
	return (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, m_szValue, m_uiLength, rString.m_szValue, m_uiLength) == CSTR_EQUAL);
	//return (_wcsnicmp(m_szValue, rString.m_szValue, m_uiLength) == 0);
	#else
	return wcscasecmp(m_szValue, rString.Value()) == 0;
	#endif
}

bool CString::IsEqualCI(_IN LPTSTR szText) const
{
    #ifdef WIN32
	return (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, m_szValue, m_uiLength, szText, m_uiLength) == CSTR_EQUAL);
	//return (_wcsnicmp(m_szValue, szText, m_uiLength) == 0);
	#else
	return wcscasecmp(m_szValue, szText) == 0;
	#endif
}

//Allows to test for object equality
bool CString::IsEqualTo(_IN CObject& rObject) const
{
	return rObject.ToString().IsEqualCI(*this);
}

void CString::operator +=(_IN CString& rString)
{
	if (m_bMemoryAllocated)
	{
		this->InternalResize(m_uiLength + rString.m_uiLength);
	}
	else
	{
		LPTSTR sText = m_szValue;
		this->InternalSize(m_uiLength + rString.m_uiLength);

		if (sText) { String_CopyChars(m_szValue, sText, m_uiLength); }
	}

	String_CopyChars(&m_szValue[m_uiLength - rString.m_uiLength], rString.m_szValue, rString.m_uiLength);
}

void CString::operator +=(_IN TCHAR cChar)
{
	if (m_bMemoryAllocated)
	{
		this->InternalResize(m_uiLength + 1);
	}
	else
	{
		LPTSTR sText = m_szValue;
		this->InternalSize(m_uiLength + 1);

		if (sText) { String_CopyChars(m_szValue, sText, m_uiLength); }
	}

	m_szValue[m_uiLength - 1] = cChar;
}

void CString::Resize(_IN UINT uiLength)
{
	if (m_bMemoryAllocated)
	{
		this->InternalResize(uiLength);
	}
	else
	{
		LPTSTR sText = m_szValue;
		this->InternalSize(uiLength);

		_PENDING; //The " && (sText != m_szValue)" wasn't here a long time ..
		if (sText && (sText != m_szValue)) { String_CopyChars(m_szValue, sText, m_uiLength); }
	}
}

void CString::Clear()
{
	if (m_bMemoryAllocated)
	{
		this->InternalResize(0);
	}
	else
	{
		this->InternalSize(0);
	}
}

void CString::InternalSize(_IN UINT uiLength)
{
	_ASSERTE(!this->m_bMemoryAllocated);

	m_uiLength = uiLength;
	m_szValue = StringAlloc(m_uiLength);

	m_szValue[m_uiLength] = 0;

	m_bMemoryAllocated = true;
}

void CString::InternalResize(_IN UINT uiLength)
{
	_ASSERTE(this->m_bMemoryAllocated);
	BETAONLY(_ASSERTE(!this->Locked());)

	m_szValue = StringRealloc(m_szValue, m_uiLength, uiLength);
	m_uiLength = uiLength;

	m_szValue[m_uiLength] = 0;
}

UINT CString::BytesReserved() _GET
{
	if (!m_szValue || !m_bMemoryAllocated) { return 0; }

	SCFPrivate::CFSBHeap* pHeap = *((SCFPrivate::CFSBHeap**)m_szValue - 1);
	if (pHeap)
	{
		return pHeap->BlockSize() - sizeof(void*);
	}
	else
	{
        #ifdef WIN32
        return ((UINT)_msize            ((void**)m_szValue - 1) - sizeof(void*));
        #else
        return ((UINT)malloc_usable_size((void**)m_szValue - 1) - sizeof(void*));
        #endif
    }
}

void CString::BytesReserve(_IN UINT uiCount) _SET
{
	//We want only to enlarge the memory block(shrinking is pointless), however [CharsReserved()] returns 0 if the string references the text & does not own any memory, so we need a special case for this
	if (uiCount > BytesReserved())
	{
		//If the string owns a chunk of memory, we just enlarge it, [StringRealloc(..)] never shrinks a memory block, so there is no need to specially handle such cases
		if (m_bMemoryAllocated) { m_szValue = StringRealloc(m_szValue, m_uiLength, uiCount / sizeof(TCHAR)); }
		else
		{
			//If we we don't own any memory, we need to alloc a new chunk & copy the referenced text
			LPTSTR sText = m_szValue;

			//We need to allocate at least enough space for the currently referenced text
			m_szValue = StringAlloc(__max(uiCount / sizeof(TCHAR), m_uiLength));
			m_bMemoryAllocated = true;

			//Now we need to actually perform the copy(if needed) & terminate the string
			if (sText) { String_CopyChars(m_szValue, sText, m_uiLength); m_szValue[m_uiLength] = 0; }
			else       { m_szValue[0] = 0; }
		}
	}
}

UINT CString::CharsReserved() _GET
{
	if (!m_szValue || !m_bMemoryAllocated) { return 0; }

	SCFPrivate::CFSBHeap* pHeap = *((SCFPrivate::CFSBHeap**)m_szValue - 1);
	if (pHeap)
	{
		return (pHeap->BlockSize() - sizeof(void*)) / sizeof(TCHAR);
	}
	else
	{
        #ifdef WIN32
        return ((UINT)_msize            ((void**)m_szValue - 1) - sizeof(void*)) / sizeof(TCHAR);
        #else
        return ((UINT)malloc_usable_size((void**)m_szValue - 1) - sizeof(void*)) / sizeof(TCHAR);
        #endif
    }
}

void CString::CharsReserve(_IN UINT uiCount) _SET
{
	//We want only to enlarge the memory block(shrinking is pointless), however [CharsReserved()] returns 0 if the string references the text & does not own any memory, so we need a special case for this
	if (uiCount > CharsReserved())
	{
		//If the string owns a chunk of memory, we just enlarge it, [StringRealloc(..)] never shrinks a memory block, so there is no need to specially handle such cases
		if (m_bMemoryAllocated) { m_szValue = StringRealloc(m_szValue, m_uiLength, uiCount); }
		else
		{
			//If we we don't own any memory, we need to alloc a new chunk & copy the referenced text
			LPTSTR sText = m_szValue;

			//We need to allocate at least enough space for the currently referenced text
			m_szValue = StringAlloc(__max(uiCount, m_uiLength));
			m_bMemoryAllocated = true;

			//Now we need to actually perform the copy(if needed) & terminate the string
			if (sText) { String_CopyChars(m_szValue, sText, m_uiLength); m_szValue[m_uiLength] = 0; }
			else       { m_szValue[0] = 0; }
		}
	}
}

void CString::LengthScan()
{
	m_uiLength = (UINT)wcslen(m_szValue);
}

void CString::ToASCII(_OUT char* szText) const
{
	for (UINT i = 0; i <= m_uiLength; i++)
	{
		szText[i] = (char)m_szValue[i];
	}
}

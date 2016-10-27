#include "Errors.h"

#include "Int.h"
#include "String.h"
#include "DictionaryInt64.h"
#include "StreamWrite.h"
#include "StreamWriteTextUCS2.h"
#include "MemoryBlock.h"
#include "StreamMemoryWrite.h"

ENUM Errors_eLast = 0;
CDictionaryInt64<CString> Errors_Strings;

IStreamWriteText* Errors_pStreamText = NULL;

struct SDefault
{
	CMemoryBlock*         pStorage;
	CStreamMemoryWrite*   pStream;
	CStreamWriteTextUCS2* pStreamText;

} Errors_Default = { NULL, NULL, NULL };


CString* CError::String(_IN ENUM eError)
{
	return Errors_Strings.At(eError);
}

void CError::String(_IN ENUM eError, _IN _REF CString& rString)
{
	_ASSERTE(!Errors_Strings.ContainsKey(eError));

	Errors_Strings.AtPut(eError, rString);
}

ENUM CError::Last() { return Errors_eLast; }

void CError::Last(_IN ENUM eError)
{
	Errors_eLast = eError;

	if (Errors_pStreamText)
	{
		Errors_pStreamText->PutString(STRING("Error "));
		Errors_pStreamText->PutString(CInt(eError).ToString());
	}

#ifdef _DEBUG
	if (_CrtDbgReportW(_CRT_ERROR, NULL, 0, NULL, SCFTEXT("Error code: %i"), eError) == 1) { _CrtDbgBreak(); }
#endif
}
void CError::Last(_IN ENUM eError, _IN CString& rFuncSig)
{
	Errors_eLast = eError;

	if (Errors_pStreamText)
	{
		Errors_pStreamText->PutString(STRING("Error "));
		Errors_pStreamText->PutString(CInt(eError).ToString());
		Errors_pStreamText->PutString(STRING(": "));
		Errors_pStreamText->PutString(rFuncSig);

		CString* pErrorString = Errors_Strings.At(eError);
		if (pErrorString)
		{
			Errors_pStreamText->PutString(STRING(": "));
			Errors_pStreamText->PutString(*pErrorString);
		}
	}

#ifdef _DEBUG
	if (_CrtDbgReportW(_CRT_ERROR, NULL, 0, NULL, SCFTEXT("Error in function: %s\nError code: %i"), rFuncSig.Value(), eError) == 1) { _CrtDbgBreak(); }
#endif
}

CString* CError::LastString() { return String(Errors_eLast); }

void CError::Reset() { Errors_eLast = 0; }

void SCFErrorInitialize()
{
	Errors_Default.pStorage    = new CMemoryBlock(4096);
	Errors_Default.pStream     = new CStreamMemoryWrite(*Errors_Default.pStorage);
	Errors_Default.pStreamText = new CStreamWriteTextUCS2(*Errors_Default.pStream);

	Errors_pStreamText = Errors_Default.pStreamText;
}

void SCFErrorCleanUp()
{
	delete Errors_Default.pStreamText;
	delete Errors_Default.pStream;
	delete Errors_Default.pStorage;
}

CMemoryBlock* CError::DefaultStorage() { return Errors_Default.pStorage; }

void              CError::Stream(_INOUT _REF IStreamWriteText* pStream) { Errors_pStreamText = pStream; }
IStreamWriteText* CError::Stream()                                      { return Errors_pStreamText; }

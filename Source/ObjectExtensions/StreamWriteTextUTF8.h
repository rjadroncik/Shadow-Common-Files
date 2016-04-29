#pragma once
#include "StreamWriteText.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamWriteTextUTF8 : public CStreamWriteText
	{
	public:
		CString ToString() _GET { return STRING("{StreamWriteTextUTF8}"); }

	public:
		CStreamWriteTextUTF8(_INOUT IStreamWrite& rStreamWrite, _IN bool bUseBOM = TRUE);
		virtual ~CStreamWriteTextUTF8();

	public:
		void PutString(_IN CString& rString);
		void PutString(_IN SCF::LPTSTR szString);
		void PutString(_IN SCF::LPTSTR szString, _IN SCF::UINT uiLength);
	
		void PutLine(_IN CString& rString);
		void PutLine();

		void PutChar(_IN SCF::TCHAR cChar);

	public:
		SCF::UINT CharsWritten() _GET; 

	public:
		SCF::ENUM Encoding() _GET { return EncodingUTF8; }
	};
};
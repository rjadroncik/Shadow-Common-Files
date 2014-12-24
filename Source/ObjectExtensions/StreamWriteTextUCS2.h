#pragma once
#include "StreamWriteText.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamWriteTextUCS2 : public CStreamWriteText
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamWriteTextUCS2; }
		CString   ToString() _GET { return STRING("{StreamWriteTextUCS2}"); }

	public:
		CStreamWriteTextUCS2(_INOUT IStreamWrite& rStreamWrite, _IN bool bUseBOM = TRUE);
		virtual ~CStreamWriteTextUCS2();

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
		SCF::ENUM Encoding() _GET { return EncodingUCS2; }
	};
};
#pragma once
#include "StreamWriteText.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamWriteTextASCII : public CStreamWriteText
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamWriteTextASCII; }
		CString   ToString() _GET { return STRING("{StreamWriteTextASCII}"); }

	public:
		CStreamWriteTextASCII(_INOUT IStreamWrite& rStreamWrite);
		virtual ~CStreamWriteTextASCII();

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
		SCF::ENUM Encoding() _GET { return EncodingASCII; }
	};
};
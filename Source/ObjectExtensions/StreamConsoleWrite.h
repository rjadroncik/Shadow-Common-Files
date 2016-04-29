#pragma once
#include "StreamWriteText.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamConsoleWrite : public CStream, public virtual IStreamWriteText
	{
	public:
		CString ToString() _GET { return STRING("{StreamConsoleWrite}"); }

	public:
		CStreamConsoleWrite();
		virtual ~CStreamConsoleWrite();

	public:
		void PutString(_IN CString& rString);
		void PutString(_IN SCF::LPTSTR szString);
		void PutString(_IN SCF::LPTSTR szString, _IN SCF::UINT uiLength);
		
		void PutLine(_IN CString& rString);
		void PutLine();
	
		void PutChar(_IN SCF::TCHAR cChar);

	public:
		SCF::UINT CharsWritten() _GET { return 0; } 

	public:
		void Close() {}
		bool IsOpen() _GET { return TRUE; }

	public:
		SCF::ENUM Encoding() _GET { return EncodingUCS2; }
	};
};
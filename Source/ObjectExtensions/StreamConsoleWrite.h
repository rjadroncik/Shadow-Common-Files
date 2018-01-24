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
		void PutString(_IN LPTSTR szString);
		void PutString(_IN LPTSTR szString, _IN UINT uiLength);
		
		void PutLine(_IN CString& rString);
		void PutLine();
	
		void PutChar(_IN TCHAR cChar);

	public:
		UINT CharsWritten() _GET { return 0; } 

	public:
		void Close() {}
		bool IsOpen() _GET { return true; }

	public:
		ENUM Encoding() _GET { return EncodingUCS2; }
	};
};
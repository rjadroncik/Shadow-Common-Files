#pragma once
#include "StreamWriteText.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamWriteTextASCII : public CStreamWriteText
	{
	public:
		CString ToString() _GET { return STRING("{StreamWriteTextASCII}"); }

	public:
		CStreamWriteTextASCII(_INOUT IStreamWrite& rStreamWrite);
		virtual ~CStreamWriteTextASCII();

	public:
		void PutString(_IN CString& rString);
		void PutString(_IN LPTSTR szString);
		void PutString(_IN LPTSTR szString, _IN UINT uiLength);
	
		void PutLine(_IN CString& rString);
		void PutLine();
	
		void PutChar(_IN TCHAR cChar);

	public:
		UINT CharsWritten() _GET; 

	public:
		ENUM Encoding() _GET { return EncodingASCII; }
	};
};
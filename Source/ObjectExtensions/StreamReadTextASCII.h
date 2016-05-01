#pragma once
#include "StreamReadText.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamReadTextASCII : public CStreamReadText
	{
	public:
		CString ToString() _GET { return STRING("{StreamReadTextASCII}"); }

	public:
		CStreamReadTextASCII(_INOUT IStreamRead& rStreamRead);
		virtual ~CStreamReadTextASCII();

	public:
		bool GetString(_OUT CString& rOutString, _IN UINT uiLength);
		bool GetLine  (_OUT CString& rOutString);

		TCHAR GetChar();

		bool CharsLeft() _GET;

	public:
		ENUM Encoding() _GET { return EncodingASCII; }
	};
};
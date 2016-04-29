#pragma once
#include "StreamReadText.h"

namespace SCFBase
{

	class OBJECT_EXTENSIONS_API CStreamReadTextUCS2 : public CStreamReadText
	{
	public:
		CString ToString() _GET { return STRING("{StreamReadTextUCS2}"); }

	public:
		CStreamReadTextUCS2(_INOUT IStreamRead& rStreamRead);
		virtual ~CStreamReadTextUCS2();

	public:
		bool GetString(_OUT CString& rOutString, _IN SCF::UINT uiLength);
		bool GetLine  (_OUT CString& rOutString);

		SCF::TCHAR GetChar();

		bool CharsLeft() _GET;

	public:
		SCF::ENUM Encoding() _GET { return EncodingUCS2; }
	};
};
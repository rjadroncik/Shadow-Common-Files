#pragma once
#include "StreamReadText.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamReadTextUTF8 : public CStreamReadText
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamReadTextUTF8; }
		CString   ToString() _GET { return STRING("{StreamReadTextUTF8}"); }

	public:
		CStreamReadTextUTF8(_INOUT IStreamRead& rStreamRead);
		virtual ~CStreamReadTextUTF8();

	public:
		bool GetString(_OUT CString& rOutString, _IN SCF::UINT uiLength);
		bool GetLine  (_OUT CString& rOutString);

		SCF::TCHAR GetChar();

		bool CharsLeft() _GET;

	public:
		SCF::ENUM Encoding() _GET { return EncodingUTF8; }
	};
};
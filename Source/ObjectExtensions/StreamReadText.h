#pragma once

#include "StreamRead.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API IStreamReadText : virtual public IStreamBase
	{
	public:
		virtual bool GetString(_OUT CString& rOutString, _IN SCF::UINT uiLength) = 0;
		virtual bool GetLine  (_OUT CString& rOutString) = 0;

	public:
		virtual SCF::TCHAR GetChar() = 0;

	public:
		virtual bool CharsLeft() _GET = 0;

	public:
		virtual SCF::ENUM Encoding() _GET = 0;
	};

	class OBJECT_EXTENSIONS_API CStreamReadText : public CStream, public virtual IStreamReadText
	{
	public:
		CStreamReadText(_INOUT IStreamRead& rStreamRead);
		virtual ~CStreamReadText();

	public:
		void Close()       { m_pStream->Close(); }
		bool IsOpen() _GET { return m_pStream->IsOpen(); }

	protected:
		IStreamRead* m_pStream;
	};
};
#pragma once
#include "StreamReadText.h"

namespace SCFBase
{
	//This is a special proxy class, it determines the nature of the underlying
	//text file encoding & creates & maintains the corresponding read stream
	class OBJECT_EXTENSIONS_API CStreamReadTextGeneric : public CStream, public virtual IStreamReadText
	{
	public:
		CString ToString() _GET { return STRING("{StreamReadTextGeneric}"); }

	public:
		CStreamReadTextGeneric(_INOUT IStreamRead& rStreamRead);
		virtual ~CStreamReadTextGeneric();

	public:
		bool GetString(_OUT CString& rOutString, _IN SCF::UINT uiLength);
		bool GetLine  (_OUT CString& rOutString);

		SCF::TCHAR GetChar();

		bool CharsLeft() _GET;

	public:
		void Close()       { m_pStream->Close(); }
		bool IsOpen() _GET { return m_pStream->IsOpen(); }

	public:
		SCF::ENUM Encoding() _GET { return m_pStream->Encoding(); }

	private:
		CStreamReadText* m_pStream;
	};
};
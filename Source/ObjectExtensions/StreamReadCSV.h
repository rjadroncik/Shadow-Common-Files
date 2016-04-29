#pragma once
#include "StreamReadText.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamReadCSV : public CStream
	{
	public:
		static void       Separator(_IN SCF::TCHAR usChar);
		static SCF::TCHAR Separator();

	public:
		CString ToString() _GET { return STRING("{StreamReadCSV}"); }

	public:
		CStreamReadCSV(_INOUT IStreamReadText& rStreamRead);
		virtual ~CStreamReadCSV();

	public:
		bool GetValue(_OUT CString& rOutString);

	public:
		bool BlockNext();

	public:
		void Close()  { m_pStream->Close(); }
		bool IsOpen() _GET { return m_pStream->IsOpen(); }

	protected:
		IStreamReadText* m_pStream;

	protected:
		CString   m_csBlock;
		SCF::UINT m_uiBlockNextChar;
	};
};
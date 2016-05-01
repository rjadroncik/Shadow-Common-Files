#pragma once
#include "StreamFileWrite.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API IStreamWriteText : virtual public IStreamBase
	{
	public:
		virtual void PutString(_IN CString& rString) = 0;
		virtual void PutString(_IN LPTSTR szString) = 0;
		virtual void PutString(_IN LPTSTR szString, _IN UINT uiLength) = 0;
	
		virtual void PutLine(_IN CString& rString) = 0;
		virtual void PutLine() = 0;

		virtual void PutChar(_IN TCHAR cChar) = 0;

	public:
		virtual UINT CharsWritten() _GET = 0; 

	public:
		virtual ENUM Encoding() _GET = 0;
	};

	class OBJECT_EXTENSIONS_API CStreamWriteText : public CStream, public virtual IStreamWriteText
	{
	public:
		CStreamWriteText(_INOUT IStreamWrite& rStreamWrite);
		virtual ~CStreamWriteText();

	public:
		void Close()       { m_pStream->Close(); }
		bool IsOpen() _GET { return m_pStream->IsOpen(); }

		bool UseCR() _GET { return m_bUseCR; }
		void UseCR(_IN bool bUseCR) { m_bUseCR = bUseCR; }

	protected:
		IStreamWrite* m_pStream;
		bool m_bUseCR;
	};
};
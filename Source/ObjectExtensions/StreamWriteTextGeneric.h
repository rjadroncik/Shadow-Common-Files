#pragma once
#include "StreamWriteText.h"

namespace SCFBase
{
	//This is a special proxy class, it determines the nature of the underlying
	//text file encoding & creates & maintains the corresponding read stream
	class OBJECT_EXTENSIONS_API CStreamWriteTextGeneric : public CStream, public virtual IStreamWriteText
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamWriteTextGeneric; }
		CString   ToString() _GET { return STRING("{StreamWriteTextGeneric}"); }

	public:
		CStreamWriteTextGeneric(_INOUT IStreamWrite& rStreamWrite, _IN SCF::ENUM eEncoding = EncodingUCS2, _IN bool bUseBOM = TRUE);
		virtual ~CStreamWriteTextGeneric();

	public:
		void PutString(_IN CString& rString);
		void PutString(_IN SCF::LPTSTR szString);
		void PutString(_IN SCF::LPTSTR szString, _IN SCF::UINT uiLength);

		void PutLine(_IN CString& rString);
		void PutLine();

		void PutChar(_IN SCF::TCHAR cChar);

	public:
		SCF::UINT CharsWritten() _GET; 

	public:
		void Close()       { m_pStream->Close(); }
		bool IsOpen() _GET { return m_pStream->IsOpen(); }

	public:
		SCF::ENUM Encoding() _GET { return m_pStream->Encoding(); }

	private:
		CStreamWriteText* m_pStream;
	};
};
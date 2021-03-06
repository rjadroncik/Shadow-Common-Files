#pragma once
#include "StreamWriteText.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamWriteCSV : public CStream
	{
	public:
		static void       Separator(_IN TCHAR usChar);
		static TCHAR Separator();

	public:
		CString ToString() _GET { return STRING("{StreamWriteCSV}"); }

	public:
		CStreamWriteCSV(_INOUT IStreamWriteText& rStreamWrite);
		virtual ~CStreamWriteCSV();

	public:
		void PutValue(_IN CObject& rValue);
	
	public:
		void BlockNext();

	public:
		void Close();
		bool IsOpen() _GET { return m_pStream->IsOpen(); }

	protected:
		IStreamWriteText* m_pStream;

	protected:
		CString m_csBlock;
	};
};
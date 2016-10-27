#pragma once
#include "File.h"
#include "Directory.h"
#include "Record.h"
#include "RecordFile.h"
#include "RecordDirectory.h"

using namespace SCFBase;

namespace SCFDatafileIO
{
	template class __declspec(dllexport) SCFBase::CDictionaryString<SCFDatafileIOPrivate::CRecord>;	

	class DATAFILEIO_API CDataPool;

	class DATAFILEIO_API CDatafile : public CFile
	{
		friend class DATAFILEIO_API CDFFile;
		friend class DATAFILEIO_API CDFStreamFileRead;
		friend class DATAFILEIO_API CDFDirectory;
		friend class DATAFILEIO_API CDataPool;

	public:
		CDatafile(_IN CString& rFullNameOrPath, _IN bool bKeepFileOpen = FALSE);
		CDatafile(_IN CString& rPath, _IN CString& rName, _IN CString& rExtension, _IN bool bKeepFileOpen = FALSE);
		~CDatafile();

	public:
		CString CWD()                       _GET { return STRINGREF(m_CWD); }
		void    CWD(_IN CString&      rCWD) _SET { m_CWD = CDFDirectory(*this, rCWD).PathFull(); }
		void    CWD(_IN CDFDirectory& rCWD) _SET { m_CWD = rCWD.PathFull(); }

	public:
		UINT IOBufferSize()                     _GET { return m_uiIOBufferSize; }
		void      IOBufferSize(_IN UINT uiSize) _SET { m_uiIOBufferSize = uiSize; }

	public:
		bool WriteAs(_INOUT CFile& rFile);

	public:
		bool Write();
		bool Read();

	public:
		void PasswordEnter (_IN CString& rString);
		void PasswordChange(_IN CString& rString);

	protected:
		UINT64 HeaderSize();
		bool   HeaderWrite(_INOUT void* hFile);
		bool   HeaderRead (_INOUT void* hFile);

	protected:
		bool Write(_INOUT void* hFile);

	protected:
		bool FilesWrite(_INOUT void* hFile);
		bool FileWrite(_INOUT SCFDatafileIOPrivate::CRecordFile& rRecord, _INOUT CMemoryBlock& rIOBuffer,
			_OUT CStreamFileWrite& rStreamWrite, _IN UINT64 ui64HeaderSize);
	
		void RecordsWrite(_INOUT IStreamWrite& rStream);
		void RecordsRead(_INOUT IStreamRead& rStream);

	protected:
		UINT64 FileWritePassThrough    (_INOUT IStreamRead& rStreamRead, _INOUT CMemoryBlock& rIOBuffer, _OUT IStreamWrite& rStreamWrite);
		
		UINT64 FileWriteCompress       (_INOUT IStreamRead& rStreamRead, _INOUT CMemoryBlock& rIOBuffer, _OUT IStreamWrite& rStreamWrite);
		UINT64 FileWriteUncompress     (_INOUT IStreamRead& rStreamRead, _INOUT CMemoryBlock& rIOBuffer, _OUT IStreamWrite& rStreamWrite);
	
		UINT64 FileWriteEncrypt        (_INOUT IStreamRead& rStreamRead, _INOUT CMemoryBlock& rIOBuffer, _OUT IStreamWrite& rStreamWrite);
		UINT64 FileWriteDecrypt        (_INOUT IStreamRead& rStreamRead, _INOUT CMemoryBlock& rIOBuffer, _OUT IStreamWrite& rStreamWrite);

		UINT64 FileWriteEncryptCompress  (_INOUT IStreamRead& rStreamRead, _INOUT CMemoryBlock& rIOBuffer, _OUT IStreamWrite& rStreamWrite);
		UINT64 FileWriteEncryptUncompress(_INOUT IStreamRead& rStreamRead, _INOUT CMemoryBlock& rIOBuffer, _OUT IStreamWrite& rStreamWrite);
		UINT64 FileWriteDecryptCompress  (_INOUT IStreamRead& rStreamRead, _INOUT CMemoryBlock& rIOBuffer, _OUT IStreamWrite& rStreamWrite);
		UINT64 FileWriteDecryptUncompress(_INOUT IStreamRead& rStreamRead, _INOUT CMemoryBlock& rIOBuffer, _OUT IStreamWrite& rStreamWrite);

	protected:
		//Persistent data
		CDictionaryString<SCFDatafileIOPrivate::CRecord> m_Records;
		BYTE m_ucAttributes;

	protected:
		//Temporary data/settings
		CString m_CWD;

		//Contains a valid read stream associated with this datafile only if the
		//[bKeepFileOpen = TRUE] at object creation, otherwise it is [NULL]
		CStreamFileRead* m_pStreamRead;

		UINT m_uiIOBufferSize;
		CMemoryBlock* m_pEncryptionKey;
	};
};



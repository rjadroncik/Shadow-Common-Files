#pragma once
#include <SCFObjectExtensions.h>
#include "Errors.h"
#include "Record.h"

using namespace SCFBase;

namespace SCFDatafileIO
{
	class DATAFILEIO_API CDatafile;
	class DATAFILEIO_API CDFFile;
	class DATAFILEIO_API CDFStreamFileRead;
};

namespace SCFDatafileIOPrivate
{
	class CRecordFile : public CRecord
	{
		friend class DATAFILEIO_API SCFDatafileIO::CDatafile;
		friend class DATAFILEIO_API SCFDatafileIO::CDFFile;
		friend class DATAFILEIO_API SCFDatafileIO::CDFStreamFileRead;

	public:
		CRecordFile(_IN CRecordFile& rFile);
		CRecordFile(_IN CFile& rSource, _IN BYTE ucAttributes, _IN bool bSourceIsFileSystem);
		~CRecordFile();

	public:
		UINT64 Size() _GET { return m_ui64Size; }

	public:
		bool Compressed()                     _GET;
		bool Compressed(_IN bool bCompressed) _SET;

	public:
		CString XmlName() _GET { return STRING("File"); }

		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);

	protected: 
		//Persistent data
		UINT64 m_ui64Size;
	
		//File data, if the file is stored, these contain valid information
		UINT64 m_ui64DataOffset;
		UINT64 m_ui64DataSize;

	protected:
		//Upon saving, this variable is modified to correspond to the actual state of the file
		//within the datafile
		BYTE m_ucAttributesApplied;

		//The source contains a valid pointer if the file has a pending storage operation
		//The source can be either a regular file or a file within another or the same datafile
		CFile* m_pSource;
		bool m_bSourceIsFileSystem;

	private:
		//Exported, so it can be used during deserialization
		__declspec(dllexport) CRecordFile();
	};
};

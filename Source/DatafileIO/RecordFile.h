#pragma once
#include <SCFObjectExtensions.h>
#include "Classes.h"
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
		SCF::ENUM ClassKey() _GET { return ClassRecordFile; }

	public:
		CRecordFile(_IN CRecordFile& rFile);
		CRecordFile(_IN CFile& rSource, _IN SCF::BYTE ucAttributes);
		~CRecordFile();

	public:
		SCF::UINT64 Size() _GET { return m_ui64Size; }

	public:
		bool Compressed()                     _GET;
		bool Compressed(_IN bool bCompressed) _SET;

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);

	protected: 
		//Persistent data
		SCF::UINT64 m_ui64Size;
	
		//File data, if the file is stored, these contain valid information
		SCF::UINT64 m_ui64DataOffset;
		SCF::UINT64 m_ui64DataSize;

	protected:
		//Upon saving, this variable is modified to correspond to the actual state of the file
		//within the datafile
		SCF::BYTE m_ucAttributesApplied;

		//The source contains a valid pointer if the file has a pending storage operation
		//THe source can be either a regular file or a file within another or the same datafile
		CObject* m_pSource;

	private:
		//Exported, so it can be used during deserialization
		__declspec(dllexport) CRecordFile();
	};
};

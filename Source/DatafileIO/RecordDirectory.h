#pragma once
#include <SCFObjectExtensions.h>
#include "Classes.h"
#include "Record.h"

using namespace SCFBase;

namespace SCFDatafileIO
{
	class DATAFILEIO_API CDatafile;
	class DATAFILEIO_API CDFFile;
};

namespace SCFDatafileIOPrivate
{
	class CRecordDirectory : public CRecord
	{
		friend class DATAFILEIO_API SCFDatafileIO::CDatafile;
		friend class DATAFILEIO_API SCFDatafileIO::CDFFile;

	public:
		SCF::ENUM ClassKey() _GET { return ClassRecordDirectory; }

	public:
		CRecordDirectory(_IN CRecordDirectory& rDirectory);
		CRecordDirectory(_IN SCF::BYTE ucAttributes);
		~CRecordDirectory();

	public:
		//Exported, so it can be used during deserialization
		__declspec(dllexport) CRecordDirectory();

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);
 	};
};

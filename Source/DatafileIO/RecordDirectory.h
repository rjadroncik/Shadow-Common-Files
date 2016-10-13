#pragma once
#include <SCFObjectExtensions.h>
#include "Errors.h"
#include "Record.h"

using namespace SCFBase;

namespace SCFDatafileIO
{
	class DATAFILEIO_API CDatafile;
	class DATAFILEIO_API CDFFile;
	class DATAFILEIO_API CDFDirctory;
};

namespace SCFDatafileIOPrivate
{
	class CRecordDirectory : public CRecord
	{
		friend class DATAFILEIO_API SCFDatafileIO::CDatafile;
		friend class DATAFILEIO_API SCFDatafileIO::CDFFile;
		friend class DATAFILEIO_API SCFDatafileIO::CDFDirectory;

	public:
		CRecordDirectory(_IN CRecordDirectory& rDirectory);
		CRecordDirectory(_IN BYTE ucAttributes);
		~CRecordDirectory();

	protected:
		CRecordDirectory();
	};
};

#pragma once
#include <SCFObjectExtensions.h>
#include "Errors.h"
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
		CRecordDirectory(_IN CRecordDirectory& rDirectory);
		CRecordDirectory(_IN BYTE ucAttributes);
		~CRecordDirectory();

	public:
		//Exported, so it can be used during deserialization
		__declspec(dllexport) CRecordDirectory();

	public:
		CString XmlName() _GET { return STRING("Directory"); }

		void XMLSerialize  (_INOUT SCFXML::IXMLStreamWrite& rWriter) const;
		void XMLDeserialize(_INOUT SCFXML::IXMLStreamRead&  rReader);
	};
};

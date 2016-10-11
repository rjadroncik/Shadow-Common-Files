#pragma once
#include <SCFObjectExtensions.h>
#include "Errors.h"
#include "File.h"

using namespace SCFBase;

namespace SCFDatafileIOPrivate { class CRecordFile; };

namespace SCFDatafileIO
{
	class DATAFILEIO_API CDatafile;

	class DATAFILEIO_API CDFStreamFileRead : public CStreamFileRead
	{
		friend class DATAFILEIO_API CDatafile;

	public:
		//Throws a [ErrorFileNotFound]  if file is missing
		//Throws a [ErrorAccessDenied]  if file access denied by OS
		//Throws a [ErrorFileFailedOpen] if file opening failed for unknown(unhandled) reason
		CDFStreamFileRead(_IN CDFFile& rFile);
		virtual ~CDFStreamFileRead();
	
	protected:
		CDFStreamFileRead(_IN CFile& rFile, _IN SCFDatafileIOPrivate::CRecordFile& rFileRecord);
	};
};
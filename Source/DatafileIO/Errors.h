#pragma once

#ifdef DATAFILEIO_EXPORTS
#define DATAFILEIO_API __declspec(dllexport)
#else
#define DATAFILEIO_API __declspec(dllimport)
#endif

namespace SCFDatafileIO
{
	enum Errors
	{
		ErrorDatafileFormatWrong = 0x3001,
		ErrorDatafileFormatOld,
		ErrorDatafileFormatNew,
	
		ErrorDFFileSourceUnsupported,
		ErrorDFFileFailedAttributeGet,
		ErrorDFFileFailedAttributeSet,

		ErrorDFDirectoryFailedAttributeGet,
		ErrorDFDirectoryFailedAttributeSet,

		ErrorBadPath,
	};
};

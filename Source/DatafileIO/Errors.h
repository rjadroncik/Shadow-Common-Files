#pragma once

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

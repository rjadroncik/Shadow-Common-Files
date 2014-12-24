#pragma once

#ifdef DATAFILEIO_EXPORTS
#define DATAFILEIO_API __declspec(dllexport)
#else
#define DATAFILEIO_API __declspec(dllimport)
#endif

#include "Errors.h"

//Reserved class keys: 0x0251 - 0x0300

namespace SCFDatafileIO
{
	enum Classes
	{
		ClassDFFile = 0x251,
		ClassDFDirectory,
		ClassDFStreamFileRead,
		ClassDatafile,
		ClassDataCache,
	};
};

namespace SCFDatafileIOPrivate
{
	enum Classes
	{
		ClassRecordFile = 0x281,
		ClassRecordDirectory,
		ClassEnumeratorDirectory,
		ClassDFDictionary,
	};

	enum Attributes
	{
		AttributeCompressed = 0x01,
		AttributeEncrypted  = 0x02,
	};
};

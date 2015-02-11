#pragma once

#ifdef WIN32

#ifdef OBJECT_EXTENSIONS_EXPORTS
#define OBJECT_EXTENSIONS_API __declspec(dllexport)
#else
#define OBJECT_EXTENSIONS_API __declspec(dllimport)
#endif

#else

#define OBJECT_EXTENSIONS_API

#endif // WIN32

#include <SCFStandard.h>

//Reserved class keys: 0x0001 - 0x0100

namespace SCFBase
{
	enum Classes
	{
		//Basic values
		ClassBool = 0x0001,
		ClassFloat,
		ClassChar,
		ClassInt,
		ClassEnum,
		ClassInt64,
		ClassPointer,
		ClassDateTime,

		//Derived values
		ClassArrayInt,
		ClassArrayFloat,
		ClassInt2,
		ClassInt3,
		ClassInt4,
		ClassFloat2,
		ClassFloat3,
		ClassFloat4,
		ClassAssociation,
		ClassString,
		ClassStringRange,

		//Collections
		ClassStack,
		ClassVector,
		ClassVectorRange,
		ClassBagInt64,
		ClassBagInt16,
		ClassBagObject,
		ClassList,

		//Trees
		ClassTreeSimple,
		ClassTreeBinary,

		//Dictionaries
		ClassDictionaryInt64,
		ClassDictionaryString,
		ClassDictionaryObject,

		//Enumerators
		ClassEnumeratorDictionaryInt64,
		ClassEnumeratorDictionaryString,
		ClassEnumeratorDictionaryObject,
		ClassEnumeratorBagInt64,
		ClassEnumeratorBagInt16,
		ClassEnumeratorBagObject,
		ClassEnumeratorTreeRegular,
		ClassEnumeratorTreeBinary,
		ClassEnumeratorList,

		//Streams
		ClassStreamFileRead,
		ClassStreamFileWrite,
		ClassStreamMemoryRead,
		ClassStreamMemoryWrite,
		ClassStreamStringRead,
		ClassStreamStringWrite,
		ClassStreamConsoleWrite,
		ClassStreamDummyWrite,

		ClassStreamReadObject,
		ClassStreamReadTextGeneric,
		ClassStreamReadTextUCS2,
		ClassStreamReadTextASCII,
		ClassStreamReadTextUTF8,
		ClassStreamReadCSV,

		ClassStreamWriteObject,
		ClassStreamWriteTextGeneric,
		ClassStreamWriteTextUCS2,
		ClassStreamWriteTextASCII,
		ClassStreamWriteTextUTF8,
		ClassStreamWriteCSV,

		//File system classes
		ClassFile,
		ClassDirectory,

		//Helper classes
		ClassMemoryBlock,
		ClassStringSearch,

		//Object comparer classes
		ClassComparer,
		ClassComparerIdentity,
		ClassComparerBool,
		ClassComparerFloat,
		ClassComparerInt,
		ClassComparerString,

		//Data formats
		ClassFormatInt,
		ClassFormatFloat,
		ClassFormatDateTime,
	};
};

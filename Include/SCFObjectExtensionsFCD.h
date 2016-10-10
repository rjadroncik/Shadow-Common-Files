#pragma once

#include "../Source/ObjectExtensions/classes.h"

namespace SCFBase
{
	//Absolute base class
	class OBJECT_EXTENSIONS_API CObject;

	//Value base class
	class OBJECT_EXTENSIONS_API CValue;

	//Basic values
	class OBJECT_EXTENSIONS_API CBool;
	class OBJECT_EXTENSIONS_API CFloat;
	class OBJECT_EXTENSIONS_API CChar;
	class OBJECT_EXTENSIONS_API CInt;
	class OBJECT_EXTENSIONS_API CEnum;
	class OBJECT_EXTENSIONS_API CInt64;
	class OBJECT_EXTENSIONS_API CPointer;
	class OBJECT_EXTENSIONS_API CDateTime;

	//Derived values
	class OBJECT_EXTENSIONS_API CArrayInt;
	class OBJECT_EXTENSIONS_API CArrayFloat;
	class OBJECT_EXTENSIONS_API CFloat2;
	class OBJECT_EXTENSIONS_API CFloat3;
	class OBJECT_EXTENSIONS_API CFloat4;
	class OBJECT_EXTENSIONS_API CAssociation;
	class OBJECT_EXTENSIONS_API CString;
	class OBJECT_EXTENSIONS_API CStringRange;

	//Collections
	class OBJECT_EXTENSIONS_API CStack;
	class OBJECT_EXTENSIONS_API CVector;
	class OBJECT_EXTENSIONS_API CSetInt64;
	class OBJECT_EXTENSIONS_API CSetInt16;
	class OBJECT_EXTENSIONS_API CSetObject;
	class OBJECT_EXTENSIONS_API CList;

	//Trees
	class OBJECT_EXTENSIONS_API CTreeSimple;

	//Dictionaries
	class OBJECT_EXTENSIONS_API CDictionaryInt64;
	class OBJECT_EXTENSIONS_API CDictionaryStringRaw;
	class OBJECT_EXTENSIONS_API CDictionaryObject;

	//Enumerators
	class OBJECT_EXTENSIONS_API CEnumeratorDictionaryInt64;
	class OBJECT_EXTENSIONS_API CEnumeratorDictionaryString;
	class OBJECT_EXTENSIONS_API CEnumeratorDictionaryObject;
	class OBJECT_EXTENSIONS_API CEnumeratorSetInt64;
	class OBJECT_EXTENSIONS_API CEnumeratorSetInt16;
	class OBJECT_EXTENSIONS_API CEnumeratorSetObject;
	class OBJECT_EXTENSIONS_API CEnumeratorList;

	//Streams
	class OBJECT_EXTENSIONS_API CStreamFileRead;
	class OBJECT_EXTENSIONS_API CStreamFileWrite;
	class OBJECT_EXTENSIONS_API CStreamMemoryRead;
	class OBJECT_EXTENSIONS_API CStreamMemoryWrite;
	class OBJECT_EXTENSIONS_API CStreamStringRead;
	class OBJECT_EXTENSIONS_API CStreamStringWrite;
	class OBJECT_EXTENSIONS_API CStreamConsoleWrite;
	class OBJECT_EXTENSIONS_API CStreamDummyWrite;

	class OBJECT_EXTENSIONS_API CStreamReadObject;
	class OBJECT_EXTENSIONS_API CStreamReadText;
	class OBJECT_EXTENSIONS_API CStreamReadTextGeneric;
	class OBJECT_EXTENSIONS_API CStreamReadTextASCII;
	class OBJECT_EXTENSIONS_API CStreamReadTextUTF8;
	class OBJECT_EXTENSIONS_API CStreamReadTextUCS2;
	class OBJECT_EXTENSIONS_API CStreamReadCSV;
	class OBJECT_EXTENSIONS_API CStreamWriteObject;
	class OBJECT_EXTENSIONS_API CStreamWriteText;
	class OBJECT_EXTENSIONS_API CStreamWriteTextGeneric;
	class OBJECT_EXTENSIONS_API CStreamWriteTextASCII;
	class OBJECT_EXTENSIONS_API CStreamWriteTextUTF8;
	class OBJECT_EXTENSIONS_API CStreamWriteTextUCS2;
	class OBJECT_EXTENSIONS_API CStreamWriteCSV;

	//File system classes
	class OBJECT_EXTENSIONS_API CFile;
	class OBJECT_EXTENSIONS_API CDirectory;

	//Helper classes
	class OBJECT_EXTENSIONS_API CMemory;
	class OBJECT_EXTENSIONS_API CMemoryBlock;

	class OBJECT_EXTENSIONS_API CStringSearch;

	class OBJECT_EXTENSIONS_API CComparer;
	class OBJECT_EXTENSIONS_API CComparerIdentity;
};

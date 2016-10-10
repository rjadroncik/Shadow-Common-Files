#pragma once

//Reserved class keys: 0x0001 - 0x0100

#include <SCFStandard.h>

#define SCF_OBJECT_EXTENSIONS_2

#include "../Source/ObjectExtensions/Errors.h"

#include "../Source/ObjectExtensions/Object.h"

#include "../Source/ObjectExtensions/DictionaryInt64.h"
#include "../Source/ObjectExtensions/DictionaryString.h"
#include "../Source/ObjectExtensions/DictionaryObject.h"

#include "../Source/ObjectExtensions/Stack.h"
#include "../Source/ObjectExtensions/Vector.h"
#include "../Source/ObjectExtensions/VectorRange.h"
#include "../Source/ObjectExtensions/SetInt64.h"
#include "../Source/ObjectExtensions/SetObject.h"
#include "../Source/ObjectExtensions/List.h"

#include "../Source/ObjectExtensions/TreeSimple.h"

#include "../Source/ObjectExtensions/Bool.h"
#include "../Source/ObjectExtensions/Enum.h"
#include "../Source/ObjectExtensions/Float.h"
#include "../Source/ObjectExtensions/Char.h"
#include "../Source/ObjectExtensions/Int.h"
#include "../Source/ObjectExtensions/Int64.h"
#include "../Source/ObjectExtensions/Pointer.h"
#include "../Source/ObjectExtensions/DateTime.h"

#include "../Source/ObjectExtensions/Array.h"
#include "../Source/ObjectExtensions/ArrayFloat.h"

#include "../Source/ObjectExtensions/Float2.h"
#include "../Source/ObjectExtensions/Float3.h"
#include "../Source/ObjectExtensions/Float4.h"

#include "../Source/ObjectExtensions/ArrayInt.h"
#include "../Source/ObjectExtensions/Association.h"
#include "../Source/ObjectExtensions/String.h"
#include "../Source/ObjectExtensions/StringRange.h"

#include "../Source/ObjectExtensions/EnumeratorDictionaryInt64.h"
#include "../Source/ObjectExtensions/EnumeratorDictionaryString.h"
#include "../Source/ObjectExtensions/EnumeratorDictionaryObject.h"
#include "../Source/ObjectExtensions/EnumeratorSetInt64.h"
#include "../Source/ObjectExtensions/EnumeratorSetObject.h"
#include "../Source/ObjectExtensions/EnumeratorList.h"

#include "../Source/ObjectExtensions/File.h"
#include "../Source/ObjectExtensions/Directory.h"

#include "../Source/ObjectExtensions/StreamFileRead.h"
#include "../Source/ObjectExtensions/StreamFileWrite.h"
#include "../Source/ObjectExtensions/StreamMemoryRead.h"
#include "../Source/ObjectExtensions/StreamMemoryWrite.h"
#include "../Source/ObjectExtensions/StreamStringRead.h"
#include "../Source/ObjectExtensions/StreamStringWrite.h"
#include "../Source/ObjectExtensions/StreamDummyWrite.h"
#include "../Source/ObjectExtensions/StreamConsoleWrite.h"

#include "../Source/ObjectExtensions/StreamReadText.h"
#include "../Source/ObjectExtensions/StreamReadTextGeneric.h"
#include "../Source/ObjectExtensions/StreamReadTextASCII.h"
#include "../Source/ObjectExtensions/StreamReadTextUTF8.h"
#include "../Source/ObjectExtensions/StreamReadTextUCS2.h"
#include "../Source/ObjectExtensions/StreamReadCSV.h"
#include "../Source/ObjectExtensions/StreamWriteText.h"
#include "../Source/ObjectExtensions/StreamWriteTextGeneric.h"
#include "../Source/ObjectExtensions/StreamWriteTextASCII.h"
#include "../Source/ObjectExtensions/StreamWriteTextUTF8.h"
#include "../Source/ObjectExtensions/StreamWriteTextUCS2.h"
#include "../Source/ObjectExtensions/StreamWriteCSV.h"

#include "../Source/ObjectExtensions/Memory.h"
#include "../Source/ObjectExtensions/MemoryBlock.h"

#include "../Source/ObjectExtensions/StringSearch.h"

#include "../Source/ObjectExtensions/Comparer.h"
#include "../Source/ObjectExtensions/ComparerIdentity.h"
#include "../Source/ObjectExtensions/ComparerInt.h"

#include "../Source/ObjectExtensions/InfoCPU.h"

#include "../Source/ObjectExtensions/Format.h"
#include "../Source/ObjectExtensions/FormatFloat.h"
#include "../Source/ObjectExtensions/FormatInt.h"

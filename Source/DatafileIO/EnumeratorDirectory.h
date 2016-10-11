#pragma once
#include <SCFObjectExtensions.h>
#include "Errors.h"
#include "Record.h"

namespace SCFDatafileIOPrivate
{
	class CEnumeratorDirectory : public CEnumeratorDictionaryString<CRecord>
	{
		//friend class OBJECT_EXTENSIONS_API CDictionaryString;

	public:
		CEnumeratorDirectory(_IN CDictionaryString<CRecord>& rDictionary, _IN CString& rDirectoryPath);
		virtual ~CEnumeratorDirectory();

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { return FALSE; }

	protected:
		//This function is called until a node which contains an object is found or enumeration ends
		//(not all nodes contain objects)
		bool NextNode();
	};
};
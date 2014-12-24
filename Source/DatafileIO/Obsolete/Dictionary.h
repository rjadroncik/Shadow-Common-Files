#pragma once
#include <SCFObjectExtensions.h>
#include "Classes.h"

using namespace SCFObjectExtensions;

namespace SCFDatafileIOPrivate
{
	class CDFDictionary : public CDictionaryString
	{
	public:
		//Dynamic casting
		void* AsType(_IN ENUM eType) const; 
		ENUM ClassKey() _GET { return ClassDFDictionary; }

	public:
		//Renames a directory (this modifies the paths of all objects in the 
		//hierarchy below the renamed object)

		//Example:
		//We have 3 objects:
		// {root}
		// |_ directory01
		//    |_directory02
		//    |_ file01
		//    |_ file02
		//
		// full-name of file01: //directory01//file01
		//
		// we call Rename("//directory01//", "//directory01Renamed//") & get:
		//
		// {root}
		// |_ directory01Renamed
		//    |_directory02
		//    |_ file01
		//    |_ file02
		//
		// full-name of file01: //directory01Renamed//file01

		bool DirectoryRenameOrMove(_IN CString& rPathOld, _IN CString& rPathNew);
		//bool DirectoryDelete(_IN CString& rPath);

	protected:
		SCFPrivate::CDictionaryNodeString* NameToNode(_IN CString& rName) _GET;

	public:
		CDFDictionary();
		virtual ~CDFDictionary();
	};
};

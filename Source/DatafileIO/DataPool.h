#pragma once

#include "Datafile.h"

namespace SCFDatafileIO
{
	class DATAFILEIO_API CDataPool : public CObject
	{
	public:
		ENUM ClassKey() _GET { return ClassDataCache; }

	public:
		CDataPool();
		~CDataPool();

	public:
		//Rebuilds the cache base on the list of associated datafiles, needs to
		//be called before first use & after adding/removing datafiles with [bReRead] set to TRUE
		//[bReRead] - if set to true, the contents of each datafile are read again from the physical medium
		bool Rebuild(_IN bool bReRead = TRUE);

	public:
		//Note that the effects of [DatafileRemove(..)] take place ONLY AFTER a
		//call to [Rebuild()]
		bool DatafileAdd   (_IN CDatafile& rDatafile);
		bool DatafileRemove(_IN CDatafile& rDatafile);

	public:
		//Resolves a relative path to an absolute path, then
		bool ParsePathFile     (_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName, _OUT CString* pOutExtension);
		bool ParsePathDirectory(_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName);

	public:
		//These synchronize the CWD amongst all contained datafiles
		//Do NOT change CWD of any datafile prior to adding it into the pool 
		CString CWD()                       _GET;
		void    CWD(_IN CString&      rCWD) _SET;
		void    CWD(_IN CDFDirectory& rCWD) _SET;

	public:
		//Creates a datafile file object for the given file path, if the path 
		//points to an actual existing file record, otherwise returns [NULL]
		CDFFile* File(_IN CString& rFilePath) _GET;

	public:
		//Same as [File(..)]
		CDFDirectory* Directory(_IN CString& rDirectoryPath) _GET;

	protected:
		//Represents the relation file/directory path -> file/directory record

		#pragma warning(disable:4251)
		CDictionaryString<SCFDatafileIOPrivate::CRecord> m_Records;

		//Represents the relation file/directory record -> datafile which contains it
		CDictionaryInt64 m_RecordDatafiles;

		//List of used datafiles (for management only, not used during lookup)
		CVector<CDatafile> m_Datafiles;
		#pragma warning(default:4251)
	};
};


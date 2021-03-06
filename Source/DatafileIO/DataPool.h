#pragma once

#include "Datafile.h"

namespace SCFDatafileIO
{
	template class __declspec(dllexport) SCFBase::CDictionaryString<SCFDatafileIOPrivate::CRecord>;
	template class __declspec(dllexport) SCFBase::CDictionaryInt64<CDatafile>;
	template class __declspec(dllexport) SCFBase::CVector<CDatafile>;

	class DATAFILEIO_API CDataPool : public CObject
	{
	public:
		CDataPool();
		~CDataPool();

	public:
		//Rebuilds the cache base on the list of associated datafiles, needs to
		//be called before first use & after adding/removing datafiles with [bReRead] set to true
		//[bReRead] - if set to true, the contents of each datafile are read again from the physical medium
		bool Rebuild(_IN bool bReRead = true);

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
		//points to an actual existing file record, otherwise returns [nullptr]
		CDFFile* File(_IN CString& rFilePath) _GET;

	public:
		//Same as [File(..)]
		CDFDirectory* Directory(_IN CString& rDirectoryPath) _GET;

	protected:
		//Represents the relation file/directory path -> file/directory record

		CDictionaryString<SCFDatafileIOPrivate::CRecord> m_Records;

		//Represents the relation file/directory record -> datafile which contains it
		CDictionaryInt64<CDatafile> m_RecordDatafiles;

		//List of used datafiles (for management only, not used during lookup)
		CVector<CDatafile> m_Datafiles;
	};
};


#pragma once
#include <SCFObjectExtensions.h>
#include "Errors.h"

using namespace SCFBase;

namespace SCFDatafileIO
{
	class DATAFILEIO_API CDatafile;

	class DATAFILEIO_API CDFDirectory : public CDirectory 
	{
	public:
		//Creates a directory object representing the current working directory
		CDFDirectory(_INOUT CDatafile& rDatafile);
		CDFDirectory(_INOUT CDatafile& rDatafile, _IN CString& rFullNameOrPath);
		CDFDirectory(_INOUT CDatafile& rDatafile, _IN CString& rPath, _IN CString& rName);
		virtual ~CDFDirectory();

	public:
		static bool ParsePath(_IN CString& rPath, _IN CDatafile& rDatafile, _OUT CString* pOutPath, _OUT CString* pOutName);

	public: 
		CDatafile& Datafile() _GET { return *m_pDatafile; }

	public:
		void CWD() _SET;

	public:
		bool Exists() _GET;
		UINT64 Size();

		bool Create(_IN bool bEraseExisting = FALSE);
		bool Delete();

		bool Erase() ;
		bool Rename(_IN CString& rNewName);
		bool Move  (_IN CString& rNewPath, _IN bool bPathHasName = FALSE);

	public:
		//Take care when using these functions the second one extract a directory 
		//from the datafile into a regular directory 
		bool Copy(_INOUT CDFDirectory& rDestination, _IN bool bOverwriteExisting = TRUE);
		bool Copy(_INOUT CDirectory&   rDestination, _IN bool bOverwriteExisting = TRUE);

	public:
		//Stubs, should not be called
		bool Writable()                   _GET;
		bool Writable(_IN bool bWritable) _SET; 

	public:
		bool Encrypted()                    _GET;
		bool Encrypted(_IN bool bCompressed) _SET;

	public:
		virtual bool Read(_OUT CVector<CString>* pOutFiles, _OUT CVector<CString>* pOutDirectories) _GET;

	protected:
		static bool Copy(_INOUT CDFDirectory& rSource, _INOUT CDFDirectory& rDestination, _IN bool bOverwriteExisting = TRUE);
		static bool Copy(_INOUT CDFDirectory& rSource, _INOUT CDirectory&   rDestination, _IN bool bOverwriteExisting = TRUE);

	protected:
		CDatafile* m_pDatafile;
	};
};
#pragma once
#include <SCFObjectExtensions.h>
#include "Errors.h"

using namespace SCFBase;

namespace SCFDatafileIO
{
	class DATAFILEIO_API CDatafile;
	class DATAFILEIO_API CDFStreamFileRead;

	class DATAFILEIO_API CDFFile : public CFile
	{
		friend CDFStreamFileRead;

	public:
		CDFFile(_IN CDFFile& rFile);
		CDFFile(_INOUT CDatafile& rDatafile, _IN CString& rFullNameOrPath);
		CDFFile(_INOUT CDatafile& rDatafile, _IN CString& rPath, _IN CString& rName, _IN CString& rExtension);
		virtual ~CDFFile();

	public:
		static bool ParsePath(_IN CString& rPath, _IN CDatafile& rDatafile, _OUT CString* pOutPath, _OUT CString* pOutName, _OUT CString* pOutExtension);

	public: 
		CDatafile& Datafile() _GET { return *m_pDatafile; }

	public:
		UINT64 Size() _GET;

	public:
		bool Exists() _GET;
		bool Create(_IN CFile& rSource, _IN bool bSourceIsFileSystem, _IN bool bEraseExisting = true);
		bool Delete();

		bool Rename(_IN CString& rNewName);
		bool Move  (_IN CString& rNewPath, _IN bool bPathHasName = false);

	public:
		//Take care when using these functions the second one extract a file from 
		//the datafile into a regular file 
		bool Copy(_INOUT CDFFile& rDestination, _IN bool bOverwriteExisting = true);
		bool Copy(_INOUT CFile&   rDestination, _IN bool bOverwriteExisting = true);

	public:
		//Stubs, should not be called
		bool Writable()                   _GET;
		bool Writable(_IN bool bWritable) _SET; 
	
	public:
		bool Compressed()                    _GET;
		bool Compressed(_IN bool bCompressed) _SET;
		
		bool Encrypted()                    _GET;
		bool Encrypted(_IN bool bEncrypted) _SET;

	protected:
		//Not used in this subclass
		bool Create(_IN bool bEraseExisting = true) { SCF_UNREFERENCED_PARAMETER(bEraseExisting); return false; }
		bool Size(UINT64 ui64Size) _SET        { SCF_UNREFERENCED_PARAMETER(ui64Size); return false; }
		bool Erase()                                { return false; }

	protected:
		CDatafile* m_pDatafile;
	};
};

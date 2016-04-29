#pragma once
#include "Vector.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CDirectory : public CObject
	{
	public:
		CString ToString() _GET { return this->PathFull(); }

	public:
		//Creates a directory object representing the current working directory
		CDirectory();
		CDirectory(_IN CString& rFullNameOrPath);
		CDirectory(_IN CString& rPath, _IN CString& rName);
		virtual ~CDirectory();

	public:
		static void ParsePath(_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName);

	public:
		virtual bool Exists() _GET;
		virtual SCF::UINT64 Size();

		virtual bool Create(_IN bool bEraseExisting = FALSE);
		virtual bool Delete();

		virtual bool Erase() ;
		virtual bool Rename(_IN CString& rNewName);
		virtual bool Move  (_IN CString& rNewPath, _IN bool bPathHasName = FALSE);

	public:
		virtual bool Copy(_INOUT CDirectory& rDestination, _IN bool bOverwriteExisting = TRUE);

	public:
		virtual bool Writable()                   _GET;
		virtual bool Writable(_IN bool bWritable) _SET;

	public:
		const CString PathFull() _GET;

		const CString Name() _GET { return m_Name; }
		const CString Path() _GET { return m_Path; }

	public:
		//Reads the contents of the directory, thus returns only file or/and directory names
		virtual bool Read(_OUT CVector<CString>* pOutFiles, _OUT CVector<CString>* pOutDirectories) _GET;

	public:
		//This generates a list of full file or/and directory paths
		bool ListContents(_OUT CVector<CString>* pOutFiles, _OUT CVector<CString>* pOutDirectories) _GET;

	protected:
		bool ListFiles    (_OUT CVector<CString>& rOutFiles) _GET;
		bool ListFilesDirs(_OUT CVector<CString>& rOutFiles, _OUT CVector<CString>& rOutDirectories) _GET;
		bool ListDirs     (_OUT CVector<CString>& rOutDirectories) _GET;

	protected:
		static bool Copy(_INOUT CDirectory& rSource, _INOUT CDirectory& rDestination, _IN bool bOverwriteExisting = TRUE);

	protected:
		CString m_Path;
		CString m_Name;
	};
};

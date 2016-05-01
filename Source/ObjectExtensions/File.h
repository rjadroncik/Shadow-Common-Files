#pragma once
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CFile : public CObject
	{
	public:
		CString ToString() _GET { return this->PathFull(); }

	public:
		CFile(_IN CFile& rFile);
		CFile(_IN CString& rFullNameOrPath);
		CFile(_IN CString& rPath, _IN CString& rName, _IN CString& rExtension);
		virtual ~CFile();

	public:
		static void ParsePath(_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName, _OUT CString* pOutExtension);

	public:
		virtual UINT64 Size()                     _GET;
		virtual bool        Size(UINT64 ui64Size) _SET;

	public:
		virtual bool Exists()     _GET;
		virtual bool Accessible() _GET; //Allows you to filter out inaccessible files such as locked system files 

	public:
		virtual bool Create(_IN bool bEraseExisting = TRUE);
		virtual bool Delete();

		virtual bool Erase() ;
		virtual bool Rename(_IN CString& rNewName);
		virtual bool Move  (_IN CString& rNewPath, _IN bool bPathHasName = FALSE);

	public:
		virtual bool Copy(_INOUT CFile& rDestination, _IN bool bOverwriteExisting = TRUE);

	public:
		virtual bool Writable()                   _GET;
		virtual bool Writable(_IN bool bWritable) _SET; 

	public:
		const CString PathFull() _GET;
		const CString NameFull() _GET;

		const CString Name()      _GET { return m_Name; }
		const CString Extension() _GET { return m_Extension; }
		const CString Path()      _GET { return m_Path; }

	public:
		const CString TempPathFull() _GET;
		const CString TempNameFull() _GET;

		const CString TempName()      _GET;
		const CString TempExtension() _GET;
		const CString TempPath()      _GET;

	protected:
		CString m_Path;	
		CString m_Name;	
		CString m_Extension;

	protected:
		CFile() {}
	};
};

#include "stdafx.h"
#include "TestFileSystem.h"

CTestFileSystem::CTestFileSystem(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of file system"), &rErrorStream)
{
	m_pDirectorySource = nullptr;
	m_pDirectoryDestination = nullptr;

	m_pFileSource = nullptr;
	m_pFileDestination = nullptr;
}
CTestFileSystem::~CTestFileSystem()
{

}

bool CTestFileSystem::Prepare() 
{
	m_pDirectorySource      = new CDirectory(STRING("Data\\ObjectExtensions\\DirSource\\"));
	m_pDirectoryDestination = new CDirectory(STRING("Data\\ObjectExtensions\\DirDest\\"));

	m_pFileSource      = new CFile(STRING("Data\\ObjectExtensions\\FileSource.txt"));
	m_pFileDestination = new CFile(STRING("Data\\ObjectExtensions\\FileDest.txt"));

	return true;
}
bool CTestFileSystem::Run()    
{ 
	CFile Suborik(STRING("\\\\spitfire\\Incomming!!!\\KS.Ian\\abcot"));

//	CFile Suborik(STRING("\\\\spitfire\\Incomming!!!\\KS - Ian\\abcot"));
//	CStreamFileRead Read(Suborik);

	if (!m_pDirectorySource->Exists())
	{
		CError::Stream()->PutLine(STRING("The directory: ") + m_pDirectorySource->PathFull() + STRING(" doesn't exist!"));
		return false;
	}
	if (!m_pFileSource->Exists())
	{
		CError::Stream()->PutLine(STRING("The file: ") + m_pFileSource->PathFull() + STRING(" doesn't exist!"));
		return false;
	}

	{
		PrintDir(*m_pDirectorySource);

		if (!m_pDirectorySource->Writable()) { m_pDirectorySource->Writable(); }

		m_pDirectorySource->Rename(STRING("DirSource.Renamed"));
		m_pDirectorySource->Copy(*m_pDirectoryDestination); 
		m_pDirectorySource->Rename(STRING("DirSource"));

		m_pDirectoryDestination->Erase();
		m_pDirectoryDestination->Delete();

		CError::Stream()->PutLine(CInt64(m_pDirectorySource->Size()).ToString());
	}

	{
		if (!m_pFileSource->Writable()) { m_pFileSource->Writable(); }

		m_pFileSource->Rename(STRING("FileSource.txt.Renamed"));
		m_pFileSource->Copy(*m_pFileDestination); 
		m_pFileSource->Rename(STRING("FileSource.txt"));
	
		m_pFileDestination->Erase();
		m_pFileDestination->Delete();

		CError::Stream()->PutLine(CInt64(m_pFileSource->Size()).ToString());
	}

	return true;
}
bool CTestFileSystem::Check()  
{
	return true; 
}
bool CTestFileSystem::CleanUp() 
{
	delete m_pDirectorySource;
	delete m_pDirectoryDestination;

	delete m_pFileSource;
	delete m_pFileDestination;

	return true;
}

void CTestFileSystem::PrintDir(_INOUT CDirectory& rDirectory, _IN UINT uiIndent)
{
	CVector<CString> Directories;
	CVector<CString> Files;

	rDirectory.Read(&Files, &Directories);

	for (UINT i = 0; i < Directories.Size(); i++)
	{
		for (UINT j = 0; j < uiIndent; j++) { CError::Stream()->PutString(STRING("  ")); }

		CError::Stream()->PutString(STRING("["));
		CError::Stream()->PutString((CString&)(Directories[i]));
		CError::Stream()->PutLine(STRING("]"));

		CDirectory Directory(rDirectory.PathFull() + (CString&)(Directories[i]) + STRING("\\"));
		PrintDir(Directory, uiIndent + 1);
	}

	for (UINT i = 0; i < Files.Size(); i++)
	{
		for (UINT j = 0; j < uiIndent; j++) { CError::Stream()->PutString(STRING("  ")); }

		CError::Stream()->PutLine((CString&)(Files[i]));
	}

	Directories.AllDelete();
	Files.AllDelete();
}
#include "DataPool.h"

using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;

CDataPool::CDataPool()
{

}

CDataPool::~CDataPool()
{

}

bool CDataPool::DatafileAdd(_IN CDatafile& rDatafile)
{
	m_Datafiles.LastAdd(rDatafile);
	return TRUE;
}

bool CDataPool::DatafileRemove(_IN CDatafile& rDatafile)
{
	m_Datafiles.Remove(rDatafile);
	return TRUE;
}

bool CDataPool::Rebuild(_IN bool bReRead)
{
	//Clear the datapool's cache, but don't delete the actual record (we need then :)
	m_Records.AllRemove();
	m_RecordDatafiles.AllRemove(); 

	for (UINT i = 0; i < m_Datafiles.Size(); i++)
	{
		//Re-read datafile contents if requested
		CDatafile* pDatafile = (CDatafile*)&m_Datafiles[i];
		if (bReRead) { pDatafile->Read(); }

		//Enumerate records & store them inside the datapool cache
		if (pDatafile->m_pRecords)
		{
			CEnumeratorDictionaryString Enumerator(*pDatafile->m_pRecords);

			while (Enumerator.Next())
			{
				//[AtPut(..)] replaces existing values so file can be "overwritten"
				m_Records.AtPut(Enumerator.CurrentPath(), (CRecord&)*Enumerator.Current());
				m_RecordDatafiles.AtPut((INT64)Enumerator.Current(), *pDatafile);
			}
		}
	}

	return TRUE;
}

CDFFile* CDataPool::File(_IN CString& rFilePath) _GET
{
	if (rFilePath[rFilePath.Length() - 1] == '\\') { return NULL; }

	CRecordFile* pRecord = (CRecordFile*)m_Records.At(rFilePath);
	if (pRecord)
	{
		CDatafile* pDatafile = (CDatafile*)m_RecordDatafiles.At((INT64)pRecord);
		if (pDatafile)
		{
			_PENDING; 
			//Need new constructor to optimize code (no need to perform 
			//record lookup in constructor, instead need to pass it as argument)
			return new CDFFile(*pDatafile, rFilePath);
		}
	}

	return NULL;
}

CDFDirectory* CDataPool::Directory(_IN CString& rDirectoryPath) _GET
{
	if (rDirectoryPath[rDirectoryPath.Length() - 1] != '\\') { return NULL; }

	CRecordDirectory* pRecord = (CRecordDirectory*)m_Records.At(rDirectoryPath);
	if (pRecord)
	{
		CDatafile* pDatafile = (CDatafile*)m_RecordDatafiles.At((INT64)pRecord);
		if (pDatafile)
		{
			_PENDING; 
			//Need new constructor to optimize code (no need to perform 
			//record lookup in constructor, instead need to pass it as argument)
			return new CDFDirectory(*pDatafile, rDirectoryPath);
		}
	}

	return NULL;
}


bool CDataPool::ParsePathFile(_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName, _OUT CString* pOutExtension)
{
	return CDFFile::ParsePath(rPath, ((CDatafile&)m_Datafiles[0]), pOutPath, pOutName, pOutExtension);
}

bool CDataPool::ParsePathDirectory(_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName)
{
	return CDFDirectory::ParsePath(rPath, ((CDatafile&)m_Datafiles[0]), pOutPath, pOutName);
}

CString CDataPool::CWD() _GET 
{ 
	return ((CDatafile&)m_Datafiles[0]).CWD(); 
}

void CDataPool::CWD(_IN CString& rCWD) _SET 
{
	for (UINT i = 0; i < m_Datafiles.Size(); i++)
	{
		((CDatafile&)m_Datafiles[i]).CWD(rCWD);
	}
}

void CDataPool::CWD(_IN CDFDirectory& rCWD) _SET 
{
	for (UINT i = 0; i < m_Datafiles.Size(); i++)
	{
		((CDatafile&)m_Datafiles[i]).CWD(rCWD);
	}
}

#include "stdafx.h"
#include "TestDatafile.h"

using namespace SCFBase;

CTestDatafile::CTestDatafile(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of datafile"), &rErrorStream)
{
	m_pDatafile = NULL;
}

CTestDatafile::~CTestDatafile()
{

}

bool CTestDatafile::Prepare() 
{
	m_pDatafile = new CDatafile(STRING("Data\\DatafileIO\\Tmp.sdf"), TRUE);

	m_Directories.LastAdd(*(new CDFDirectory(*m_pDatafile, STRING("Test\\"))));
	m_Directories.LastAdd(*(new CDFDirectory(*m_pDatafile, STRING("Test\\A1\\"))));
	m_Directories.LastAdd(*(new CDFDirectory(*m_pDatafile, STRING("Test\\A2\\"))));
	m_Directories.LastAdd(*(new CDFDirectory(*m_pDatafile, STRING("Test\\A2.xxx\\"))));

	m_Files.LastAdd(*(new CDFFile(*m_pDatafile, STRING("Test\\a1.txt"))));
	m_Files.LastAdd(*(new CDFFile(*m_pDatafile, STRING("Test\\a2.txt"))));
	m_Files.LastAdd(*(new CDFFile(*m_pDatafile, STRING("Test\\A1\\b1.txt"))));
	m_Files.LastAdd(*(new CDFFile(*m_pDatafile, STRING("Test\\A2\\b2.txt"))));
	m_Files.LastAdd(*(new CDFFile(*m_pDatafile, STRING("Test\\A2\\b3.txt"))));
	m_Files.LastAdd(*(new CDFFile(*m_pDatafile, STRING("Test\\A2.xxx\\filename"))));

	for (SCF::UINT i = 0; i < m_Directories.Size(); i++)
	{
		((CDFDirectory&)m_Directories[i]).Create();
	}

	CFile Source(STRING("Data\\DatafileIO\\treeinfo.wc"));

	for (SCF::UINT i = 0; i < m_Files.Size(); i++)
	{
		((CDFFile&)m_Files[i]).Create(Source);
	}

	return TRUE;
}
bool CTestDatafile::Run()    
{
	((CDFDirectory&)m_Directories[2]).CWD();

	CDFDirectory PathRelativeTestDirectory(*m_pDatafile, STRING("..\\A1\\"));
	CDFFile      PathRelativeTestFile     (*m_pDatafile, STRING("..\\A1\\b1.txt"));

	//File copy test from DF to FS while DF not saved..
	{
		CFile Destination(STRING("Data\\DatafileIO\\treeinfo.wc.copy"));
		((CDFFile&)m_Files[1]).Copy(Destination, TRUE);
	}

	//Directory copy test within DF
	{
		CDFDirectory Destination(*m_pDatafile, STRING("\\Test.copy\\"));
		((CDFDirectory&)m_Directories[0]).Copy(Destination);

		Destination.Rename(STRING("Test.renamed.copied"));
		Destination.Move  (STRING("\\Test\\"));
	}

	m_pDatafile->Write();
	m_pDatafile->Read();

	/////////////////////////////// AFTER SAVE & RELOAD ////////////////////////////

	((CDFFile&)m_Files[1]).Rename(STRING("a2.txt.renamed"));

	//File copy test from DF to FS after DF saved..
	{
		CFile Destination(STRING("Data\\DatafileIO\\treeinfo.wc.extracted"));
		((CDFFile&)m_Files[1]).Copy(Destination, TRUE);
	}

	//File copy test from DF to FS after DF saved.. (using a copied & renamed DF dir)
	{
		CFile Destination(STRING("Data\\DatafileIO\\treeinfo.wc.copied.extracted"));
		
		CDFFile	Source(*m_pDatafile, STRING("\\Test\\Test.renamed.copied\\a1.txt"));
		Source.Copy(Destination, TRUE);
	}

	{
		//..if it is stored, extract it & store it in the specified target file
		CMemoryBlock IOBuffer(m_pDatafile->IOBufferSize());

		CDFStreamFileRead DFStreamRead((CDFFile&)m_Files[1]);
		CStreamFileWrite FileStreamWrite(STRING("Data\\DatafileIO\\treeinfo.wc.streamed"));
		
		while (DFStreamRead.BytesLeft() > 0)
		{
			SCF::UINT uiBytesToTransfer = (DFStreamRead.BytesLeft() > m_pDatafile->IOBufferSize()) ? (m_pDatafile->IOBufferSize()) : ((SCF::UINT)DFStreamRead.BytesLeft());

			DFStreamRead.GetBytes   (IOBuffer.Value(), uiBytesToTransfer);
			FileStreamWrite.PutBytes(IOBuffer.Value(), uiBytesToTransfer);
		}
	}

	((CDFDirectory&)m_Directories[0]).Delete();

	return TRUE;
}
bool CTestDatafile::Check()  
{
	return TRUE;
}
bool CTestDatafile::CleanUp() 
{
	m_Files.AllDelete();
	m_Directories.AllDelete();

	delete m_pDatafile;

	return TRUE;
}


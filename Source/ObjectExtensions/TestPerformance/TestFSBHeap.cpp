#include "stdafx.h"
#include "TestFSBHeap.h"

#include <SCFTimer.h>
#include <malloc.h>

using namespace SCFTimer;

CTestFSBHeap::CTestFSBHeap(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of fixed-size block heaps"), &rErrorStream)
{
	m_pHeap = NULL;
}

CTestFSBHeap::~CTestFSBHeap()
{

}

bool CTestFSBHeap::Prepare() 
{
	m_pHeap = new SCFPrivate::CFSBHeap(32, 8192);
	TimerInitialize();

	return TRUE;
}
bool CTestFSBHeap::Run()    
{
	{
		SCFPrivate::CFSBHeap Heap(16, 8192);

		bool bAppend = FALSE;
		CFile File("out16.csv");
		if (File.Exists()) { bAppend = TRUE; }

		CStreamFileWrite     Write(File, bAppend);
		CStreamWriteTextUTF8 WriteText(Write);
		CStreamWriteCSV      WriteCSV(WriteText);

		ResetTime01();

		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = m_pHeap->Allocate();
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_pHeap->Free(m_vpBlocks[i]);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = m_pHeap->Allocate();
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_pHeap->Free(m_vpBlocks[i]);
		}

		WriteCSV.PutValue(CFloat(GetTime01()));

		ResetTime01();

		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = malloc(16);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			free(m_vpBlocks[i]);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = malloc(16);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			free(m_vpBlocks[i]);
		}

		WriteCSV.PutValue(CFloat(GetTime01()));
		WriteCSV.BlockNext();
	}

	{
		SCFPrivate::CFSBHeap Heap(32, 8192);

		bool bAppend = FALSE;
		CFile File("out32.csv");
		if (File.Exists()) { bAppend = TRUE; }

		CStreamFileWrite     Write(File, bAppend);
		CStreamWriteTextUTF8 WriteText(Write);
		CStreamWriteCSV      WriteCSV(WriteText);

		ResetTime01();

		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = m_pHeap->Allocate();
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_pHeap->Free(m_vpBlocks[i]);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = m_pHeap->Allocate();
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_pHeap->Free(m_vpBlocks[i]);
		}

		WriteCSV.PutValue(CFloat(GetTime01()));

		ResetTime01();

		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = malloc(32);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			free(m_vpBlocks[i]);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = malloc(32);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			free(m_vpBlocks[i]);
		}

		WriteCSV.PutValue(CFloat(GetTime01()));
		WriteCSV.BlockNext();
	}

	{
		SCFPrivate::CFSBHeap Heap(64, 8192);

		bool bAppend = FALSE;
		CFile File("out64.csv");
		if (File.Exists()) { bAppend = TRUE; }

		CStreamFileWrite     Write(File, bAppend);
		CStreamWriteTextUTF8 WriteText(Write);
		CStreamWriteCSV      WriteCSV(WriteText);

		ResetTime01();

		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = m_pHeap->Allocate();
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_pHeap->Free(m_vpBlocks[i]);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = m_pHeap->Allocate();
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_pHeap->Free(m_vpBlocks[i]);
		}

		WriteCSV.PutValue(CFloat(GetTime01()));

		ResetTime01();

		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = malloc(64);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			free(m_vpBlocks[i]);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = malloc(64);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			free(m_vpBlocks[i]);
		}

		WriteCSV.PutValue(CFloat(GetTime01()));
		WriteCSV.BlockNext();
	}

	{
		SCFPrivate::CFSBHeap Heap(128, 8192);

		bool bAppend = FALSE;
		CFile File("out128.csv");
		if (File.Exists()) { bAppend = TRUE; }

		CStreamFileWrite     Write(File, bAppend);
		CStreamWriteTextUTF8 WriteText(Write);
		CStreamWriteCSV      WriteCSV(WriteText);

		ResetTime01();

		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = m_pHeap->Allocate();
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_pHeap->Free(m_vpBlocks[i]);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = m_pHeap->Allocate();
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_pHeap->Free(m_vpBlocks[i]);
		}

		WriteCSV.PutValue(CFloat(GetTime01()));

		ResetTime01();

		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = malloc(128);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			free(m_vpBlocks[i]);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			m_vpBlocks[i] = malloc(128);
		}
		for (UINT i = 0; i < RUN_COUNT; i++)
		{
			free(m_vpBlocks[i]);
		}

		WriteCSV.PutValue(CFloat(GetTime01()));
		WriteCSV.BlockNext();
	}
	return TRUE;
}
bool CTestFSBHeap::Check()  
{
	return TRUE; 
}
bool CTestFSBHeap::CleanUp() 
{
	delete m_pHeap;

	return TRUE;
}
#include "stdafx.h"
#include "TestFSBHeap.h"

CTestFSBHeap::CTestFSBHeap(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of fixed-size block heaps"), &rErrorStream)
{
	m_pHeap = NULL;

	m_vpData01 = NULL;
	m_vpData02 = NULL;
	m_vpData03 = NULL;
	m_vpData04 = NULL;
	m_vpData05 = NULL;
	m_vpData06 = NULL;
	m_vpData07 = NULL;
}

CTestFSBHeap::~CTestFSBHeap()
{

}

bool CTestFSBHeap::Prepare() 
{
	m_pHeap = new SCFPrivate::CFSBHeap(768, 2048);

	return TRUE;
}
bool CTestFSBHeap::Run()    
{ 
	CError::Stream()->PutLine(CPointer((m_vpData01 = m_pHeap->Allocate())).ToString());
	CError::Stream()->PutLine(CPointer((m_vpData02 = m_pHeap->Allocate())).ToString());
	CError::Stream()->PutLine(CPointer((m_vpData03 = m_pHeap->Allocate())).ToString());
	CError::Stream()->PutLine(CPointer((m_vpData04 = m_pHeap->Allocate())).ToString());
	CError::Stream()->PutLine(CPointer((m_vpData05 = m_pHeap->Allocate())).ToString());

	m_pHeap->Free(m_vpData04);
	m_pHeap->Free(m_vpData02);
	m_pHeap->Free(m_vpData03);
	m_pHeap->Free(m_vpData05);

	CError::Stream()->PutLine(CPointer((m_vpData02 = m_pHeap->Allocate())).ToString());
	CError::Stream()->PutLine(CPointer((m_vpData05 = m_pHeap->Allocate())).ToString());
	CError::Stream()->PutLine(CPointer((m_vpData03 = m_pHeap->Allocate())).ToString());
	CError::Stream()->PutLine(CPointer((m_vpData04 = m_pHeap->Allocate())).ToString());

	m_pHeap->Free(m_vpData01);

	CError::Stream()->PutLine(CPointer((m_vpData06 = m_pHeap->Allocate())).ToString());
	CError::Stream()->PutLine(CPointer((m_vpData07 = m_pHeap->Allocate())).ToString());
	CError::Stream()->PutLine(CPointer((m_vpData01 = m_pHeap->Allocate())).ToString());
	
	m_pHeap->Free(m_vpData01);
	m_pHeap->Free(m_vpData02);
	m_pHeap->Free(m_vpData03);
	m_pHeap->Free(m_vpData04);
	m_pHeap->Free(m_vpData05);
	m_pHeap->Free(m_vpData06);
	m_pHeap->Free(m_vpData07);

	return TRUE;
}
bool CTestFSBHeap::Check()  
{
	if (m_pHeap->AllocatedBytes() != 0) { return FALSE; }

	return TRUE; 
}
bool CTestFSBHeap::CleanUp() 
{
	delete m_pHeap;

	return TRUE;
}
#include "stdafx.h"
#include "TestFSBHeap.h"

CTestFSBHeap::CTestFSBHeap(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of fixed-size block heaps"), &rErrorStream)
{
	m_pHeap = nullptr;

	m_vpData01 = nullptr;
	m_vpData02 = nullptr;
	m_vpData03 = nullptr;
	m_vpData04 = nullptr;
	m_vpData05 = nullptr;
	m_vpData06 = nullptr;
	m_vpData07 = nullptr;
}

CTestFSBHeap::~CTestFSBHeap()
{

}

bool CTestFSBHeap::Prepare() 
{
	m_pHeap = new SCFPrivate::CFSBHeap(768, 2048);

	return true;
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

	return true;
}
bool CTestFSBHeap::Check()  
{
	if (m_pHeap->AllocatedBytes() != 0) { return false; }

	return true; 
}
bool CTestFSBHeap::CleanUp() 
{
	delete m_pHeap;

	return true;
}
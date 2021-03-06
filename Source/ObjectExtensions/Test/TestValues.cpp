#include "stdafx.h"
#include "TestValues.h"

CTestValues::CTestValues(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of values"), &rErrorStream)
{
/*	void* vpPtr  = CMemory::Aligned16Allocate(100);
	void* vpPtr2 = CMemory::Aligned16Reallocate(vpPtr, 200);

	UINT uiSize = CMemory::Aligned16SizeOf(vpPtr2);
	CMemory::Aligned16Free(vpPtr2);*/

	m_pChar = nullptr;
	m_pInt = nullptr;
	m_pInt64 = nullptr;
	m_pBool = nullptr;
	m_pFloat = nullptr;
	m_pFloat2 = nullptr;
	m_pFloat3 = nullptr;
	m_pFloat4 = nullptr;
	m_pString = nullptr;
	m_pEnum = nullptr;
	m_pPointer = nullptr;
	m_pDateTime = nullptr;

	m_pValue01 = nullptr;
	m_pValue02 = nullptr;
	m_pValue03 = nullptr;
}

CTestValues::~CTestValues()
{

}

bool CTestValues::Prepare() 
{
	m_pChar  = new CChar('F');
	m_pInt   = new CInt(STRING("00000123456789"));
	m_pInt64 = new CInt64(STRING("123456789123456789"));;
	m_pBool  = new CBool(false);

	CFloat::FractionDigits(4, 4);

	m_pFloat  = new CFloat (STRING("-150.9 555 688 -0.55"));
	m_pFloat2 = new CFloat2(STRING("-150.9 555 688 -0.55"));
	m_pFloat3 = new CFloat3(STRING("-150.9 555 688 -0.55"));
	m_pFloat4 = new CFloat4(STRING("-150.9 555 688 -0.55"));

	m_pString      = new CString(STRING("PartA"), STRING("PartB"));
	m_pStringRange = new CStringRange(*m_pString, 4, 4);
	
	m_pEnum     = new CEnum(9);
	m_pPointer  = new CPointer(STRING("0xFfadc9a00C012"));
	m_pDateTime = new CDateTime();

	m_pValue01 = &CValue::Parse(STRING("0x55"), nullptr);
	m_pValue02 = &CValue::Parse(STRING("55"),   nullptr);
	m_pValue03 = &CValue::Parse(STRING("55.0"), nullptr);
	return true;
}
bool CTestValues::Run()    
{ 
	CString str = STRING("KOKOT") + STRING("TestApp.cfg");

	CError::Stream()->PutLine(m_pChar->ToString());
	CError::Stream()->PutLine(m_pInt->ToString());
	CError::Stream()->PutLine(m_pInt64->ToString());
	CError::Stream()->PutLine(m_pBool->ToString());
	CError::Stream()->PutLine(m_pFloat->ToString());
	CError::Stream()->PutLine(m_pFloat2->ToString());
	CError::Stream()->PutLine(m_pFloat3->ToString());
	CError::Stream()->PutLine(m_pFloat4->ToString());
	CError::Stream()->PutLine(m_pString->ToString());
	CError::Stream()->PutLine(m_pStringRange->ToString());
	CError::Stream()->PutLine(m_pString->ToString() + m_pStringRange->ToString());
	CError::Stream()->PutLine(m_pEnum->ToString());
	CError::Stream()->PutLine(m_pPointer->ToString());
	CError::Stream()->PutLine(m_pDateTime->ToString());

	CError::Stream()->PutLine(m_pValue01->ToString());
	CError::Stream()->PutLine(m_pValue02->ToString());
	CError::Stream()->PutLine(m_pValue03->ToString());

	CFormatInt FormatInt(2, 16, 1);
	CError::Stream()->PutLine(CInt(1).ToString(FormatInt));

	return true;
}
bool CTestValues::Check()  
{
	return true; 
}
bool CTestValues::CleanUp() 
{
	delete m_pChar;
	delete m_pInt;
	delete m_pInt64;
	delete m_pBool;
	delete m_pFloat;
	delete m_pFloat2;
	delete m_pFloat3;
	delete m_pFloat4;
	delete m_pStringRange;
	delete m_pString;
	delete m_pEnum;
	delete m_pPointer;
	delete m_pDateTime;

	delete m_pValue01;
	delete m_pValue02;
	delete m_pValue03;

	return true;
}
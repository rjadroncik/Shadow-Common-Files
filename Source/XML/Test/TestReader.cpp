#include "TestReader.h"

#include <SCFTimer.h>

using namespace SCFTimer;

CTestReader::CTestReader(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of XML parser performance"), &rErrorStream)
{
	m_pReader = nullptr;

	static bool bInitDone = false;

	if (!bInitDone) { TimerInitialize(); bInitDone = true; }
}
CTestReader::~CTestReader()
{

}

bool CTestReader::Prepare() 
{
	m_pReader = new CXMLReader();

	m_pRootNew = new CXMLElement();
	m_pRootOld = new CXMLElement();

	return true;
}

bool CTestReader::Run()    
{ 
	static CString Text;
	static bool bFirstRun = true;

	if (bFirstRun)
	{
		bFirstRun = false;

		CStreamFileRead ReadStream(STRING("Data\\XML\\Test.xml"));
		CStreamReadTextGeneric ReadStreamText(ReadStream);

		ReadStreamText.GetString(Text, 0);
	}

	float fTime = 0;
	CFormatFloat Format(6, 6, '.');

	//Compute a checksum in order to cache all string data in CPU memory
	//UINT64 ui64CheckSum = 0;	
	//for (UINT i = 0; i < Text.Length(); i++) { ui64CheckSum += Text[i]; }

	CError::Stream()->PutLine(STRING("Timing new parser."));
	ResetTime01();

	m_pReader->Read(Text, *m_pRootNew);

	fTime = GetTime01();
	CError::Stream()->PutString(STRING("Time taken: "));
	CError::Stream()->PutString(CFloat(fTime).ToString(Format));	
	CError::Stream()->PutLine(STRING("s\n"));	

//	CError::Stream()->PutLine(STRING("Timing old parser."));
//	ResetTime01();

//	CXMLReader::ReadOld(Text, *m_pRootOld);

//	fTime = GetTime01();
//	CError::Stream()->PutString(STRING("Time taken: "));
//	CError::Stream()->PutString(CFloat(fTime).ToString(Format));	
//	CError::Stream()->PutLine(STRING("s\n"));	

	return true;
}

bool CTestReader::Check()  
{
//	if (!(*m_pRootNew == *m_pRootOld)) { return false; }

	return true;
}
bool CTestReader::CleanUp() 
{
	delete m_pReader;

	delete m_pRootNew;
	delete m_pRootOld;

	return true;
}


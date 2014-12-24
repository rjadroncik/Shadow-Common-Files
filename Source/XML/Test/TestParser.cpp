#include "TestReader.h"

#include <SCFTimer.h>

using namespace SCF;
using namespace SCFTimer;

CTestReader::CTestReader(_INOUT IStreamWriteText& rErrorStream) : CTestCase(STRING("Test of XML parser performance"), &rErrorStream)
{
	m_pReader = NULL;

	static bool bInitDone = FALSE;

	if (!bInitDone) { TimerInitialize(); bInitDone = TRUE; }
}
CTestReader::~CTestReader()
{

}

bool CTestReader::Prepare() 
{
	m_pReader = new CXMLReader();

	m_pRootNew = NULL;
	m_pRootOld = NULL;

	return TRUE;
}

bool CTestReader::Run()    
{ 
	static CString Text;
	static bool bFirstRun = TRUE;

	if (bFirstRun)
	{
		bFirstRun = FALSE;

		CStreamFileRead ReadStream(STRING("Data\\XML\\Test.xml"));
		CStreamReadTextGeneric ReadStreamText(ReadStream);

		ReadStreamText.GetString(Text, 0);
	}

	float fTime = 0;
	CFormatFloat Format(6, 6, '.');

	//Compute a checksum in order to cache all string data in CPU memory
	//SCF::UINT64 ui64CheckSum = 0;	
	//for (SCF::UINT i = 0; i < Text.Length(); i++) { ui64CheckSum += Text[i]; }

	CError::Stream()->PutLine(STRING("Timing new parser."));
	ResetTime01();

	m_pRootNew = m_pReader->Parse(Text);

	fTime = GetTime01();
	CError::Stream()->PutString(STRING("Time taken: "));
	CError::Stream()->PutString(CFloat(fTime).AsString(Format));	
	CError::Stream()->PutLine(STRING("s\n"));	

	CError::Stream()->PutLine(STRING("Timing old parser."));
	ResetTime01();

	m_pRootOld = CXMLReader::ParseOld(Text);

	fTime = GetTime01();
	CError::Stream()->PutString(STRING("Time taken: "));
	CError::Stream()->PutString(CFloat(fTime).AsString(Format));	
	CError::Stream()->PutLine(STRING("s\n"));	

	return TRUE;
}

bool CTestReader::Check()  
{
	if (!(m_pRootNew->AsString() == m_pRootOld->AsString()))
	{
		return FALSE;
	}

	//CError::Stream()->PutLine(STRING("Result:"));	
	//if (m_pRootNew) { CError::Stream()->PutLine(m_pRootNew->AsString()); }
	//CError::Stream()->PutLine(STRING("Result:"));	
	//if (m_pRootOld) { CError::Stream()->PutLine(m_pRootOld->AsString()); }

	return TRUE;
}
bool CTestReader::CleanUp() 
{
	delete m_pReader;

	if (m_pRootNew) { delete m_pRootNew; }
	if (m_pRootOld) { delete m_pRootOld; }


	return TRUE;
}


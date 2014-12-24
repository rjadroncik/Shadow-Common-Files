#include "Assembler.h"

using namespace SCFScripting;

INSTRUCTION CAssembler::s_Instructions[InstructionsCount];

CDictionaryString CAssembler::s_InstructionCodes;

CString* CAssembler::s_Words[3] = { NULL, NULL, NULL };

CDictionaryString CAssembler::s_Labels;
CVector CAssembler::s_LabelsDeferred;

CStreamReadText* CAssembler::s_pStreamIn  = NULL;
IStreamWrite*    CAssembler::s_pStreamOut = NULL;

bool CAssembler::Initialize()
{
	s_Instructions[InstructionPush8]  = Push8;
	s_Instructions[InstructionPush16] = Push16;
	s_Instructions[InstructionPush32] = Push32;
	s_Instructions[InstructionPush64] = Push64;

	s_Instructions[InstructionPushACC]  = InstructionStub;
	s_Instructions[InstructionPushDATA] = InstructionStub;
	s_Instructions[InstructionPushCNT]  = InstructionStub;
	s_Instructions[InstructionPushBP]   = InstructionStub;

	s_Instructions[InstructionPop] = Pop;

	s_Instructions[InstructionPopACC]  = InstructionStub;
	s_Instructions[InstructionPopDATA] = InstructionStub;
	s_Instructions[InstructionPopCNT]  = InstructionStub;
	s_Instructions[InstructionPopBP]   = InstructionStub;

	s_Instructions[InstructionSetACC]  = InstructionInt;
	s_Instructions[InstructionSetDATA] = InstructionInt;
	s_Instructions[InstructionSetCNT]  = InstructionInt;

	s_Instructions[InstructionLoadACCSP]  = InstructionInt;
	s_Instructions[InstructionLoadACCBP]  = InstructionInt;
	s_Instructions[InstructionLoadDATASP] = InstructionInt;
	s_Instructions[InstructionLoadDATABP] = InstructionInt;
	s_Instructions[InstructionLoadCNTSP]  = InstructionInt;
	s_Instructions[InstructionLoadCNTBP]  = InstructionInt;

	s_Instructions[InstructionCall] = Jmp;
	s_Instructions[InstructionRet]  = InstructionStub;

	s_Instructions[InstructionJmpZACC]  = Jmp;
	s_Instructions[InstructionJmpZDATA] = Jmp;
	s_Instructions[InstructionJmpZCNT]  = Jmp;

	s_Instructions[InstructionJmpNZACC]  = Jmp;
	s_Instructions[InstructionJmpNZDATA] = Jmp;
	s_Instructions[InstructionJmpNZCNT]  = Jmp;

	s_Instructions[InstructionIntAdd] = InstructionStub;
	s_Instructions[InstructionIntSub] = InstructionStub;
	s_Instructions[InstructionIntMul] = InstructionStub;
	s_Instructions[InstructionIntDiv] = InstructionStub;
	s_Instructions[InstructionIntRmd] = InstructionStub;

	s_Instructions[InstructionFloatAdd]  = InstructionStub;
	s_Instructions[InstructionFloatSub]  = InstructionStub;
	s_Instructions[InstructionFloatMul]  = InstructionStub;
	s_Instructions[InstructionFloatDiv]  = InstructionStub;
	s_Instructions[InstructionFloatSqrt] = InstructionStub;
	s_Instructions[InstructionFloatLog]  = InstructionStub;

	s_Instructions[InstructionFloatSqrt] = InstructionStub;
	s_Instructions[InstructionFloatLog]  = InstructionStub;
	
	s_Instructions[InstructionIncCNT] = InstructionStub;
	s_Instructions[InstructionDecCNT] = InstructionStub;

	s_InstructionCodes.AtPut(STRING("Push8"),   *(new CInt(InstructionPush8)));
	s_InstructionCodes.AtPut(STRING("Push16"),  *(new CInt(InstructionPush16)));
	s_InstructionCodes.AtPut(STRING("Push32"),  *(new CInt(InstructionPush32)));
	s_InstructionCodes.AtPut(STRING("Push64"),  *(new CInt(InstructionPush64)));

	s_InstructionCodes.AtPut(STRING("PushACC"),   *(new CInt(InstructionPushACC)));
	s_InstructionCodes.AtPut(STRING("PushDATA"),  *(new CInt(InstructionPushDATA)));
	s_InstructionCodes.AtPut(STRING("PushCNT"),   *(new CInt(InstructionPushCNT)));
	s_InstructionCodes.AtPut(STRING("PushBP"),    *(new CInt(InstructionPushBP)));

	s_InstructionCodes.AtPut(STRING("Pop"),      *(new CInt(InstructionPop)));
	s_InstructionCodes.AtPut(STRING("PopACC"),   *(new CInt(InstructionPopACC)));
	s_InstructionCodes.AtPut(STRING("PopDATA"),  *(new CInt(InstructionPopDATA)));
	s_InstructionCodes.AtPut(STRING("PopCNT"),   *(new CInt(InstructionPopCNT)));
	s_InstructionCodes.AtPut(STRING("PopBP"),    *(new CInt(InstructionPopBP)));

	s_InstructionCodes.AtPut(STRING("SetACC"),   *(new CInt(InstructionSetACC)));
	s_InstructionCodes.AtPut(STRING("SetDATA"),  *(new CInt(InstructionSetDATA)));
	s_InstructionCodes.AtPut(STRING("SetCNT"),   *(new CInt(InstructionSetCNT)));

	s_InstructionCodes.AtPut(STRING("LoadACCSP"),   *(new CInt(InstructionLoadACCSP)));
	s_InstructionCodes.AtPut(STRING("LoadACCBP"),   *(new CInt(InstructionLoadACCBP)));
	s_InstructionCodes.AtPut(STRING("LoadDATASP"),  *(new CInt(InstructionLoadDATASP)));
	s_InstructionCodes.AtPut(STRING("LoadDATABP"),  *(new CInt(InstructionLoadDATABP)));
	s_InstructionCodes.AtPut(STRING("LoadCNTSP"),   *(new CInt(InstructionLoadCNTSP)));
	s_InstructionCodes.AtPut(STRING("LoadCNTBP"),   *(new CInt(InstructionLoadCNTBP)));

	s_InstructionCodes.AtPut(STRING("Call"),  *(new CInt(InstructionCall)));
	s_InstructionCodes.AtPut(STRING("Ret"),   *(new CInt(InstructionRet)));

	s_InstructionCodes.AtPut(STRING("JmpZACC"),   *(new CInt(InstructionJmpZACC)));
	s_InstructionCodes.AtPut(STRING("JmpZDATA"),  *(new CInt(InstructionJmpZDATA)));
	s_InstructionCodes.AtPut(STRING("JmpZCNT"),   *(new CInt(InstructionJmpZCNT)));

	s_InstructionCodes.AtPut(STRING("JmpNZACC"),   *(new CInt(InstructionJmpNZACC)));
	s_InstructionCodes.AtPut(STRING("JmpNZDATA"),  *(new CInt(InstructionJmpNZDATA)));
	s_InstructionCodes.AtPut(STRING("JmpNZCNT"),   *(new CInt(InstructionJmpNZCNT)));

	s_InstructionCodes.AtPut(STRING("IntAdd"),  *(new CInt(InstructionIntAdd)));
	s_InstructionCodes.AtPut(STRING("IntSub"),  *(new CInt(InstructionIntSub)));
	s_InstructionCodes.AtPut(STRING("IntMul"),  *(new CInt(InstructionIntMul)));
	s_InstructionCodes.AtPut(STRING("IntDiv"),  *(new CInt(InstructionIntDiv)));
	s_InstructionCodes.AtPut(STRING("IntRmd"),  *(new CInt(InstructionIntRmd)));

	s_InstructionCodes.AtPut(STRING("FloatAdd"),  *(new CInt(InstructionFloatAdd)));
	s_InstructionCodes.AtPut(STRING("FloatSub"),  *(new CInt(InstructionFloatSub)));
	s_InstructionCodes.AtPut(STRING("FloatMul"),  *(new CInt(InstructionFloatMul)));
	s_InstructionCodes.AtPut(STRING("FloatDiv"),  *(new CInt(InstructionFloatDiv)));
	s_InstructionCodes.AtPut(STRING("FloatSqrt"), *(new CInt(InstructionFloatSqrt)));
	s_InstructionCodes.AtPut(STRING("FloatLog"),  *(new CInt(InstructionFloatLog)));
	
	s_InstructionCodes.AtPut(STRING("IncCNT"), *(new CInt(InstructionIncCNT)));
	s_InstructionCodes.AtPut(STRING("DecCNT"), *(new CInt(InstructionDecCNT)));

	s_Words[0] = new CString();
	s_Words[1] = new CString();
	s_Words[2] = new CString();

	return TRUE;
}

UINT CAssembler::LineParse(_IN CString& rLine)
{
	//Skip empty lines & comments
	if (!rLine.Length() || rLine[0] == '#') { return 0; }

	//Prepare the word index & the first word
	UINT uiWord = 0;
	s_Words[uiWord]->Resize(0);

	//Go through all the characters of the line
	for (UINT i = 0; i < rLine.Length(); i++)
	{
		//Check for a subset of whitespace characters
		if ((rLine[i] == '\r') ||
			(rLine[i] == ' ' ) ||
			(rLine[i] == '\t') ||
			(rLine[i] == ','))
		{
			//Don't parse more than 3 words (for now)
			if (uiWord == 2) { return 3; }

			//If we found the first whitespace after a word
			if (s_Words[uiWord]->Length())
			{
				//Start a new word
				uiWord++;
				s_Words[uiWord]->Resize(0);
			}
			//Ignore multiple whitespaces (therefore nothing here)
		}
		else
		{
			//Append current word
			*s_Words[uiWord] += rLine[i];
		}
	}

	//Return the number of parsed words
	return uiWord;
}

bool CAssembler::Assemble(_INOUT CStreamReadText& rAssemblyCodeStream, _OUT CExecutable& rExecutable)
{
	CStreamMemoryWrite StreamOut(rExecutable.m_Code);

	s_pStreamIn  = &rAssemblyCodeStream;
	s_pStreamOut = &StreamOut;

	CString Line;

	while (s_pStreamIn->GetLine(Line))
	{
		if (LineParse(Line))
		{
			CInt* pCode = (CInt*)s_InstructionCodes.NameToObject(*s_Words[0]);
			if (pCode)
			{
				//Write instruction code first, as this must always be done anyway
				s_pStreamOut->PutByte((BYTE)pCode->Value());

				//Perform instruction specific processing by invoking functions indexed
				//by instruction codes
				s_Instructions[(BYTE)pCode->Value()]();
			}
			else
			{
				s_Labels.AtPut(CStringRange(*s_Words[0], 1, s_Words[0]->Length() - 2), *(new CInt((UINT)s_pStreamOut->BytesWritten())));
			}
		}
	}

	s_pStreamOut->PutByte(InstructionsCount);

	ResolveDeferredLabels(rExecutable);
	CleanUp();

	return TRUE;
}

void CAssembler::CleanUp()
{
	s_Labels.AllDelete();

	while (s_LabelsDeferred.Size())
	{
		CAssociation* pAssociation = (CAssociation*)&s_LabelsDeferred.Last();

		s_LabelsDeferred.LastRemove();

		pAssociation->DeleteWithObjects();
	}

	s_LabelsDeferred.AllDelete();
}

bool CAssembler::ResolveDeferredLabels(_INOUT CExecutable& rExecutable)
{
	for (UINT i =0; i < s_LabelsDeferred.Size(); i++)
	{
		CAssociation* pLabelDeferred = (CAssociation*)&(s_LabelsDeferred[i]);

		CInt* pOffset = (CInt*)s_Labels.NameToObject((CString&)pLabelDeferred->Key()); 
		if (pOffset) 
		{ 
			*(__int32*)(rExecutable.Code() + ((CInt&)pLabelDeferred->Value()).Value()) = pOffset->Value();
		} 
		else 
		{
			return FALSE;
		} 
	}

	return TRUE;
}

void CAssembler::Push8()  { s_pStreamOut->PutByte ((BYTE)  CInt::Parse  (*s_Words[1])); }
void CAssembler::Push16() { s_pStreamOut->PutWord ((WORD)  CInt::Parse  (*s_Words[1])); }
void CAssembler::Push32() { s_pStreamOut->PutInt  ((UINT)  CInt::Parse  (*s_Words[1])); }
void CAssembler::Push64() { s_pStreamOut->PutInt64((UINT64)CInt64::Parse(*s_Words[1])); }

void CAssembler::Pop() { s_pStreamOut->PutByte((BYTE)CInt::Parse(*s_Words[1])); }

void CAssembler::InstructionInt() { s_pStreamOut->PutInt(CInt::Parse(*s_Words[1])); }

void CAssembler::Jmp() 
{ 
	CInt* pOffset = (CInt*)s_Labels.NameToObject(*s_Words[1]); 
	if (pOffset) 
	{ 
		s_pStreamOut->PutInt(pOffset->Value());
	} 
	else 
	{
		if ((*s_Words[1])[0] == '0')
		{
			s_pStreamOut->PutInt(CInt::Parse(*s_Words[1])); 
		}
		else
		{
			//Deferred label resolution
			s_LabelsDeferred.LastAdd(*(new CAssociation(*(new CString(*s_Words[1])), *(new CInt((UINT)s_pStreamOut->BytesWritten())))));

			//Placeholder value
			s_pStreamOut->PutInt(0); 
		}
	} 
}
#pragma once

#include "Executable.h"

namespace SCFScripting
{
#define INSTRUCTION_CODE_SIZE sizeof(BYTE)

	enum Instructions
	{
		InstructionPush8,
		InstructionPush16,
		InstructionPush32,
		InstructionPush64,

		InstructionPushACC,
		InstructionPushDATA,
		InstructionPushCNT,
		InstructionPushBP,

		InstructionPop,

		InstructionPopACC,
		InstructionPopDATA,
		InstructionPopCNT,
		InstructionPopBP,

		InstructionSetACC,
		InstructionSetDATA,
		InstructionSetCNT,

		InstructionLoadACCSP,
		InstructionLoadACCBP,
		InstructionLoadDATASP,
		InstructionLoadDATABP,
		InstructionLoadCNTSP,
		InstructionLoadCNTBP,

		InstructionCall,
		InstructionRet,

		InstructionJmpZACC,
		InstructionJmpZDATA,
		InstructionJmpZCNT,

		InstructionJmpNZACC,
		InstructionJmpNZDATA,
		InstructionJmpNZCNT,

		InstructionIntAdd,
		InstructionIntSub,
		InstructionIntMul,
		InstructionIntDiv,
		InstructionIntRmd,

		InstructionFloatAdd,
		InstructionFloatSub,
		InstructionFloatMul,
		InstructionFloatDiv,

		InstructionFloatSqrt,
		InstructionFloatLog,

		InstructionIncCNT,
		InstructionDecCNT,

		InstructionsCount,
	};

	typedef void (__cdecl *INSTRUCTION)();

	class SCRIPTING_API CAssembler
	{
	public:
		static bool Initialize();

	public:
		static bool Assemble(_INOUT CStreamReadText& rAssemblyCodeStream, _OUT CExecutable& rExecutable);

	private:
		//Parses line & stores words into the [s_Words] array
		//Returns the number of parsed words (capped at max 3 for now)
		static SCF::UINT LineParse(_IN CString& rLine);

		static void CleanUp();
	
		//Injects values of label offsets into generated VM code
		static bool ResolveDeferredLabels(_INOUT CExecutable& rExecutable);

	private:
		static void InstructionStub() {}
		static void InstructionInt();

		//Stack instructions
		static void Push8();
		static void Push16();
		static void Push32();
		static void Push64();

		//static void PushACC();  //InstructionStub() used
		//static void PushDATA(); //InstructionStub() used
		//static void PushCNT();  //InstructionStub() used
		//static void PushBP();  //InstructionStub() used

		static void Pop();

		//static void PopACC();  //InstructionStub() used
		//static void PopDATA(); //InstructionStub() used
		//static void PopCNT();  //InstructionStub() used

		//static void LoadACCSP();  //InstructionInt() used
		//static void LoadACCBP();  //InstructionInt() used
		//static void LoadDATASP(); //InstructionInt() used
		//static void LoadDATABP(); //InstructionInt() used
		//static void LoadCNTSP();  //InstructionInt() used
		//static void LoadCNTBP();  //InstructionInt() used

		//Register instructions
		//static void SetACC();  //InstructionInt() used
		//static void SetDATA(); //InstructionInt() used
		//static void SetCNT();  //InstructionInt() used

		//Jump instructions
		//static void Call(); //Jmp() used
		//static void Ret(); //InstructionStub() used

		/*
		static void JmpZACC();  //Jmp() used
		static void JmpZDATA(); //Jmp() used
		static void JmpZCNT();  //Jmp() used

		static void JmpNZACC();  //Jmp() used
		static void JmpNZDATA(); //Jmp() used 
		static void JmpNZCNT();  //Jmp() used
		*/
		static void Jmp();

		//Arithmetic instruction
		/*
		static void IntAdd(); //InstructionStub() used
		static void IntSub(); //InstructionStub() used
		static void IntMul(); //InstructionStub() used
		static void IntDiv(); //InstructionStub() used
		static void IntRmd(); //InstructionStub() used

		static void FloatAdd(); //InstructionStub() used
		static void FloatSub(); //InstructionStub() used
		static void FloatMul(); //InstructionStub() used
		static void FloatDiv(); //InstructionStub() used

		static void FloatSqrt(); //InstructionStub() used
		static void FloatLog();  //InstructionStub() used

		static void IncCNT(); //InstructionStub() used
		static void DecCNT(); //InstructionStub() used
		*/

	private:
		//A static array of function pointers representing the code needed to be executed 
		//for each instruction, the respective function is invoked based on the instruction code
		static INSTRUCTION s_Instructions[InstructionsCount];

	private:
		//Maps instruction names to their respective instruction codes
		static CDictionaryString s_InstructionCodes;

	private:
		//Temporary storage for words on the current line during parsing
		static CString* s_Words[3];

	private:
		//Maps label names to their values(global instruction addresses)
		static CDictionaryString s_Labels;
		//Contains a list of associations of type [LabelName] -> [PoisitionOfUsage]
		static CVector s_LabelsDeferred;

	private:
		//Streams for reading from the assembly source & writing to code segment of the executable
		static CStreamReadText* s_pStreamIn;
		static IStreamWrite*    s_pStreamOut;
	};
};
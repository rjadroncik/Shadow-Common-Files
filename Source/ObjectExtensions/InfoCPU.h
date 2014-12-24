#pragma once 

#include "String.h"
#include "Vector.h"

namespace SCFBase
{
	enum CPUVendors
	{
		CPUVendorUnknown = 0,
		CPUVendorIntel = 1,
		CPUVendorAMD,
		CPUVendorIBM,
		CPUVendorTransmeta,
	};

	enum CPUInstructionSets
	{
		CPUInstructionSet386 = 0x0001,
		CPUInstructionSet486 = 0x0002,
		CPUInstructionSet586 = 0x0004,
		CPUInstructionSet686 = 0x0008,

		CPUInstructionSetMMX  = 0x0100,
		CPUInstructionSetSSE  = 0x0200,
		CPUInstructionSetSSE2 = 0x0300,
		CPUInstructionSetSSE3 = 0x0400,
		CPUInstructionSetSSE4 = 0x1000,
	};

	class OBJECT_EXTENSIONS_API CInfoCPU
	{
	public:
		//Returns the CPU name string
		static CString Name();

		//Returns a value from [CPUVendors]
		static SCF::ENUM Vendor();
		static CString   VendorString();

		//Returns OR-ed values from [CPUInstructionSets]
		static SCF::DWORD InstructionSets();
		static CString    InstructionSetsString();

		//Returns frequency in MHz
		static SCF::DWORD Frequency();
		static SCF::DWORD Cores();

	public:
		//Sumarizes CPU information in a short report
		static CString Report();

	private:
		CInfoCPU();
		~CInfoCPU();
	};
}
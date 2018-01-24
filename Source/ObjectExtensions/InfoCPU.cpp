#include "InfoCPU.h"

#include "StringSearch.h"
#include "Int.h"

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#else

#endif // WIN32


using namespace SCFBase;

ENUM CInfoCPU::Vendor()
{
	CString VendorString(CInfoCPU::VendorString());
	CStringSearch VendorSearch(VendorString);

	if (VendorSearch.FindStringIC(STRING("intel")))     { return CPUVendorIntel; }
	if (VendorSearch.FindStringIC(STRING("amd")))       { return CPUVendorAMD; }
	if (VendorSearch.FindStringIC(STRING("ibm")))       { return CPUVendorIBM; }
	if (VendorSearch.FindStringIC(STRING("transmeta"))) { return CPUVendorTransmeta; }

	return CPUVendorUnknown;
}

CString CInfoCPU::VendorString()
{
	int Part1, Part2, Part3;

    #ifdef WIN32

	__asm
	{
		mov eax, 0
		cpuid
		mov Part1, ebx
		mov Part3, ecx
		mov Part2, edx
	}

    #else
    //TODO:
    #endif // WIN32

	CString Vendor;
	Vendor.Resize(12);

	for (int i = 0; i < 4; i++)
	{
		Vendor.AtPut(0 + i, (char)Part1);
		Part1 >>= 8;
	}
	for (int i = 0; i < 4; i++)
	{
		Vendor.AtPut(4 + i, (char)Part2);
		Part2 >>= 8;
	}
	for (int i = 0; i < 4; i++)
	{
		Vendor.AtPut(8 + i, (char)Part3);
		Part3 >>= 8;
	}

	Vendor.LengthScan();

	return Vendor;
}

DWORD CInfoCPU::InstructionSets()
{
	int BaseInfo;
	int FeaturesA;
	int FeaturesB;

    #ifdef WIN32

	__asm
	{
		mov eax, 1
		cpuid
		mov BaseInfo, eax
		mov FeaturesB, ecx
		mov FeaturesA, edx
	}

    #else
    //TODO:
    #endif // WIN32

	DWORD dwResult = CPUInstructionSet386 | CPUInstructionSet486 | CPUInstructionSet586 | CPUInstructionSet686;

	if (FeaturesA & 0x00800000) { dwResult |= CPUInstructionSetMMX; }
	if (FeaturesA & 0x02000000) { dwResult |= CPUInstructionSetSSE; }
	if (FeaturesA & 0x04000000) { dwResult |= CPUInstructionSetSSE2; }

	if (FeaturesB & 0x00000001) { dwResult |= CPUInstructionSetSSE3; }

	return dwResult;
}

CString CInfoCPU::InstructionSetsString()
{
	CString Result;

	DWORD dwSets = InstructionSets();

	if (dwSets & CPUInstructionSetMMX)  { Result += STRING("MMX"); }
	if (dwSets & CPUInstructionSetSSE)  { if (Result.Length()) { Result += STRING(", "); } Result += STRING("SSE"); }
	if (dwSets & CPUInstructionSetSSE2) { if (Result.Length()) { Result += STRING(", "); } Result += STRING("SSE2"); }
	if (dwSets & CPUInstructionSetSSE3) { if (Result.Length()) { Result += STRING(", "); } Result += STRING("SSE3"); }

	return Result;
}

CString CInfoCPU::Name()
{
    #ifdef WIN32

	HKEY hKey = nullptr;

	UINT uiRetVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 0, KEY_READ, &hKey);
	if (uiRetVal == ERROR_SUCCESS)
	{
		DWORD dwSize = 0;

		//Get length of string
		uiRetVal = RegQueryValueEx(hKey, TEXT("ProcessorNameString"), nullptr, nullptr, nullptr, &dwSize);
		if (uiRetVal == ERROR_SUCCESS)
		{
			CString Result;
			Result.Resize(dwSize / 2);

			uiRetVal = RegQueryValueEx(hKey, TEXT("ProcessorNameString"), nullptr, nullptr, (LPBYTE)Result.Value(), &dwSize);
			if (uiRetVal == ERROR_SUCCESS)
			{
				RegCloseKey(hKey);

				Result.LengthScan();
				return Result;
			}

			RegCloseKey(hKey);
			return CString();
		}

		RegCloseKey(hKey);
	}

    #else
    //TODO:
    #endif // WIN32

	return CString();
}

DWORD CInfoCPU::Frequency()
{
    #ifdef WIN32

	HKEY hKey = nullptr;

	UINT uiRetVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 0, KEY_READ, &hKey);
	if (uiRetVal == ERROR_SUCCESS)
	{
		DWORD dwSize = 4;
		DWORD dwFrequency = 0;

		uiRetVal = RegQueryValueEx(hKey, TEXT("~MHz"), nullptr, nullptr, (LPBYTE)&dwFrequency, &dwSize);
		if (uiRetVal == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return dwFrequency;
		}

		RegCloseKey(hKey);
	}

    #else
    //TODO:
    #endif // WIN32

	return 0;
}

DWORD CInfoCPU::Cores()
{
	DWORD dwCoreCount = 1;

    #ifdef WIN32

	HKEY hKey = nullptr;

	for (int i = 0; i < 8; i++)
	{
		UINT uiRetVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (STRING("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\") + CInt(i + 1).ToString()).Value(), 0, KEY_READ, &hKey);
		if (uiRetVal == ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			dwCoreCount++;
		}
	}

    #else
    //TODO:
    #endif // WIN32

	return dwCoreCount;
}

CString CInfoCPU::Report()
{
	CString Result;

	Result += STRING("Name: ");             Result += Name();                       Result += '\n';
	Result += STRING("Vendor: ");           Result += VendorString();               Result += '\n';
	Result += STRING("Frequency: ");        Result += CInt(Frequency()).ToString(); Result += STRING(" MHz\n");
	Result += STRING("Core count: ");       Result += CInt(Cores()).ToString();     Result += '\n';
	Result += STRING("Instruction sets: "); Result += InstructionSetsString();      Result += '\n';

	return Result;
}


// 	int MaxFunctions;
// 	int Part1, Part2, Part3;
//
// 	__asm
// 	{
// 		mov eax, 0
// 		cpuid
// 		mov MaxFunctions, eax
// 		mov Part1, ebx
// 		mov Part3, ecx
// 		mov Part2, edx
// 	}

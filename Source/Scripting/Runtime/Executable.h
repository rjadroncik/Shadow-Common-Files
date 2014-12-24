#pragma once

#include <SCFObjectExtensions.h>

#include "../Classes.h"

using namespace SCFObjectExtensions;

namespace SCFScripting
{
	class SCRIPTING_API CAssembler;

	class SCRIPTING_API CExecutable : public CObject
	{
		friend 	class SCRIPTING_API CAssembler;

	public:
		//Dynamic casting
		void* AsType(_IN SCF::ENUM eType) const; 
		SCF::ENUM ClassKey() _GET { return ClassExecutable; }

	public:
		CExecutable(_IN SCF::UINT uiCodeSize);
		virtual ~CExecutable();

	public:
		void DumpCode(_IN CFile& rFile);

	public:
		const SCF::BYTE* Code()     _GET { return (const SCF::BYTE*)m_Code.Value(); }
		SCF::UINT        CodeSize() _GET { return m_Code.Size(); }
	
		const SCF::BYTE* Main()     _GET { return m_pMain; }

	private:
		//Contains data of the code segment
		CMemoryBlock m_Code;

	private:
		//Points to the first instruction of the main() function
		const SCF::BYTE* m_pMain;
	};
};
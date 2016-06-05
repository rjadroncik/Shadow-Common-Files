#pragma once

#include "Scanner.h"
#include "Parser.h"
#include "..\Description\Enum.h"

namespace SCFCompiler
{
	class SCRIPTING_API CCompiler : public CObject
	{
	public:
		ENUM ClassKey() _GET { return ClassCompiler; }
		CString   ToString() _GET { return STRING("Compiler"); }

	public:
		CCompiler();
		virtual ~CCompiler();

	public:
		bool Compile(_IN CString& rText);

	public:
		//Debugging feature - reconstructs the text from input word stream
		void TextReconstruct(_OUT CString& rOutText);

	public:
        CDictionaryString<SCFScripting::CPackage>&   Packages()   _GET { return (CDictionaryString<SCFScripting::CPackage>&)m_Packages; }
		CDictionaryString<SCFScripting::CInterface>& Interfaces() _GET { return (CDictionaryString<SCFScripting::CInterface>&)m_Interfaces; }
		CDictionaryString<SCFScripting::CClass>&     Classes()    _GET { return (CDictionaryString<SCFScripting::CClass>&)m_Classes; }
		CDictionaryString<SCFScripting::CMethod>&    Methods()    _GET { return (CDictionaryString<SCFScripting::CMethod>&)m_Methods; }
		CDictionaryString<SCFScripting::CEnum>&      Enums()      _GET { return (CDictionaryString<SCFScripting::CEnum>&)m_Enums; }

		CList<CCompilationUnit>& CompilationUnits() _GET { return (CList<CCompilationUnit>&)m_CompilationUnits; }
	protected:

#pragma warning(disable:4251)
	    //A dictionaries of all the objects known to the compiler
		CDictionaryString<SCFScripting::CPackage>   m_Packages;
		CDictionaryString<SCFScripting::CInterface> m_Interfaces;
		CDictionaryString<SCFScripting::CClass>     m_Classes;
		CDictionaryString<SCFScripting::CMethod>    m_Methods;
		CDictionaryString<SCFScripting::CEnum>      m_Enums;

		CList<CCompilationUnit> m_CompilationUnits;
#pragma warning(default:4251)

	protected:
		CScanner m_Scanner;
		CParser  m_Parser;
	};
};

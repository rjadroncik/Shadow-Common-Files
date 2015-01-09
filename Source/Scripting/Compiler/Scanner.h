#pragma once

#include "Scanner/Token.h"

namespace SCFCompiler
{
	class CScanner;

	typedef bool (__thiscall CScanner::*SCAN_FUNC) ();

	class SCRIPTING_API CScanner : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassScanner; }
		CString   ToString() _GET { return STRING("Scanner"); }

	public:
		CScanner();
		virtual ~CScanner();

	public:
		bool Scan(_IN CString& rText);

	public:
		CString              Text()   _GET { return m_Text; }
		const CList<CToken>& Tokens() _GET { return m_Tokens; }

	public:
		//Debugging feature - reconstructs the text after parsing it
		void TextReconstruct(_OUT CString& rOutText);
		void PrintWordTypes (_OUT CString& rOutText);

	protected:
		bool ScanWordStart();
	
		bool ScanTokenWordContinue();
	
		bool ScanTokenNumberContinue();
		bool ScanTokenNumberContinue2nd();
	
		bool ScanTokenOperatorContinue();
	
		bool ScanTokenStringContinue();
		bool ScanTokenStringContinueChar();
	
		bool ScanWhitespaceContinue();
	
		bool ScanCommentLine();
		bool ScanCommentBlock();

	protected:
		//Input text
		CString m_Text;
		//Generated list/stream of words
#pragma warning(disable:4251)
		CList<CToken> m_Tokens; 
#pragma warning(default:4251)

	private:
		//Index of the current character
		SCF::UINT m_uiChar;
		//Pointer to the next function to evaluate (this is a form of state representation of a typical state machine)
		SCAN_FUNC m_fpNext;

	private:
		SCF::UINT m_uiStart;
	};
};

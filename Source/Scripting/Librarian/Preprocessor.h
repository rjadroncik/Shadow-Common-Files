#pragma once

#include "Classes.h"

#include <SCFObjectExtensions.h>

using namespace SCFBase;

namespace SCFLibrarian
{
	class CPreprocessor : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassPreprocessor; }
		CString   AsString() _GET { return STRING("Preprocessor"); }

	public:
		CPreprocessor();
		virtual ~CPreprocessor();

	public:
		bool Preprocess(_IN CString& rText);

	public:
		CString Text() _GET { return m_Text; }

	protected:
		//Input aswell as result text
		CString m_Text;

	protected:
		CDictionaryString<CString> m_Defines;
	};
};

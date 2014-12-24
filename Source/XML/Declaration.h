#pragma once

#include "Node.h"
#include "Attribute.h"

namespace SCFXML
{
	class XML_API CXMLDeclaration : public SCFBase::CObject
	{
		friend class XML_API CXMLStreamRead;

	public:
		SCF::ENUM ClassKey() _GET { return ClassXMLDeclaration; }

	public:
		CXMLDeclaration();
		virtual ~CXMLDeclaration();	

	public:
		CString Version()    _GET { return STRINGREF(m_Version); }
		CString Encoding()   _GET { return STRINGREF(m_Encoding); }
		CString Standalone() _GET { return STRINGREF(m_Standalone); }

	public:
		void Version   (_IN CString& rVersion)    _SET { m_Version    = rVersion; }
		void Encoding  (_IN CString& rEncoding)   _SET { m_Encoding   = rEncoding; }
		void Standalone(_IN CString& rStandalone) _SET { m_Standalone = rStandalone; }

	protected:
		CString m_Version;
		CString m_Encoding;
		CString m_Standalone;
	};
};
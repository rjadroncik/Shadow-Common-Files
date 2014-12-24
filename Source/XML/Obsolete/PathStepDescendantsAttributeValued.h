#pragma once

#include "PathStep.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepDescendantsAttributeValued : public CXMLPathStep
	{
	public:
		CXMLPathStepDescendantsAttributeValued();
		virtual ~CXMLPathStepDescendantsAttributeValued();

	public:
		inline void          QName(_IN CString& rQName) _SET { m_QName = rQName; }
		inline const CString QName()                    _GET { return STRINGREF(m_QName); }

	public:
		inline void          Value(_IN CString& rValue) _SET { m_Value = rValue; }
		inline const CString Value()                    _GET { return STRINGREF(m_Value); }

	public:
		bool Match(_IN SCFXML::CXMLNode& rCurrent, _OUT CList& rOutList) const;

	private:
		CString m_QName;
		CString m_Value;
	};
};

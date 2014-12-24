#pragma once

#include "BoundValue.h"

namespace SCFXMLPrivate
{
	class CXMLBoundValueNode: public CXMLBoundValue
	{
	public:
		CXMLBoundValueNode();
		virtual ~CXMLBoundValueNode();

	public:
		void Name(_IN CString& rName) _SET { m_Name = rName; }

	public:
		const CValue* Evaluate(_IN SCFXML::CXMLNode& rCurrent) const;

	private:
		CString m_Name;
	};
};

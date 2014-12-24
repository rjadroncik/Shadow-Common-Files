#pragma once

#include "Method.h"
#include "Property.h"

namespace SCFScripting
{
	class SCRIPTING_API CInterface : public CDescriptor
	{
	public:
		//The class key returns the identifier of the class object, but bot the class it describes
		SCF::ENUM ClassKey() _GET { return ClassInterface; }
		CString   AsString() _GET { return STRING("{Interface}"); }

	public:
		CInterface();
		virtual ~CInterface();

	public:
		CMethodSignature* Method(_IN CString& rName, _IN CList<CClass>& rArgumentTypes) _GET;

	public:
		const CList<CMethod>&    Methods()        _GET { return m_Methods; }
		const CList<CInterface>& BaseInterfaces() _GET { return m_BaseInterfaces; }

	private:
		#pragma warning(disable:4251)
		CList<CMethod>    m_Methods;
		CList<CInterface> m_BaseInterfaces; 
		#pragma warning(default:4251)
	};
};

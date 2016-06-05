#pragma once

#include "Argument.h"
#include "CallingConventions.h"

namespace SCFScripting
{
	class SCRIPTING_API CMethodSignature : public CDescriptor
	{
	public:
		CString ToString() _GET { return STRING("{MethodSignature} ") + m_Name; }

	public:
		CMethodSignature();
		virtual ~CMethodSignature();

	public:
		void          ReturnType(_IN CClass& rType) _SET { m_pReturnType = &rType; }
		const CClass& ReturnType()                  _GET { return *m_pReturnType; }

		void               CallingConvention(_IN CallingConventions eCallingConvention) _SET{ m_eCallingConvention = eCallingConvention; }
		CallingConventions CallingConvention()                                          _GET{ return m_eCallingConvention; }

	public:
		//The accessible argument descriptor [rArgument] will be deleted when an
		//instance of this class is deleted 
		//Arguments must be specified in correct order!!!
		void ArgumentAdd(_IN _REF CArgument& rArgument) _SET;

	public:
		const CArgument& Argument(_IN UINT uiIndex) _GET { return (const CArgument&)m_Arguments[uiIndex]; }
	
	private:
		const CClass* m_pReturnType;
		CallingConventions m_eCallingConvention;

	private:
#pragma warning(disable:4251)
		CVector<CArgument> m_Arguments;
#pragma warning(default:4251)
	};
};

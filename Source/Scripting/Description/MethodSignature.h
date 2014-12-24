#pragma once

#include "Argument.h"

namespace SCFScripting
{
	enum CallingConvention
	{
		CallStd   = 0x1000, 
		CallCDecl = 0x2000, 
		CallThis  = 0x4000, 

		CallType  = 0xf000, 
	};

	class SCRIPTING_API CMethodSignature : public CDescriptor
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassMethodSignature; }
		CString   AsString() _GET { return STRING("{MethodSignature}"); }

	public:
		CMethodSignature();
		virtual ~CMethodSignature();

	public:
		void          ReturnType(_IN CClass& rType) _SET { m_pReturnType = &rType; }
		const CClass& ReturnType()                  _GET { return *m_pReturnType; }
	
	public:
		//The accessible argument descriptor [rArgument] will be deleted when an
		//instance of this class is deleted 
		//Arguments must be specified in correct order!!!
		void ArgumentAdd(_IN _REF CArgument& rArgument) _SET;

	public:
		const CArgument& Argument(_IN SCF::UINT uiIndex) _GET { return (const CArgument&)m_Arguments[uiIndex]; }
	
	private:
		const CClass* m_pReturnType;

	private:
#pragma warning(disable:4251)
		CVector<CArgument> m_Arguments;
#pragma warning(default:4251)
	};
};

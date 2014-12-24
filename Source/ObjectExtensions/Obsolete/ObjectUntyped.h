#pragma once 
#include "Object.h"

namespace SCFBase
{

	class OBJECT_EXTENSIONS_API CObjectUnTyped : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return 0xffffff; }
		CString   AsString() _GET { return STRING("{UntypedObject}"); }
	
	public:
		virtual ~CObjectSerializable() {}

	protected:
		CObjectSerializable() {}
	};
};
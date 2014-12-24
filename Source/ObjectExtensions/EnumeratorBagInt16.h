#pragma once

#include "Enumerator.h"
#include "BagInt16.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorBagInt16 : public CEnumerator
	{
		friend class OBJECT_EXTENSIONS_API CBagInt16;

	public:
		SCF::ENUM ClassKey() _GET { return ClassEnumeratorBagInt16; }
		CString   ToString() _GET { return STRING("{EnumeratorBagInt16}"); }

	public:
		CEnumeratorBagInt16(_IN CBagInt16& rBag);
		virtual ~CEnumeratorBagInt16();

 	public:
 		//This is correct :)
 		SCF::USHORT Current() _GET { return (SCF::USHORT)m_paNodes[MAX_DEPTH_BAG_INT16]; }

	public:
		void CurrentRemove();

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { m_bFinished = TRUE; return FALSE; }

	protected:
		SCFPrivate::CBagNodeInt16* m_paNodes  [MAX_DEPTH_BAG_INT16 + 1];
		char                       m_caIndexes[MAX_DEPTH_BAG_INT16];
	};
};
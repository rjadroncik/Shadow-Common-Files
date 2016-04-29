#pragma once

#include "String.h"
#include "BagNodeInt16.h"
#include "FSBHeap.h"
#include "Container.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CBagInt16 : public CContainer
	{
		friend class OBJECT_EXTENSIONS_API CEnumeratorBagInt16;

	public:
		CString ToString() _GET { return STRING("{BagInt16}"); }

	public:
		CBagInt16();
		virtual ~CBagInt16();

	public:
		void Add     (_IN SCF::USHORT usValue);
		void Remove  (_IN SCF::USHORT usValue);
		bool Contains(_IN SCF::USHORT usValue) _GET;

	public:
		//Removes all objects without deleting them
		void AllRemove();

	public:
		SCF::UINT Size()    _GET { return m_uiCount; }
		bool      IsEmpty() _GET { return (m_uiCount == 0); }

	protected:
		SCFPrivate::CBagNodeInt16* m_pNodeRoot;
		SCF::UINT m_uiCount;

	protected:
		//SCFPrivate::CFSBHeap m_Heap;
	};
};

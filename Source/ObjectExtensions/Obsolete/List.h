h#pragma once
#include "Value.h"
#include "Comparer.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CList : public CObjectSerializable
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassList; }
		CString   AsString() _GET { return STRING("{List}"); }

	public:
		CList();
		virtual ~CList();

	public:
		bool SortBubble(_IN CComparer& rComparer);

	public:
		CObject& At         (_IN SCF::UINT uiIndex) _GET;
		CObject& operator [](_IN SCF::UINT uiIndex) _GET;

		void AtPut(_IN SCF::UINT uiIndex, _IN _REF CObject& rObject) _SET;

	public:
		CObject& Last() _GET;

	public:
		void LastAdd(_IN _REF CObject& rObject);
		void LastRemove();

		void Remove(_IN SCF::UINT uiIndex);
		void Delete(_IN SCF::UINT uiIndex);

		void Remove(_IN CObject& rObject);
		void Delete(_IN CObject& rObject);

	public:
		void Swap(_IN SCF::UINT uiIndex1, _IN SCF::UINT uiIndex2);

	public:
		//Find a given object in the  List, the first performs an 
		//identity test, while the latter tests for equality of the objects
		int Find     (_IN CObject& rObject) _GET;
		int FindEqual(_IN CObject& rObject) _GET;

	public:
		//Removes all objects without deleting them
		void AllRemove() ;

		//Removes & deletes all contained objects
		void AllDelete() ;

		//Calls [Dispose()] on each object to prepare them for deletion
		void AllDispose();

	public:
		inline SCF::UINT Size()    _GET { return m_uiCount; }
		inline bool      IsEmpty() _GET { return (m_uiCount == 0); }

	protected:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);
		
		void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const;
		void DependentsDeserialize(_INOUT IStreamReadObject&  rStream);

	protected:
		CObject** m_ppObjects;
		SCF::UINT m_uiCount;
	};
};

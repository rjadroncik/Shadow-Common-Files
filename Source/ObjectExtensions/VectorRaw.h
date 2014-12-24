#pragma once
#include "Value.h"
#include "Comparer.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CVectorRange;

	class OBJECT_EXTENSIONS_API CVectorRaw : public CObjectSerializable
	{
		friend class CVectorRange;

	public:
		virtual bool IsRange() _GET { return FALSE; }

	public:
		SCF::ENUM ClassKey() _GET { return ClassVector; }
		CString   ToString() _GET { return STRING("{Vector}"); }

	public:
		CVectorRaw();
		virtual ~CVectorRaw();

	public:
		bool SortBubble(_IN CComparer& rComparer);

	public:
		inline CObject& At         (_IN SCF::UINT uiIndex) _GET { _ASSERTE(m_uiCount > 0); _ASSERTE(uiIndex < m_uiCount); return *m_ppObjects[uiIndex]; }
		inline CObject& operator [](_IN SCF::UINT uiIndex) _GET { _ASSERTE(m_uiCount > 0); _ASSERTE(uiIndex < m_uiCount); return *m_ppObjects[uiIndex]; }

		void AtPut(_IN SCF::UINT uiIndex, _IN _REF CObject& rObject) _SET;

	public:
		inline CObject& Last() _GET { _ASSERTE(m_uiCount > 0); return *m_ppObjects[m_uiCount - 1]; }

	public:
		void LastAdd(_IN _REF CObject& rObject);
		void LastRemove();
		void LastDelete();

		void RemoveAt(_IN SCF::UINT uiIndex);
		void DeleteAt(_IN SCF::UINT uiIndex);

		void Remove(_IN CObject& rObject);
		void Delete(_IN CObject& rObject);

	public:
		void Insert(_IN SCF::UINT uiIndex, _IN CObject& rObject);

	public:
		void Swap(_IN SCF::UINT uiIndex1, _IN SCF::UINT uiIndex2);

	public:
		//Find a given object in the vector based on an identity test
		int Find(_IN CObject& rObject) _GET;

		//Find a given object in the  vector based on a custom equality test
		int FindEqual(_IN CObject& rObject, _IN CComparer& rComparer) _GET;

	public:
		//Removes all objects without deleting them
		void AllRemove();

		//Removes & deletes all contained objects
		void AllDelete();

		//Calls [Dispose()] on each object to prepare them for deletion
		void AllDispose();

	public:
		inline SCF::UINT Size()    _GET { return m_uiCount; }
		inline bool      IsEmpty() _GET { return (m_uiCount == 0); }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);
		
		void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const;
		void DependentsDeserialize(_INOUT IStreamReadObject&  rStream);

	protected:
		CObject** m_ppObjects;
		SCF::UINT m_uiCount;
	};
};

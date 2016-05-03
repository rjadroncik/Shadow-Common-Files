#pragma once
#include "Value.h"
#include "Comparer.h"
#include "Container.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CVectorRangeRaw;

	class OBJECT_EXTENSIONS_API CVectorRaw : public CObject, public IContainer
	{
		friend class CVectorRangeRaw;

	public:
		virtual bool IsRange() _GET { return FALSE; }

	public:
		CString ToString() _GET { return STRING("{Vector}"); }

	public:
		CVectorRaw();
		virtual ~CVectorRaw();

	public:
		bool SortBubble(_IN CComparer& rComparer);

	public:
		inline CObject& At         (_IN UINT uiIndex) _GET { _ASSERTE(m_uiCount > 0); _ASSERTE(uiIndex < m_uiCount); return *m_ppObjects[uiIndex]; }
		inline CObject& operator [](_IN UINT uiIndex) _GET { _ASSERTE(m_uiCount > 0); _ASSERTE(uiIndex < m_uiCount); return *m_ppObjects[uiIndex]; }

		void AtPut(_IN UINT uiIndex, _IN _REF CObject& rObject) _SET;

	public:
		inline CObject& Last() _GET { _ASSERTE(m_uiCount > 0); return *m_ppObjects[m_uiCount - 1]; }

	public:
		void LastAdd(_IN _REF CObject& rObject);
		void LastRemove();
		void LastDelete();

		void RemoveAt(_IN UINT uiIndex);
		void DeleteAt(_IN UINT uiIndex);

		void Remove(_IN CObject& rObject);
		void Delete(_IN CObject& rObject);

	public:
		void Insert(_IN UINT uiIndex, _IN CObject& rObject);

	public:
		void Swap(_IN UINT uiIndex1, _IN UINT uiIndex2);

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
		inline UINT Size()    _GET { return m_uiCount; }
		inline bool IsEmpty() _GET { return (m_uiCount == 0); }

	protected:
		CObject** m_ppObjects;
		UINT m_uiCount;
	};
};

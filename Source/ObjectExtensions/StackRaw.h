#pragma once

#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStackRaw : public CObject
	{
	public:
		CString ToString() _GET { return STRING("{Stack}"); }

	public:
		CStackRaw();
		virtual ~CStackRaw();

	public:
		inline bool IsEmpty() _GET { return (m_uiCount == 0) ? (1) : (0); }
		inline UINT Size()    _GET { return m_uiCount; }

	public:
		void Push(_IN _REF CObject& rObject);
		void Pop();
		void PopAndDelete();

	public:
		CObject* Top()    _GET { return m_ppObjects[m_uiCount - 1]; }
		CObject* Bottom() _GET { return m_ppObjects[0]; }

		CObject* At(_IN UINT uiIndex) _GET { return m_ppObjects[uiIndex]; }

	public:
		//Removes all objects without deleting them
		void AllRemove();

		//Removes & deletes all contained objects
		void AllDelete();

		//Calls [Dispose()] on each object to prepare them for deletion
		void AllDispose();

	private:
		CObject** m_ppObjects;
		UINT m_uiCount;
	};
};

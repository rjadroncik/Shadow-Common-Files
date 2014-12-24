#pragma once
#include "Event.h"

namespace SCFUI
{
	class SCFUI_API CEventKey : public CEvent
	{
		friend CWindowAbstract;

	public:
		UINT VirtualKey () { return m_uiKeyCode; }
		UINT Character  () { return m_uiKeyCode; }
		UINT RepeatCount() { return m_uiRepeatCount; }

	protected:

		CEventKey(_IN ULONG uiID, _INOUT CWindowAbstract* pSource);
		virtual ~CEventKey();

		void VirtualKey (_IN UINT uiVKey)  { m_uiKeyCode = uiVKey; }
		void Character  (_IN UINT uiChar)  { m_uiKeyCode = uiChar; }
		void RepeatCount(_IN UINT uiCount) { m_uiRepeatCount = uiCount; }

	private:

		UINT m_uiKeyCode;
		UINT m_uiRepeatCount;
	};
};

#pragma once

#include "SUIMessageReceiver.h"
#include "SUIEvent.h"
#include "SUIEventMouse.h"

class CSUIAbstractWindow;

class SHADOWUI_API CSUIEventListener : public CSUIMessageReceiver
{
public:

	void                Announcer(CSUIAbstractWindow* pAnnouncer)	{ m_pSource = pAnnouncer; }
	CSUIAbstractWindow* Announcer()									{ return m_pSource; }

	virtual int		OnCreate(_IN LPCREATESTRUCT lpCreateStruct) { return 0; }
	virtual void	OnDestroy() {}

	virtual void	OnMove  (_IN Rect4i& rRect) {}
	virtual BOOL	OnMoving(_IN Rect4i& rRect) { return FALSE; }

	virtual void	OnSize  (_IN Rect4i& rRect, _IN UINT dwType) {}
	virtual BOOL	OnSizing(_IN Rect4i& rRect, _IN UINT dwSide) { return FALSE; }

	virtual void	OnMouseLeftDown         (_INCLASS CSUIEventMouse& rEvent) {}
	virtual void	OnMouseLeftUp           (_INCLASS CSUIEventMouse& rEvent) {}
	virtual void	OnMouseLeftDoubleClick  (_INCLASS CSUIEventMouse& rEvent) {}
	virtual void	OnMouseMiddleDown       (_INCLASS CSUIEventMouse& rEvent) {}
	virtual void	OnMouseMiddleUp         (_INCLASS CSUIEventMouse& rEvent) {}
	virtual void	OnMouseMiddleDoubleClick(_INCLASS CSUIEventMouse& rEvent) {}
	virtual void	OnMouseRightDown        (_INCLASS CSUIEventMouse& rEvent) {}
	virtual void	OnMouseRightUp          (_INCLASS CSUIEventMouse& rEvent) {}
	virtual void	OnMouseRightDoubleClick (_INCLASS CSUIEventMouse& rEvent) {}
 	virtual void	OnMouseMove             (_INCLASS CSUIEventMouse& rEvent) {}
	virtual void	OnMouseWheel            (_INCLASS CSUIEventMouse& rEvent) {}
	
	
	
	virtual void	OnKeyFocusReceived() {}
	virtual void	OnKeyFocusLost() {}

protected:
	CSUIEventListener();
	virtual ~CSUIEventListener();

	CSUIAbstractWindow* m_pSource;
};

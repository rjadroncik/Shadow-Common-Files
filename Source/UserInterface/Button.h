#pragma once

#include "WindowChild.h"

#include <SCFImaging.h>

namespace SCFUI
{
	class SCFUI_API CButton : public CWindowChild  
	{
	public:
		UINT Bevel()					{ return m_uiBevel; }	
		void Bevel(_IN UINT uiBevel)	{ m_uiBevel = uiBevel; }

		void                ImageActivated(_IN SCFImaging::CImage* pImage);
		SCFImaging::CImage* ImageActivated();
		
		void                ImageDeactivated(_IN SCFImaging::CImage* pImage);
		SCFImaging::CImage* ImageDeactivated();

	public:
		//Compute automatic size
		virtual void AutoRect(_OUT SCFGraphics::Rect4i& rOutRect) = 0;

	protected:
		CButton();
		virtual ~CButton();

 		bool RegisterWin32Class();
		
	protected:
		UINT m_uiBevel;
	};
};
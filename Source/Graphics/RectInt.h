#pragma once
#include "Classes.h"

namespace SCFGraphics
{
	typedef struct SCFGRAPHICS_API Rect4i
	{
	public:
		Rect4i() : iX(0), iY(0), iWidth(0), iHeight(0) {}
		Rect4i(_IN int iX, _IN int iY, _IN int iWidth, _IN int iHeight) : iX(iX), iY(iY), iWidth(iWidth), iHeight(iHeight) {}

	public:
		int iX;
		int iY;
		int iWidth;
		int iHeight;

	} Rect4i;

	bool SCFGRAPHICS_API __stdcall RectanglesIntersect(_IN Rect4i& rRect4i01, _IN Rect4i& rRect4i02);

	class SCFGRAPHICS_API CRectInt : public CValue
	{
	public:
		UINT Parse(_IN CString& rString) { UINT uiCharsParsed = 0; m_Value = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static Rect4i& Parse                        (_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static void    Parse(_OUT Rect4i& rOutValue, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static CString Print(_IN Rect4i& rValue);

	public:
		ENUM ClassKey() _GET { return ClassRectInt; }
		CString   ToString() _GET;

	public:
		CRectInt(_IN CString& rString);
		CRectInt(_IN CRectInt& rValue);
		CRectInt(_IN Rect4i&   rValue);
		CRectInt(_IN int iX, _IN int iY, _IN int iWidth, _IN int iHeight) { m_Value.iX = iX; m_Value.iY = iY; m_Value.iWidth = iWidth; m_Value.iHeight = iHeight; }

		virtual ~CRectInt();

	public:
		int  X()           _GET { return m_Value.iX; }
		void X(_IN int iX) _SET { m_Value.iX = iX; }

		int  Y()           _GET { return m_Value.iY; }
		void Y(_IN int iY) _SET { m_Value.iY = iY; }

		int  Width()               _GET { return m_Value.iWidth; }
		void Width(_IN int iWidth) _SET { m_Value.iWidth = iWidth; }

		int  Height()                _GET { return m_Value.iHeight; }
		void Height(_IN int iHeight) _SET { m_Value.iHeight = iHeight; }

	public:
		Rect4i& Value() _GET { return (Rect4i&)m_Value; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);

	protected:
		Rect4i m_Value;

	private:
		CRectInt() { m_Value.iX = 0; m_Value.iY = 0; m_Value.iWidth = 0; m_Value.iHeight = 0; }
	};
};

#pragma once
#include "Classes.h"

namespace SCFGraphics
{
	typedef struct SCFGRAPHICS_API Rect4f
	{
	public:
		Rect4f() : fX(0), fY(0), fWidth(0), fHeight(0) {}
		Rect4f(_IN float fX, _IN float fY, _IN float fWidth, _IN float fHeight) : fX(fX), fY(fY), fWidth(fWidth), fHeight(fHeight) {}

	public:
		float fX;
		float fY;
		float fWidth;
		float fHeight;

	} Rect4f;

	bool SCFGRAPHICS_API __stdcall RectanglesIntersect(_IN Rect4f& rRect4i01, _IN Rect4f& rRect4i02);

	class SCFGRAPHICS_API CRectFloat : public CValue
	{
	public:
		SCF::UINT Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; m_Value = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static Rect4f& Parse                        (_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static void    Parse(_OUT Rect4f& rOutValue, _IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static CString Print(_IN Rect4f& rValue);

	public:
		SCF::ENUM ClassKey() _GET { return ClassRectFloat; }
		CString   ToString() _GET;

	public:
		CRectFloat(_IN CString&    rString);
		CRectFloat(_IN CRectFloat& rValue);
		CRectFloat(_IN Rect4f&     rValue);
		CRectFloat(_IN float fX, _IN float fY, _IN float fWidth, _IN float fHeight) { m_Value.fX = fX; m_Value.fY = fY; m_Value.fWidth = fWidth; m_Value.fHeight = fHeight; }

		virtual ~CRectFloat();

	public:
		float X()             _GET { return m_Value.fX; }
		void  X(_IN float fX) _SET { m_Value.fX = fX; }

		float Y()             _GET { return m_Value.fY; }
		void  Y(_IN float fY) _SET { m_Value.fY = fY; }

		float Width()                 _GET { return m_Value.fWidth; }
		void  Width(_IN float fWidth) _SET { m_Value.fWidth = fWidth; }

		float Height()                  _GET { return m_Value.fHeight; }
		void  Height(_IN float fHeight) _SET { m_Value.fHeight = fHeight; }

	public:
		Rect4f& Value() _GET { return (Rect4f&)m_Value; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);

	protected:
		Rect4f m_Value;

	private:
		CRectFloat() { m_Value.fX = 0; m_Value.fY = 0; m_Value.fWidth = 0; m_Value.fHeight = 0; }
	};
};

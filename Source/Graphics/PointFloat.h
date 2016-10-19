#pragma once
#include "Classes.h"
#include "RectFloat.h"

namespace SCFGraphics
{
	typedef struct SCFGRAPHICS_API Point2f
	{
	public:
		Point2f() : fX(0), fY(0) {}
		Point2f(_IN float fX, _IN float fY) : fX(fX), fY(fY) {}

	public:
		float fX;
		float fY;

	} Point2f;

	bool SCFGRAPHICS_API __stdcall PointInRect(_IN Point2f& rPoint2f,      _IN Rect4f& rRect4f);
	bool SCFGRAPHICS_API __stdcall PointInRect(_IN float fX, _IN float fY, _IN Rect4f& rRect4f);

	class SCFGRAPHICS_API CPointFloat : public CValue
	{
	public:
		UINT Parse(_IN CString& rString) { UINT uiCharsParsed = 0; m_Value = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static Point2f& Parse                        (_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static void    Parse(_OUT Point2f& rOutValue, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static CString Print(_IN Point2f& rValue);

	public:
		CString ToString() _GET;

	public:
		CPointFloat(_IN CString&    rString);
		CPointFloat(_IN CPointFloat& rValue);
		CPointFloat(_IN Point2f&     rValue);
		CPointFloat(_IN float fX, _IN float fY) { m_Value.fX = fX; m_Value.fY = fY; }

		virtual ~CPointFloat();

	public:
		float X()             _GET { return m_Value.fX; }
		void  X(_IN float fX) _SET { m_Value.fX = fX; }

		float Y()             _GET { return m_Value.fY; }
		void  Y(_IN float fY) _SET { m_Value.fY = fY; }

	public:
		Point2f& Value() _GET { return (Point2f&)m_Value; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);

	protected:
		Point2f m_Value;

	private:
		CPointFloat() { m_Value.fX = 0; m_Value.fY = 0; }
	};
};

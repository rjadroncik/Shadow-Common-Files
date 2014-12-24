#pragma once
#include "Classes.h"
#include "RectInt.h"

namespace SCFGraphics
{
	typedef struct SCFGRAPHICS_API Point2i
	{
	public:
		Point2i() : iX(0), iY(0) {}
		Point2i(_IN int iX, _IN int iY) : iX(iX), iY(iY) {}

	public:
		int iX;
		int iY;

	} Point2i;

	bool SCFGRAPHICS_API __stdcall PointInRect(_IN Point2i& rPoint,    _IN Rect4i& rRect);
	bool SCFGRAPHICS_API __stdcall PointInRect(_IN int iX, _IN int iY, _IN Rect4i& rRect);

	class SCFGRAPHICS_API CPointInt : public CValue
	{
	public:
		SCF::UINT Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; m_Value = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static Point2i& Parse                        (_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static void    Parse(_OUT Point2i& rOutValue, _IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static CString Print(_IN Point2i& rValue);

	public:
		SCF::ENUM ClassKey() _GET { return ClassPointInt; }
		CString   ToString() _GET;

	public:
		CPointInt(_IN CString& rString);
		CPointInt(_IN CPointInt& rValue);
		CPointInt(_IN Point2i&   rValue);
		CPointInt(_IN int iX, _IN int iY) { m_Value.iX = iX; m_Value.iY = iY; }

		virtual ~CPointInt();

	public:
		int  X()           _GET { return m_Value.iX; }
		void X(_IN int iX) _SET { m_Value.iX = iX; }

		int  Y()           _GET { return m_Value.iY; }
		void Y(_IN int iY) _SET { m_Value.iY = iY; }

	public:
		Point2i& Value() _GET { return (Point2i&)m_Value; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);

	protected:
		Point2i m_Value;

	private:
		CPointInt() { m_Value.iX = 0; m_Value.iY = 0; }
	};
};

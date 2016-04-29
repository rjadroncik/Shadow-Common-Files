#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CFloat2 : public CValue
	{
		friend class CValue;

	public:
		SCF::UINT Parse(_IN CString& rString);

	public:
		static SCFMathematics::Float2& Parse                                        (_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static void                    Parse(_OUT SCFMathematics::Float2& rOutValue, _IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static CString                 Print(_IN  SCFMathematics::Float2& rValue);

	public:
		CString ToString() _GET;

	public:
		CFloat2(_IN SCFMathematics::Float2& rValue);
		CFloat2(_IN float fX, _IN float fY);
		CFloat2(_IN CString& rString);
		CFloat2(_IN CFloat2& rValue);

		virtual ~CFloat2();

	public:
		float At         (_IN SCF::UINT uiIndex) _GET;
		float operator [](_IN SCF::UINT uiIndex) _GET;

	public:
		void AtPut(_IN SCF::UINT uiIndex, _IN float fValue) _SET;

	public:
		inline const SCFMathematics::Float2& Value() _GET { return m_Value; }

	protected:
		SCFMathematics::Float2 m_Value;

	private:
		CFloat2() { m_Value[0] = 0; m_Value[1] = 0; }
	};
};

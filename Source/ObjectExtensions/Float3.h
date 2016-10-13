#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CFloat3 : public CValue
	{
		friend class CValue;

	public:
		UINT Parse(_IN CString& rString);

	public:
		ValueKind Kind() _GET { return ValueFloat3; }

	public:
		static SCFMathematics::Float3& Parse                                        (_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static void                    Parse(_OUT SCFMathematics::Float3& rOutValue, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static CString                 Print(_IN  SCFMathematics::Float3& rValue);

	public:
		CString ToString() _GET;

	public:
		CFloat3(_IN SCFMathematics::Float3& rValue);
		CFloat3(_IN float fX, _IN float fY, _IN float fZ);
		CFloat3(_IN CString& rString);
		CFloat3(_IN CFloat3& rValue);

		virtual ~CFloat3();

	public:
		float At         (_IN UINT uiIndex) _GET;
		float operator [](_IN UINT uiIndex) _GET;

	public:
		void AtPut(_IN UINT uiIndex, _IN float fValue) _SET;

	public:
		inline const SCFMathematics::Float3& Value() _GET { return m_Value; }

	protected:
		SCFMathematics::Float3 m_Value;

	private:
		CFloat3() { m_Value[0] = 0; m_Value[1] = 0; m_Value[2] = 0; }
	};
};

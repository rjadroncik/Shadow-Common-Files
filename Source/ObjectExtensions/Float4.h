#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CFloat4 : public CValue
	{
		friend class CValue;

	public:
		UINT Parse(_IN CString& rString);

	public:
		static SCFMathematics::Float4& Parse                                        (_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static void                    Parse(_OUT SCFMathematics::Float4& rOutValue, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static CString                 Print(_IN  SCFMathematics::Float4& rValue);

	public:
		CString ToString() _GET;

	public:
		CFloat4(_IN SCFMathematics::Float4& rValue);
		CFloat4(_IN float fX, _IN float fY, _IN float fZ, _IN float fW);
		CFloat4(_IN CString& rString);
		CFloat4(_IN CFloat4& rValue);

		virtual ~CFloat4();

	public:
		float At         (_IN UINT uiIndex) _GET;
		float operator [](_IN UINT uiIndex) _GET;

	public:
		void AtPut(_IN UINT uiIndex, _IN float fValue) _SET;

	public:
		inline const SCFMathematics::Float4& Value() _GET { return m_Value; }

	protected:
		SCFMathematics::Float4 m_Value;

	private:
		CFloat4() { m_Value[0] = 0; m_Value[1] = 0; m_Value[2] = 0; m_Value[3] = 0; }
	};
};

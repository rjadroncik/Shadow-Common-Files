#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CFloat3 : public CValue
	{
		friend class CValue;

	public:
		SCF::UINT Parse(_IN CString& rString);

	public:
		static SCFMathematics::Float3& Parse                                        (_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static void                    Parse(_OUT SCFMathematics::Float3& rOutValue, _IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static CString                 Print(_IN  SCFMathematics::Float3& rValue);

	public:
		SCF::ENUM ClassKey() _GET { return ClassFloat3; }
		CString   ToString() _GET;

	public:
		CFloat3(_IN SCFMathematics::Float3& rValue);
		CFloat3(_IN float fX, _IN float fY, _IN float fZ);
		CFloat3(_IN CString& rString);
		CFloat3(_IN CFloat3& rValue);

		virtual ~CFloat3();

	public:
		float At         (_IN SCF::UINT uiIndex) _GET;
		float operator [](_IN SCF::UINT uiIndex) _GET;

	public:
		void AtPut(_IN SCF::UINT uiIndex, _IN float fValue) _SET;

	public:
		inline const SCFMathematics::Float3& Value() _GET { return m_Value; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);

	protected:
		SCFMathematics::Float3 m_Value;

	private:
		CFloat3() { m_Value[0] = 0; m_Value[1] = 0; m_Value[2] = 0; }
	};
};

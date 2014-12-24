#pragma once

#include "Headers.h"

using namespace SCFBase;

namespace SCFTextRendering
{
	//Basic class for storing property changes, used by - CStyle
	class CProperty : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return SCFTextRendering::ClassProperty; }
		CString   ToString() _GET { return STRING("{Property}"); }

	public:
		CProperty(_IN int iID, _IN CValue& rValue);
		~CProperty();

	public:
		//Sets or retrieves identification enum value (i.e. CharacterProperties.CP_KERNING_TYPE, ..)
		void ID(_IN int iID) _SET { m_iID = iID; }
		int  ID()            _GET { return m_iID; }
		
	public:
		CValue& Value()                        _GET { return *m_pValue; }
		void    Value(_IN _REF CValue& rValue) _SET;

	private:
		//ID specifier, a value from the one of the properties enumerations (CharacterProperties, ParagraphProperties, TextFrameProperties, ..)
		int	m_iID;

		//The actual value of the property
		CValue* m_pValue;
	};
};

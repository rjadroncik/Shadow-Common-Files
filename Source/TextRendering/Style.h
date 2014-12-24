#pragma once

#include "Property.h"

using namespace SCFBase;

namespace SCFTextRendering
{
	//An abstract style class, which encapsulates and manages a property list
	class TEXT_RENDERING_API CStyle : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return SCFTextRendering::ClassStyle; }
		CString   ToString() _GET { return STRING("{Style}"); }

	public:
		CStyle(SCF::ENUM eType);
		~CStyle();

	public:
		//Identification functions
		void ID(_IN int iID) _SET { m_iID = iID; } //Done
		int  ID()            _GET { return m_iID; } //Done

	public:
		//Type query function
		SCF::ENUM Type() _GET { return m_eType; } //Done

	public:
		//Base style identification functions
		void BaseID(_IN int iID) _SET { m_iBaseStyleID = iID; m_bHasBaseStyle = TRUE; } //Done
		int  BaseID()            _GET { return m_iBaseStyleID; } //Done

		bool HasBaseStyle()   _GET { return m_bHasBaseStyle; } //Done
		void ClearBaseStyle() _SET { m_bHasBaseStyle = FALSE; } //Done

	public:
		//Sets, gets & deletes an existing property
		bool       Property      (_IN int iID, _IN CValue& rValue) _SET; //Done
		CProperty* Property      (_IN int iID)                     _GET; //Done
		bool       PropertyDelete(_IN int iID);

		CDictionaryInt64& Properties() _GET { return (CDictionaryInt64&)m_Properties; }

	private:
		int	m_iID;

		//Type of style, a member of the [StyleTypes] enumeration, initialized by constructor, cannot be changed from outside
		SCF::ENUM m_eType;

		int  m_iBaseStyleID;
		bool m_bHasBaseStyle;

		CDictionaryInt64 m_Properties;
	};
};

#pragma once

#include "Style.h"

namespace SCFTextRendering
{
	//Used to input & store tab information, valid aligns are: [ALIGN_LEFT], [ALIGN_CENTER], [ALIGN_RIGHT], [ALIGN_CHARACTER]
	class TEXT_RENDERING_API CTabEntry : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassTabEntry; }
		CString   ToString() _GET { return STRING("{TabEntry}"); }

	public:
		int       iPosition;	//Done
		SCF::ENUM eUnit;		//Done

		//[iCharacter] is used in conjunction with [ALIGN_CHARACTER]
		SCF::ENUM eAlign;		//Done
		int       iCharacter;	//Done
	};

	class CParagraphStyleStack
	{
	public:
		static CTabEntry* QueryTabEntry(_IN int iTabIndex); //Done
		static int        QueryTabCount() { return (int)s_Style.Tabs.Size(); } //Done

		static bool QueryIntegerValue(_IN SCF::ENUM eProperty, _OUT int* fpOutInteger); //Done

		//This function updates paragraph style state values
		static bool ExecuteStyle(_IN CStyle& rStyle); //Done

		//Manipulates style stack - [PushStyle(..)] backs-up current style state on the stack & leaves it untouched for further ie
		static bool PushStyleStack(_IN CStyle& rStyle); //Done
		static bool PopStyleStack (_IN CStyle& rStyle); //Done

		//This function loads global default paragraph style values
		static void ResetStyleState(); //Done
		static void FlushStyleStack(); //Done
		
		static void Initialize(); //Done

	private:
		class CStyleState;

	public:
		inline static CStyleState& Top() { return s_Style; }

	private:
		CParagraphStyleStack();
		~CParagraphStyleStack();

	private:
		//Structure defining paragraph style state - text rendering is based on the immediate values of the variables in this structure 
		class CStyleState : public CObject
		{
		public:
			SCF::ENUM ClassKey() _GET { return 0xffffffff; }
			CString   ToString() _GET { return STRING("{StyleState}"); }

		public:
			//The style that is stored on this stack element/entry
			const CStyle* pStyle;

			//Text alignment within paragraph
			SCF::ENUM eAlignHorizontal;

			//This structure stores various style dependent indents used to position & size lines
			struct SIndent
			{
				int	iLineFirst;		

				int	iLeft;		
				int	iRight;		

				int	iBefore;		
				int	iAfter;		

			} Indent;

			SCFGraphics::ColorRGBA	BackgroundColor;

			//List of tab entries 
			CVector Tabs;

		} static s_Style;

	private:
		static CStack s_Styles;
		static CStack s_PopQueue;
	};
};

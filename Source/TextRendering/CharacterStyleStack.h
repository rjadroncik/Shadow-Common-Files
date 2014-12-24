#pragma once

#include "Style.h"

using namespace SCFBase;

namespace SCFTextRendering
{
	class CCharacterStyleStack
	{
	public:
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
		
		//Updates cache to match current style state
		static bool UpdateStyleState(); //Done

	private:
		class CStyleState;

	public:
		inline static CStyleState& Top() { return s_Style; }

	private:
		CCharacterStyleStack();
		~CCharacterStyleStack();

		//Structure defining character style state - text rendering is based on the immediate values of the variables in this structure 
		class CStyleState : public CObject
		{
		public:
			SCF::ENUM ClassKey() _GET { return 0xffffffff; }
			CString   ToString() _GET { return STRING("{StyleState}"); }

		public:
			//The style that is stored on this stack element/entry
			const CStyle* pStyle;

			struct tFontStyle
			{
				//This variable contains the family name string & is passed directly to the [FontFamily::FontFamily(..)] constructor
				CString Family;
				CString AlternateFamily;

				//Defines the font style - a combination of values form the GDI+'s [FontStyle] enum 
				SCF::DWORD dwStyle;

				int iSize;

			} Font;

			SCFGraphics::ColorRGBA	Color;

			//Leading
			int iLeading;
			int iTracking;

			SCF::ENUM ePosition;
			SCF::ENUM eKerning;
			SCF::ENUM eCase;

			struct tLiningStyle
			{
				//Defines what type of lining to use
				SCF::ENUM eType;

				//Defines the line appearance
				SCFGraphics::ColorRGBA	Color;
				int iWidth;

			} Underline, StrikeThrough;

		} static s_Style;

		//The stack & pop queue keep track of style changes & allow style overlapping
		static CStack s_Styles;
		static CStack s_PopQueue;

		static bool s_bFontChanged;
	};
};

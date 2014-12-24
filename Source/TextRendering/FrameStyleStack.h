#pragma once

#include "Style.h"

namespace SCFTextRendering
{
	class CFrameStyleStack
	{
	public:
		//This function updates paragraph style state values
		static bool ExecuteStyle(_IN CStyle& rStyle); //Done

		//Provided, as a way to access certain text-frame properties 
		static bool QueryIntegerValue(_IN SCF::ENUM eProperty, _OUT int* fpOutInteger);

		//Manipulates style stack - [PushStyle(..)] backs-up current style state on the stack & leaves it untouched for further ie
		static bool PushStyleStack(); //Done
		static bool PopStyleStack(); //Done

		//This function loads global default paragraph style values
		static void ResetStyleState(); //Done
		static void FlushStyleStack(); //Done
		
		static void Initialize(); //Done

	private:
		class CStyleState;

	public:
		inline static CStyleState& Top() { return s_Style; }

	private:
		CFrameStyleStack();
		~CFrameStyleStack();

	private:
		class CStyleState : public CObject
		{
		public:
			SCF::ENUM ClassKey() _GET { return 0xffffffff; }
			CString   ToString() _GET { return STRING("{StyleState}"); }

		public:
			//Column information
			struct tColumnInfo
			{
				int iCount;

				int iWidth;
				bool bFixedWidth;

				//Spacing between columns
				int iGutter;

			} Column;

			//Contains inset information
			SCFGraphics::Rect4i	Inset;

			//Specifies the first base-line options
			SCF::ENUM eFirstBaseline;
			int	      iFirstBaselineOffset;

			//Standard tab width, can be accessed from outside via [QueryIntegerValue(..)], it's initial value is not yet clearly defined !!!
			int iTabWidth;

			//These values are all fractions - they affect scaling & positioning of text during rendering, but they do NOT affect the layout (based on InDesign)
			int iSuperScriptSize;
			int iSuperScriptPosition;

			int iSubScriptSize;
			int iSubScriptPosition;

			int iSmallCapsSize;

			//Paragraph alignment within text-frame
			SCF::ENUM eAlignVertical;

			//Output scale
			int iOutputScale;

			bool bContinueParagraph;
			bool bIgnoreLineLastDescent;

		} static s_Style;

		static CStack s_Styles;
	};
};

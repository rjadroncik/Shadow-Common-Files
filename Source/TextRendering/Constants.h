#pragma once

#include <SCFStandard.h>

namespace SCFTextRendering
{
	#define PT_2_INTERNAL 1000

	#define CHARACTER_PROPERTY_MIN_VALUE 100
	#define PARAGRAPH_PROPERTY_MIN_VALUE 500
	#define TEXTFRAME_PROPERTY_MIN_VALUE 1000

	#define TEXTFRAME_PROPERTY_MAX_VALUE 2000

	//Character property enumeration - therefore CP_...
	enum CharacterProperties
	{
		CP_FONT_FAMILY = CHARACTER_PROPERTY_MIN_VALUE, //(string) //Done
		CP_FONT_STYLE,			//(string, specific) ["Regular", "Bold", "Italic", "Bold Italic"] -> CASE SENSITIVE! //Done
		CP_SIZE,				//(int, absolute) //Done
		CP_COLOR,				//(RGBA, 32bit) //Done
		CP_KERNING_TYPE,		//(enum) //Done
		CP_CASE,				//(enum) //Done
		CP_LEADING,				//(int, relative) //Done
		CP_TRACKING,			//(int, relative) //Done
		CP_POSITION,			//(enum) //Done
		CP_UNDERLINE_TYPE,		//(enum) //Done
		CP_STRIKETHROUGH_TYPE,	//(enum) //Done
		CP_LIGATURES,			//(bool)
		CP_ALTERNATE_FAMILY,	//(string) //Done

		CP_UNDERLINE_COLOR,		//(RGBA, 32bit) //Done
		CP_UNDERLINE_WIDTH,		//(int, absolute) //Done

		CP_STRIKETHROUGH_COLOR,	//(RGBA, 32bit) //Done
		CP_STRIKETHROUGH_WIDTH,	//(int, absolute) //Done
	};

	enum ParagraphProperties
	{
		PP_FIRSTLINE_INDENT = PARAGRAPH_PROPERTY_MIN_VALUE, //(int, absolute) //Done
		PP_INDENT_LEFT,			//(int, absolute) //Done
		PP_INDENT_RIGHT,		//(int, absolute) //Done
		PP_SPACE_BEFORE,		//(int, absolute) //Done
		PP_SPACE_AFTER,			//(int, absolute) //Done
		PP_BACKGROUND_COLOR,	//(RGBA, 32bit) //Done

		//Specified using [TR_SetComplexProperty(..)] as an array of [CTabEntry] structures filled with relevant information
		PP_TAB_LIST,			//(complex, array of [CTabEntry]) //Done
		PP_ALIGN_HORIZONTAL,	//(enum) //Done
	};

	enum TextFrameProperties
	{
		//Standard tab width
		FP_TAB_WIDTH = TEXTFRAME_PROPERTY_MIN_VALUE, //(int, absolute) //Done

		//Baseline options for the first line - added to achieve consistency with InDesign
		FP_FIRST_BASELINE,				//(enum) //Done
		FP_FIRST_BASELINE_OFFSET,		//(int, absolute) //Done

		FP_INDENT_LEFT,					//(int, absolute) //Done
		FP_INDENT_RIGHT,				//(int, absolute) //Done
		FP_INDENT_TOP,					//(int, absolute) //Done
		FP_INDENT_BOTTOM,				//(int, absolute) //Done

		//Percentual/relative scale & offset used ONLY for rendering, sub/super-script size is computed from font size, position is defines how much the baseline of the text will be moved down/up, this offset is based on font size
		FP_TEXT_SUPERSCRIPT_SIZE = TEXTFRAME_PROPERTY_MIN_VALUE + 20,//(int, relative) //Done
		FP_TEXT_SUPERSCRIPT_POSITION,	//(int, relative) //Done
		FP_TEXT_SUBSCRIPT_SIZE,			//(int, relative) //Done
		FP_TEXT_SUBSCRIPT_POSITION,		//(int, relative) //Done

		//Percentual/relative scale used also for layout, the font is scaled from its normal height & then measured/rendered
		FP_TEXT_SMALLCAPS_SIZE,			//(int, relative) //Done

		FP_COLUMN_COUNT = TEXTFRAME_PROPERTY_MIN_VALUE + 40, //(integer, unsigned) //Done
		FP_COLUMN_WIDTH,				//(int, absolute) //Done

		//[FP_COLUMN_WIDTH_FIXED] will cause the measurement function [TR_MeasureTextFrame(..)] to fail, this is only a layout feature & it is not logical to try to use it when measuring is required
		FP_COLUMN_WIDTH_FIXED,			//(bool) //Done
		FP_COLUMN_GUTTER,				//(int, absolute) //Done

		//Vertical alignment of text/paragraphs within the frame
		FP_ALIGN_VERTICAL,				//(enum) //Done

		//Used to simulate text flow from one frame to another
		FP_CONTINUE_PARAGRAPH = TEXTFRAME_PROPERTY_MIN_VALUE + 60, //(bool) //Done

		//If used, threats the last line in each column as if its descent part was cut-off, this affects vertical fitting, measuring & vertical aligment
		FP_IGNORE_LAST_LINE_DESCENT,	//(bool) //Done
	};

	//One of these style types must be selected using [SetStyleType(..)] prior to calling style-related functions (i.e.: [AddStyle(..)], [ApplyStyle(..)], ..)
	enum StyleTypes
	{
		STYLE_CHARACTER = 1,	//Done
		STYLE_PARAGRAPH,		//Done
		STYLE_TEXTFRAME,		//Done
	};

	//Used in calls to [GetContentStats(..)]
	enum Statistics
	{
		STATS_CHARACTERS = 1,	//Done
		STATS_WORDS,			//Done
		STATS_LINES,			//Done (No need for a [STATS_LINES_FIT] option, as lines are created bsed on layout & their count is always the same as the visible count)
		STATS_PARAGRAPHS,		//Done

		STATS_CHARACTERS_FIT = 16,	//Done
		STATS_WORDS_FIT,			//Done
		STATS_PARAGRAPHS_FIT,		//Done
	};

	//Specifies character positioning & size, used with the [CP_POSITION] property
	enum Positions
	{
		POS_NORMAL = 1,		//Done
		POS_SUBSCRIPT,		//Done
		POS_SUPERSCRIPT,	//Done
	};

	//Specifies character kerning, used with the [CP_KERNING_TYPE] property
	enum KerningTypes
	{
		KERNING_NONE = 1,	//Done
		KERNING_METRICS,	//Done
	};

	//Specifies character kerning, used with the [CP_CASE] property
	enum CaseTypes
	{
		CASE_NORMAL = 1,	//Done

		CASE_ALLSMALL,		//Done
		CASE_ALLCAPS,		//Done

		CASE_SMALLCAPS,		//Done
	};

	//[BL_FIXED] is used in conjunction with [FP_FIRST_BASELINE_OFFSET]
	enum BaselineTypes
	{
		BL_ASCENT = 1,	//Done
		BL_LEADING,		//Done
		BL_FIXED,		//Done
	};

	enum GenericAligments
	{
		//[ALIGN_JUSTIFY] can be combined with [ALIGN_LEFT], [ALIGN_CENTER], [ALIGN_RIGHT]
		ALIGN_LEFT				= 0x0001, //Done
		ALIGN_CENTER			= 0x0002, //Done
		ALIGN_RIGHT				= 0x0004, //Done
		ALIGN_JUSTIFY			= 0x0008,

		//Justifies even a single word in a line - stretches it to fill out the whole line
		ALIGN_FULL_JUSTIFY		= 0x0100, //Reserved

		ALIGN_TOP				= 0x0010, //Done
		ALIGN_CENTER_VERTICAL	= 0x0020, //Done
		ALIGN_BOTTOM			= 0x0040, //Done
		ALIGN_JUSTIFY_VERTICAL	= 0x0080, //Done

		ALIGN_CHARACTER			= 0x1000, //Done
	};

	enum StyleVaidationResults
	{
		//Set when the font families specified using [CP_FONT_FAMILY, CP_ALTERNATE_FAMILY], are not installed on the system
		SVR_FONT_FAMILY_NOT_INSTALLED			= 0x00000001, //Done
		SVR_ALTERNATE_FAMILY_NOT_INSTALLED		= 0x00000002, //Done

		//Used for convenient testing
		SVR_FONTS_NOT_INSTALLED					= SVR_FONT_FAMILY_NOT_INSTALLED | SVR_ALTERNATE_FAMILY_NOT_INSTALLED, //Done
	};

	enum UpdateOptions
	{
		UO_UPDATE_LAYOUT			= 0x00000001, //Done
		UO_UPDATE_SELECTION			= 0x00000002, //Done

		UO_UPDATE_ALL				= 0xffffffff, //Done
	};

	enum RenderOptions
	{
		RO_DRAW_WORD_OUTLINES			= 0x00000001, //Done
		RO_DRAW_LINE_OUTLINES			= 0x00000002, //Done
		RO_DRAW_PARAGRAPH_OUTLINES		= 0x00000004, //Done
		RO_DRAW_COLUMN_OUTLINES			= 0x00000008, //Done
		RO_DRAW_TEXTFRAME_OUTLINE		= 0x00000010, //Done

		RO_DRAW_SELECTION				= 0x00000100, //Done

		RO_QUALITY_DRAFT				= 0x00001000, //Done
		RO_QUALITY_NORMAL				= 0x00002000, //Done
		RO_QUALITY_PRINT				= 0x00003000, //Done

		RO_DRAW_ALL						= 0x00000fff, //Done
	};

	//Used both with underline & strike-through style properties
	enum UnderlineAndStrikeTroughTypes
	{
		//Measure the height required for the text-frame given a specific width
		US_NONE = 1,

		US_NORMAL,
		US_DOTTED,
		US_DASHED,
		US_DASH_DOT,
		US_DASH_DOT_DOT,

		US_WAVE,
	};
};

#pragma once

namespace SCFMathematics
{
	#define SCFAccuracy 0.00005

	#define SCFDegToRad 0.0174532925199432957692369076f
	#define SCFRadToDeg 57.295779513082320876798154814f

	#define SCFPi 3.1415926535897932384626433832f

	//Preprocessor macros
	#define __abs(a)   (((a) >  0)  ? (a) : -(a)) 
	#define __max(a,b) (((a) > (b)) ? (a) :  (b))
	#define __min(a,b) (((a) < (b)) ? (a) :  (b))

	#define __sqr(a) ((a) * (a))

	//Standard type definitions
	typedef char Char2[2];
	typedef char Char3[3];
	typedef char Char4[4];

	typedef short Short2[2];
	typedef short Short3[3];
	typedef short Short4[4];

	typedef int Int3[3];
	typedef int Int2[2];
	typedef int Int4[4];

	typedef float Float2[2];
	typedef float Float3[3];
	typedef float Float4[4];

	typedef double Double2[2];
	typedef double Double3[3];
	typedef double Double4[4];

	typedef double Matrix3[3][3];
	typedef double Matrix4[4][4];
};

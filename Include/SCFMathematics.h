#pragma once

#include <SCFStandard.h>
#include <SCFMathematicsTypes.h>

#include <math.h>
#include <stdlib.h>

#define SCF_MATHEMATICS_1_2

namespace SCFMathematics
{
	//Vector algebra
	inline bool CompareVectors3(_IN Float3& rVector1, _IN Float3& rVector2) { if (rVector1[0] != rVector2[0]) { return FALSE; } if (rVector1[1] != rVector2[1]) { return FALSE; } if (rVector1[2] != rVector2[2]) { return FALSE; } return TRUE; }
	inline bool CompareVectors4(_IN Float4& rVector1, _IN Float4& rVector2) { if (rVector1[0] != rVector2[0]) { return FALSE; } if (rVector1[1] != rVector2[1]) { return FALSE; } if (rVector1[2] != rVector2[2]) { return FALSE; } if (rVector1[3] != rVector2[3]) { return FALSE; } return TRUE; }

	//Set{...}
	inline void SetVector2(_OUT Char2& rVector, _IN char bX, _IN char bY)                           { rVector[0] = bX; rVector[1] = bY; } 
	inline void SetVector3(_OUT Char3& rVector, _IN char bX, _IN char bY, _IN char bZ)              { rVector[0] = bX; rVector[1] = bY; rVector[2] = bZ; } 
	inline void SetVector4(_OUT Char4& rVector, _IN char bX, _IN char bY, _IN char bZ, _IN char bW) { rVector[0] = bX; rVector[1] = bY; rVector[2] = bZ; rVector[3] = bW; } 

	inline void SetVector2(_OUT Short2& rVector, _IN short sX, _IN short sY)                             { rVector[0] = sX; rVector[1] = sY; } 
	inline void SetVector3(_OUT Short3& rVector, _IN short sX, _IN short sY, _IN short sZ)               { rVector[0] = sX; rVector[1] = sY; rVector[2] = sZ; } 
	inline void SetVector4(_OUT Short4& rVector, _IN short sX, _IN short sY, _IN short sZ, _IN short sW) { rVector[0] = sX; rVector[1] = sY; rVector[2] = sZ; rVector[3] = sW; } 

	inline void SetVector2(_OUT Int2& rVector, _IN int iX, _IN int iY)                           { rVector[0] =  iX; rVector[1] =  iY; } 
	inline void SetVector3(_OUT Int3& rVector, _IN int iX, _IN int iY, _IN int iZ)               { rVector[0] =  iX; rVector[1] =  iY; rVector[2] =  iZ; } 
	inline void SetVector4(_OUT Int4& rVector, _IN int iX, _IN int iY, _IN int iZ, _IN int   iW) { rVector[0] =  iX; rVector[1] =  iY; rVector[2] =  iZ; rVector[3] =  iW; } 

	inline void SetVector2(_OUT Float2& rVector, _IN float fX, _IN float fY)                             { rVector[0] =  fX; rVector[1] =  fY; } 
	inline void SetVector3(_OUT Float3& rVector, _IN float fX, _IN float fY, _IN float fZ)               { rVector[0] =  fX; rVector[1] =  fY; rVector[2] =  fZ; } 
	inline void SetVector4(_OUT Float4& rVector, _IN float fX, _IN float fY, _IN float fZ, _IN float fW) { rVector[0] =  fX; rVector[1] =  fY; rVector[2] =  fZ; rVector[3] =  fW; } 

	//Zero{...}
	inline void ZeroVector2(_OUT Int2& rVector) { rVector[0] = 0; rVector[1] = 0; } 
	inline void ZeroVector3(_OUT Int3& rVector) { rVector[0] = 0; rVector[1] = 0; rVector[2] = 0; } 
	inline void ZeroVector4(_OUT Int4& rVector) { rVector[0] = 0; rVector[1] = 0; rVector[2] = 0; rVector[3] = 0; } 

	inline void ZeroVector2(_OUT Float2& rVector) { rVector[0] = 0; rVector[1] = 0; } 
	inline void ZeroVector3(_OUT Float3& rVector) { rVector[0] = 0; rVector[1] = 0; rVector[2] = 0; } 
	inline void ZeroVector4(_OUT Float4& rVector) { rVector[0] = 0; rVector[1] = 0; rVector[2] = 0; rVector[3] = 0; } 

	//Copy{...}
	inline void CopyVector2(_OUT Int2& rDestination, _IN Int2&  rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; } 
	inline void CopyVector3(_OUT Int3& rDestination, _IN Int3&  rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; rDestination[2] = rSource[2]; } 
	inline void CopyVector4(_OUT Int4& rDestination, _IN Int4&  rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; rDestination[2] = rSource[2]; rDestination[3] = rSource[3]; } 

	inline void CopyVector2(_OUT Char2& rDestination, _IN Char2& rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; } 
	inline void CopyVector3(_OUT Char3& rDestination, _IN Char3& rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; rDestination[2] = rSource[2]; } 
	inline void CopyVector4(_OUT Char4& rDestination, _IN Char4& rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; rDestination[2] = rSource[2]; rDestination[3] = rSource[3]; } 
	
	inline void CopyVector2(_OUT Short2& rDestination, _IN Short2& rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; } 
	inline void CopyVector3(_OUT Short3& rDestination, _IN Short3& rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; rDestination[2] = rSource[2]; } 
	inline void CopyVector4(_OUT Short4& rDestination, _IN Short4& rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; rDestination[2] = rSource[2]; rDestination[3] = rSource[3]; } 

	inline void CopyVector2(_OUT Float2& rDestination, _IN Float2&  rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; }  
	inline void CopyVector3(_OUT Float3& rDestination, _IN Float3&  rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; rDestination[2] = rSource[2]; }  
	inline void CopyVector4(_OUT Float4& rDestination, _IN Float4&  rSource) { rDestination[0] = rSource[0]; rDestination[1] = rSource[1]; rDestination[2] = rSource[2]; rDestination[3] = rSource[3]; } 

	//Invert{...}
	inline void InvertVector2(_OUT Float2& rDestination, _IN Float2& rSource) { rDestination[0] = -rSource[0]; rDestination[1] = -rSource[1]; }  
	inline void InvertVector3(_OUT Float3& rDestination, _IN Float3& rSource) { rDestination[0] = -rSource[0]; rDestination[1] = -rSource[1]; rDestination[2] = -rSource[2]; }  
	inline void InvertVector4(_OUT Float4& rDestination, _IN Float4& rSource) { rDestination[0] = -rSource[0]; rDestination[1] = -rSource[1]; rDestination[2] = -rSource[2]; rDestination[3] = -rSource[3]; }  

	//Make{...}
	inline void MakeVector2(_OUT Float2& rResult, _IN Float2& rPoint1, _IN Float2& rPoint2)	 { rResult[0] = rPoint2[0] - rPoint1[0]; rResult[1] = rPoint2[1] - rPoint1[1]; }
	inline void MakeVector3(_OUT Float3& rResult, _IN Float3& rPoint1, _IN Float3& rPoint2)	 { rResult[0] = rPoint2[0] - rPoint1[0]; rResult[1] = rPoint2[1] - rPoint1[1]; rResult[2] = rPoint2[2] - rPoint1[2]; } 
	inline void MakeVector3(_OUT Float3& rResult, _IN Float3& rDirection, _IN float fLength) { rResult[0] = rDirection[0] * fLength; rResult[1] = rDirection[1] * fLength; rResult[2] = rDirection[2] * fLength; }

	//Average{...}
	inline void AverageVectors2(_OUT Float2& rResult, _IN Float2& rPoint1, _IN Float2& rPoint2) { rResult[0] = (rPoint1[0] + rPoint2[0]) / 2.0f; rResult[1] = (rPoint1[1] + rPoint2[1]) / 2.0f; }  
	inline void AverageVectors3(_OUT Float3& rResult, _IN Float3& rPoint1, _IN Float3& rPoint2) { rResult[0] = (rPoint1[0] + rPoint2[0]) / 2.0f; rResult[1] = (rPoint1[1] + rPoint2[1]) / 2.0f; rResult[2] = (rPoint1[2] + rPoint2[2]) / 2.0f; }  
	inline void AverageVectors4(_OUT Float4& rResult, _IN Float4& rPoint1, _IN Float4& rPoint2) { rResult[0] = (rPoint1[0] + rPoint2[0]) / 2.0f; rResult[1] = (rPoint1[1] + rPoint2[1]) / 2.0f; rResult[2] = (rPoint1[2] + rPoint2[2]) / 2.0f; rResult[3] = (rPoint1[3] + rPoint2[3]) / 2.0f; }  

	inline void AverageVectors2(_OUT Float2& rResult, _IN Float2& rPoint1, _IN Float2& rPoint2, _IN Float2& rPoint3) { rResult[0] = (rPoint1[0] + rPoint2[0] + rPoint3[0]) / 3.0f; rResult[1] = (rPoint1[1] + rPoint2[1] + rPoint3[1]) / 3.0f; }  
	inline void AverageVectors3(_OUT Float3& rResult, _IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float3& rPoint3) { rResult[0] = (rPoint1[0] + rPoint2[0] + rPoint3[0]) / 3.0f; rResult[1] = (rPoint1[1] + rPoint2[1] + rPoint3[1]) / 3.0f; rResult[2] = (rPoint1[2] + rPoint2[2] + rPoint3[2]) / 3.0f; }  
	inline void AverageVectors4(_OUT Float4& rResult, _IN Float4& rPoint1, _IN Float4& rPoint2, _IN Float4& rPoint3) { rResult[0] = (rPoint1[0] + rPoint2[0] + rPoint3[0]) / 3.0f; rResult[1] = (rPoint1[1] + rPoint2[1] + rPoint3[1]) / 3.0f; rResult[2] = (rPoint1[2] + rPoint2[2] + rPoint3[2]) / 3.0f; rResult[3] = (rPoint1[3] + rPoint2[3] + rPoint3[3]) / 3.0f; }  

	//Scale{...}
	inline void ScaleVector2(_OUT Float2& rResult, _IN Float2& rVector, float fScale) { rResult[0] = rVector[0] * fScale; rResult[1] = rVector[1] * fScale; }
	inline void ScaleVector3(_OUT Float3& rResult, _IN Float3& rVector, float fScale) { rResult[0] = rVector[0] * fScale; rResult[1] = rVector[1] * fScale; rResult[2] = rVector[2] * fScale; }
	inline void ScaleVector4(_OUT Float4& rResult, _IN Float4& rVector, float fScale) { rResult[0] = rVector[0] * fScale; rResult[1] = rVector[1] * fScale; rResult[2] = rVector[2] * fScale; rResult[3] = rVector[3] * fScale; }

	inline void DivideVector2(_OUT Float2& rResult, _IN Float2& rVector, float fFactor) { if (!fFactor) { return; } rResult[0] = rVector[0] / fFactor; rResult[1] = rVector[1] / fFactor; }
	inline void DivideVector3(_OUT Float3& rResult, _IN Float3& rVector, float fFactor) { if (!fFactor) { return; } rResult[0] = rVector[0] / fFactor; rResult[1] = rVector[1] / fFactor; rResult[2] = rVector[2] / fFactor; }
	inline void DivideVector4(_OUT Float4& rResult, _IN Float4& rVector, float fFactor) { if (!fFactor) { return; } rResult[0] = rVector[0] / fFactor; rResult[1] = rVector[1] / fFactor; rResult[2] = rVector[2] / fFactor; rResult[3] = rVector[3] / fFactor; }

	//Add{...}
	inline void AddVectors2(_OUT Char2& rResult, _IN Char2& rVector1, _IN Char2& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; }
	inline void AddVectors3(_OUT Char3& rResult, _IN Char3& rVector1, _IN Char3& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; rResult[2] = rVector1[2] + rVector2[2]; }
	inline void AddVectors4(_OUT Char4& rResult, _IN Char4& rVector1, _IN Char4& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; rResult[2] = rVector1[2] + rVector2[2]; rResult[3] = rVector1[3] + rVector2[3]; }

	inline void AddVectors2(_OUT Short2& rResult, _IN Short2& rVector1, _IN Short2& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; }
	inline void AddVectors3(_OUT Short3& rResult, _IN Short3& rVector1, _IN Short3& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; rResult[2] = rVector1[2] + rVector2[2]; }
	inline void AddVectors4(_OUT Short4& rResult, _IN Short4& rVector1, _IN Short4& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; rResult[2] = rVector1[2] + rVector2[2]; rResult[3] = rVector1[3] + rVector2[3]; }

	inline void AddVectors2(_OUT Int2& rResult, _IN Int2& rVector1, _IN Int2& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; }
	inline void AddVectors3(_OUT Int3& rResult, _IN Int3& rVector1, _IN Int3& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; rResult[2] = rVector1[2] + rVector2[2]; }
	inline void AddVectors4(_OUT Int4& rResult, _IN Int4& rVector1, _IN Int4& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; rResult[2] = rVector1[2] + rVector2[2]; rResult[3] = rVector1[3] + rVector2[3]; }

	inline void AddVectors2(_OUT Float2& rResult, _IN Float2& rVector1, _IN Float2& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; }
	inline void AddVectors3(_OUT Float3& rResult, _IN Float3& rVector1, _IN Float3& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; rResult[2] = rVector1[2] + rVector2[2]; }
	inline void AddVectors4(_OUT Float4& rResult, _IN Float4& rVector1, _IN Float4& rVector2) { rResult[0] = rVector1[0] + rVector2[0]; rResult[1] = rVector1[1] + rVector2[1]; rResult[2] = rVector1[2] + rVector2[2]; rResult[3] = rVector1[3] + rVector2[3]; }

	//Subtract{...}
	inline void SubtractVectors2(_OUT Char2& rResult, _IN Char2& rVector1, _IN Char2& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; }
	inline void SubtractVectors3(_OUT Char3& rResult, _IN Char3& rVector1, _IN Char3& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; rResult[2] = rVector1[2] - rVector2[2]; }
	inline void SubtractVectors4(_OUT Char4& rResult, _IN Char4& rVector1, _IN Char4& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; rResult[2] = rVector1[2] - rVector2[2]; rResult[3] = rVector1[3] - rVector2[3]; }

	inline void SubtractVectors2(_OUT Short2& rResult, _IN Short2& rVector1, _IN Short2& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; }
	inline void SubtractVectors3(_OUT Short3& rResult, _IN Short3& rVector1, _IN Short3& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; rResult[2] = rVector1[2] - rVector2[2]; }
	inline void SubtractVectors4(_OUT Short4& rResult, _IN Short4& rVector1, _IN Short4& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; rResult[2] = rVector1[2] - rVector2[2]; rResult[3] = rVector1[3] - rVector2[3]; }

	inline void SubtractVectors2(_OUT Int2& rResult, _IN Int2& rVector1, _IN Int2& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; }
	inline void SubtractVectors3(_OUT Int3& rResult, _IN Int3& rVector1, _IN Int3& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; rResult[2] = rVector1[2] - rVector2[2]; }
	inline void SubtractVectors4(_OUT Int4& rResult, _IN Int4& rVector1, _IN Int4& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; rResult[2] = rVector1[2] - rVector2[2]; rResult[3] = rVector1[3] - rVector2[3]; }

	inline void SubtractVectors2(_OUT Float2& rResult, _IN Float2& rVector1, _IN Float2& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; }
	inline void SubtractVectors3(_OUT Float3& rResult, _IN Float3& rVector1, _IN Float3& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; rResult[2] = rVector1[2] - rVector2[2]; }
	inline void SubtractVectors4(_OUT Float4& rResult, _IN Float4& rVector1, _IN Float4& rVector2) { rResult[0] = rVector1[0] - rVector2[0]; rResult[1] = rVector1[1] - rVector2[1]; rResult[2] = rVector1[2] - rVector2[2]; rResult[3] = rVector1[3] - rVector2[3]; }

	//Multiply{...}
	inline float MultiplyVectors2(_IN Float2& rVector1, _IN Float2& rVector2) { return (rVector1[0] * rVector2[0]) + (rVector1[1] * rVector2[1]); }
	inline float MultiplyVectors3(_IN Float3& rVector1, _IN Float3& rVector2) { return (rVector1[0] * rVector2[0]) + (rVector1[1] * rVector2[1]) + (rVector1[2] * rVector2[2]); }
	inline float MultiplyVectors4(_IN Float4& rVector1, _IN Float4& rVector2) { return (rVector1[0] * rVector2[0]) + (rVector1[1] * rVector2[1]) + (rVector1[2] * rVector2[2]) + (rVector1[3] * rVector2[3]); }

	//Multiply{...}CW
	inline void MultiplyVectors2CW(_OUT Float2& rResult, _IN Float2& rVector1, _IN Float2& rVector2) { rResult[0] = rVector1[0] * rVector2[0]; rResult[1] = rVector1[1] * rVector2[1]; }
	inline void MultiplyVectors3CW(_OUT Float3& rResult, _IN Float3& rVector1, _IN Float3& rVector2) { rResult[0] = rVector1[0] * rVector2[0]; rResult[1] = rVector1[1] * rVector2[1]; rResult[2] = rVector1[2] * rVector2[2]; }
	inline void MultiplyVectors4CW(_OUT Float4& rResult, _IN Float4& rVector1, _IN Float4& rVector2) { rResult[0] = rVector1[0] * rVector2[0]; rResult[1] = rVector1[1] * rVector2[1]; rResult[2] = rVector1[2] * rVector2[2]; rResult[3] = rVector1[3] * rVector2[3]; }

	//MAD{...}CW
	inline void MADVectors2CW(_INOUT Float2& rSummand, _IN Float2& rVector1, _IN Float2& rVector2) { rSummand[0] += rVector1[0] * rVector2[0]; rSummand[1] += rVector1[1] * rVector2[1]; }
	inline void MADVectors3CW(_INOUT Float3& rSummand, _IN Float3& rVector1, _IN Float3& rVector2) { rSummand[0] += rVector1[0] * rVector2[0]; rSummand[1] += rVector1[1] * rVector2[1]; rSummand[2] += rVector1[2] * rVector2[2]; }
	inline void MADVectors4CW(_INOUT Float4& rSummand, _IN Float4& rVector1, _IN Float4& rVector2) { rSummand[0] += rVector1[0] * rVector2[0]; rSummand[1] += rVector1[1] * rVector2[1]; rSummand[2] += rVector1[2] * rVector2[2]; rSummand[3] += rVector1[3] * rVector2[3]; }

	inline float CrossVectors2(_IN Float2& rVector1, _IN Float2& rVector2) { return ((rVector1[0] * rVector2[1]) - (rVector1[1] * rVector2[0])); }
	inline void  CrossVectors3(_OUT Float3& rResult, _IN Float3& rVector1, _IN Float3& rVector2) { rResult[0] = ((rVector1[1] * rVector2[2]) - (rVector1[2] * rVector2[1])); rResult[1] = ((rVector1[2] * rVector2[0]) - (rVector1[0] * rVector2[2])); rResult[2] = ((rVector1[0] * rVector2[1]) - (rVector1[1] * rVector2[0])); }

	inline float MeasureVector2(_IN Float2& rVector) { return sqrtf(rVector[0] * rVector[0] + rVector[1] * rVector[1]); }
	inline float MeasureVector3(_IN Float3& rVector) { return sqrtf(rVector[0] * rVector[0] + rVector[1] * rVector[1] + rVector[2] * rVector[2]); }

	inline void NormalizeVector2(_OUT Float2& rResult, _IN Float2& rVector) { register float Length = MeasureVector2(rVector); if (!Length) { return; } rResult[0] = (float)(rVector[0] / Length); rResult[1] = (float)(rVector[1] / Length); }
	inline void NormalizeVector3(_OUT Float3& rResult, _IN Float3& rVector) { register float Length = MeasureVector3(rVector); if (!Length) { return; } rResult[0] = (float)(rVector[0] / Length); rResult[1] = (float)(rVector[1] / Length); rResult[2] = (float)(rVector[2] / Length); }

	//MeasureAngle{...}
	inline float MeasureAngle2(_IN Float2& rVector1, _IN Float2& rVector2) { register Float2 Vector1N; register Float2 Vector2N; NormalizeVector2(Vector1N, rVector1); NormalizeVector2(Vector2N, rVector2); return (float)(SCFRadToDeg * acosf(MultiplyVectors2(Vector1N, Vector2N))); }
	inline float MeasureAngle3(_IN Float3& rVector1, _IN Float3& rVector2) { register Float3 Vector1N; register Float3 Vector2N; NormalizeVector3(Vector1N, rVector1); NormalizeVector3(Vector2N, rVector2); return (float)(SCFRadToDeg * acosf(MultiplyVectors3(Vector1N, Vector2N))); }

	inline float MeasureAngle2N(_IN Float2& rNormVector1, _IN Float2& rNormVector2) { return (float)(SCFRadToDeg * acosf(MultiplyVectors2(rNormVector1, rNormVector2))); }
	inline float MeasureAngle3N(_IN Float3& rNormVector1, _IN Float3& rNormVector2) { return (float)(SCFRadToDeg * acosf(MultiplyVectors3(rNormVector1, rNormVector2))); }

	//MeasureDistance{...}
	inline float MeasureDistance3      (_IN Float3& rPoint1, _IN Float3& rPoint2) { return sqrtf((rPoint2[0] - rPoint1[0]) * (rPoint2[0] - rPoint1[0]) + (rPoint2[1] - rPoint1[1]) * (rPoint2[1] - rPoint1[1]) + (rPoint2[2] - rPoint1[2]) * (rPoint2[2] - rPoint1[2])); }
	inline float MeasureDistance3Square(_IN Float3& rPoint1, _IN Float3& rPoint2) { return       (rPoint2[0] - rPoint1[0]) * (rPoint2[0] - rPoint1[0]) + (rPoint2[1] - rPoint1[1]) * (rPoint2[1] - rPoint1[1]) + (rPoint2[2] - rPoint1[2]) * (rPoint2[2] - rPoint1[2]); }

	//Projects [rVector] onto [rBase] rVector, N-version assumes [rNormBase] to be normalized
	inline void ProjectVector3 (_OUT Float3& rResult, _IN Float3& rBase, _IN Float3& rVector)		{ ScaleVector3(rResult, rBase,		MultiplyVectors3(rBase,		rVector) / MultiplyVectors3(rBase, rBase)); }
	inline void ProjectVector3N(_OUT Float3& rResult, _IN Float3& rNormBase, _IN Float3& rVector)	{ ScaleVector3(rResult, rNormBase,	MultiplyVectors3(rNormBase,	rVector)); }

	//Rotate{...}
	void __fastcall RotateVector3(_OUT Float3& rResult, _IN Float3& rVector, _IN float fX, _IN float fY, _IN float fZ);
	void __fastcall RotateVector3(_OUT Float3& rResult, _IN Float3& rVector, _IN Float3& rRotation);

	void __fastcall AxisRotateVector3N (_OUT Float3& rResult, _IN Float3& rVector, _IN Float3& rAxis, _IN float fAngle);
	void __fastcall AxisRotationMatrixN(_OUT Matrix3& rResult, _IN Float3& axis, _IN float fAngle); 

	void __fastcall DirectionToRotation3N(_OUT Float3& rResult, _IN Float3& rVector);

	//Lerp{...}
	inline void LerpVector3(_OUT Float3& rResult, _IN Float3& rVector1, _IN Float3& rVector2, _IN float fValue) { rResult[0] = (rVector2[0] - rVector1[0]) * fValue + rVector1[0]; rResult[1] = (rVector2[1] - rVector1[1]) * fValue + rVector1[1]; rResult[2] = (rVector2[2] - rVector1[2]) * fValue + rVector1[2]; } 
	inline void LerpVector4(_OUT Float4& rResult, _IN Float4& rVector1, _IN Float4& rVector2, _IN float fValue) { rResult[0] = (rVector2[0] - rVector1[0]) * fValue + rVector1[0]; rResult[1] = (rVector2[1] - rVector1[1]) * fValue + rVector1[1]; rResult[2] = (rVector2[2] - rVector1[2]) * fValue + rVector1[2]; rResult[3] = (rVector2[3] - rVector1[3]) * fValue + rVector1[3]; } 

	//Plane functions
	inline void MakeNormal(_OUT Float3& rResult, _IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float3& rPoint3) { register Float3 Side1; register Float3 Side2; MakeVector3(Side1, rPoint1, rPoint2); MakeVector3(Side2, rPoint1, rPoint3); CrossVectors3(rResult, Side1, Side2); NormalizeVector3(rResult, rResult); }
	inline void MakePlane (_OUT Float4& rResult, _IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float3& rPoint3) { MakeNormal((Float3&)rResult, rPoint1, rPoint2, rPoint3); rResult[3] = -(rPoint1[0] * rResult[0] + rPoint1[1] * rResult[1] + rPoint1[2] * rResult[2]); }
	inline void MakePlane (_OUT Float4& rResult, _IN Float3& rPoint,  _IN Float3& rNormal)                      { rResult[0] = rNormal[0]; rResult[1] = rNormal[1]; rResult[2] = rNormal[2]; rResult[3] = -(rPoint[0] * rNormal[0] + rPoint[1] * rNormal[1] + rPoint[2] * rNormal[2]); }

	//Determine whether vectors are opposing each other
	inline bool VectorsAreOpposing(_IN Float3& rVector1, _IN Float3& rVector2) { if (MultiplyVectors3(rVector1, rVector2) < 0) { return TRUE; } return FALSE; }

	//Vector/rPlane relation qualification
	inline     float PointPlaneDistance(_IN Float3& rPoint, _IN Float4& rPlane) { return MultiplyVectors3(rPoint, (Float3&)rPlane) + rPlane[3]; }
	float __fastcall PointLineDistance (_IN Float3& rPoint, _IN Float3& rLinePoint, _IN Float3& rLineDirection);


	//Intersection{...}
	bool __fastcall IntersectionVectorPlane(_IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float4& rPlane);
	bool __fastcall IntersectionVectorPlane(_IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float3& PlaneNormal, _IN Float3& PlanePoint);  		

	bool __fastcall IntersectionTriangle(_IN Float3& rVertex1a, _IN Float3& rVertex1b, _IN Float3& rVertex1c, _IN Float3& rVertex2a, _IN Float3& rVertex2b, _IN Float3& rVertex2c);
	bool __fastcall IntersectionTriangle(_IN Float3& rVertex1a, _IN Float3& rVertex1b, _IN Float3& rVertex1c, _IN Float3& rVertex2a, _IN Float3& rVertex2b, _IN Float3& rVertex2c, Float3& rResult1, Float3& rResult2);

	//CrossPoint{...}
	void __fastcall CrossPointLinePlane  (_IN Float3& rPoint,  _IN Float3& rDirection, _IN Float4& rPlane,								Float3& rResult);
	bool __fastcall CrossPointVectorPlane(_IN Float3& rPoint1, _IN Float3& rPoint2,    _IN Float4& rPlane,								Float3& rResult);
	bool __fastcall CrossPointVectorPlane(_IN Float3& rPoint1, _IN Float3& rPoint2,    _IN Float3& PlaneNormal, _IN Float3& PlanePoint,	Float3& rResult);

	//Vector/triangle relation qualification
	bool __fastcall CrossPointVectorTriangle(_IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float3& rVertex1, _IN Float3& rVertex2, _IN Float3& rVertex3,	Float3& rResult);
	

	//Point/triangle relation qualification
	bool __fastcall PointInTriangle(_IN Float3& Vertex1, _IN Float3& Vertex2, _IN Float3& Vertex3, _IN Float3& rPoint);

	       float TriangleArea2(_IN Float2& rVertex1, _IN Float2& rVertex2, _IN Float2& rVertex3);
	inline float TriangleArea3(_IN Float3& rVertex1, _IN Float3& rVertex2, _IN Float3& rVertex3)
	{
		register Float3 Side1;
		register Float3 Side2;

		MakeVector3(Side1, rVertex1, rVertex2);
		MakeVector3(Side2, rVertex1, rVertex3);

		register Float3 Crossed;
		CrossVectors3(Crossed, Side1, Side2);

		return MeasureVector3(Crossed) / 2.0f;
	}

	//Cone/rPlane relation qualification 
	//Returns < 0 if cone is behind plane, returns 0 if cone intersects plane, returns > 0 if cone is in front of plane, [rConePosition] refers to the tip of the cone, [rConeDirection] points from the tip to the base of the cone
	int  __fastcall InfiniteConePlaneRelation (_IN Float3& rConeDirection, _IN Float3& rConePosition, _IN float fConeHalfAngle, _IN Float4& rPlane);
	bool __fastcall InfiniteConeContainsPoint (_IN Float3& rConeDirection, _IN Float3& rConePosition, _IN float fConeHalfAngle, _IN Float3& rPoint);

	//Returns < 0 if sphere is outside the cone, returns 0 if cone intersects sphere, returns > 0 if cone contains sphere
	int  __fastcall InfiniteConeSphereRelation(_IN Float3& rConeDirection, _IN Float3& rConePosition, _IN float fConeHalfAngle, _IN Float3& rPosition, _IN float fRadius);

	//Matrix/quaternion algebra
	
	//Transpose{...}
	inline void TransposeMatrix3(_OUT Matrix3& rDestination, _IN Matrix3& rSource) { for (char i = 0; i < 3; i++) { for (char j = 0; j < 3; j++) { rDestination[i][j] = rSource[j][i]; } } }
	inline void TransposeMatrix4(_OUT Matrix4& rDestination, _IN Matrix4& rSource) { for (char i = 0; i < 4; i++) { for (char j = 0; j < 4; j++) { rDestination[i][j] = rSource[j][i]; } } }

	//Truncate{...}
	inline void TruncateTopLeftToMatrix3    (_OUT Matrix3& rDestination, _IN Matrix4& rSource) { rDestination[0][0] = rSource[0][0]; rDestination[0][1] = rSource[0][1]; rDestination[0][2] = rSource[0][2]; rDestination[1][0] = rSource[1][0]; rDestination[1][1] = rSource[1][1]; rDestination[1][2] = rSource[1][2]; rDestination[2][0] = rSource[2][0]; rDestination[2][1] = rSource[2][1]; rDestination[2][2] = rSource[2][2]; }
	inline void TruncateTopRightToMatrix3   (_OUT Matrix3& rDestination, _IN Matrix4& rSource) { rDestination[0][0] = rSource[1][0]; rDestination[0][1] = rSource[1][1]; rDestination[0][2] = rSource[1][2]; rDestination[1][0] = rSource[2][0]; rDestination[1][1] = rSource[2][1]; rDestination[1][2] = rSource[2][2]; rDestination[2][0] = rSource[3][0]; rDestination[2][1] = rSource[3][1]; rDestination[2][2] = rSource[3][2]; }
	inline void TruncateBottomLeftToMatrix3 (_OUT Matrix3& rDestination, _IN Matrix4& rSource) { rDestination[0][0] = rSource[0][1]; rDestination[0][1] = rSource[0][2]; rDestination[0][2] = rSource[0][3]; rDestination[1][0] = rSource[1][1]; rDestination[1][1] = rSource[1][2]; rDestination[1][2] = rSource[1][3]; rDestination[2][0] = rSource[2][1]; rDestination[2][1] = rSource[2][2]; rDestination[2][2] = rSource[2][3]; }
	inline void TruncateBottomRightToMatrix3(_OUT Matrix3& rDestination, _IN Matrix4& rSource) { rDestination[0][0] = rSource[1][1]; rDestination[0][1] = rSource[1][2]; rDestination[0][2] = rSource[1][3]; rDestination[1][0] = rSource[2][1]; rDestination[1][1] = rSource[2][2]; rDestination[1][2] = rSource[2][3]; rDestination[2][0] = rSource[3][1]; rDestination[2][1] = rSource[3][2]; rDestination[2][2] = rSource[3][3]; }

	inline void CopyMatrix3(_OUT Matrix3& rDestination, _IN Matrix3& rSource) { for (char i = 0; i < 9;  i++) { ((double*)rDestination)[i] = ((double*)rSource)[i]; } }
	inline void CopyMatrix4(_OUT Matrix4& rDestination, _IN Matrix4& rSource) { for (char i = 0; i < 16; i++) { ((double*)rDestination)[i] = ((double*)rSource)[i]; } }

	inline void MakeIndentityMatrix3(_OUT Matrix3& rMatrix) { rMatrix[0][0] = 1; rMatrix[0][1] = 0; rMatrix[0][2] = 0; rMatrix[1][0] = 0; rMatrix[1][1] = 1; rMatrix[1][2] = 0; rMatrix[2][0] = 0; rMatrix[2][1] = 0; rMatrix[2][2] = 1; }  
	inline void MakeIndentityMatrix4(_OUT Matrix4& rMatrix) { rMatrix[0][0] = 1; rMatrix[0][1] = 0; rMatrix[0][2] = 0; rMatrix[0][3] = 0; rMatrix[1][0] = 0; rMatrix[1][1] = 1; rMatrix[1][2] = 0; rMatrix[1][3] = 0; rMatrix[2][0] = 0; rMatrix[2][1] = 0; rMatrix[2][2] = 1; rMatrix[2][3] = 0; rMatrix[3][0] = 0; rMatrix[3][1] = 0; rMatrix[3][2] = 0; rMatrix[3][3] = 1; }

	inline void ZeroMatrix3(_OUT Matrix3& rMatrix) { rMatrix[0][0] = 0; rMatrix[0][1] = 0; rMatrix[0][2] = 0; rMatrix[1][0] = 0; rMatrix[1][1] = 0; rMatrix[1][2] = 0; rMatrix[2][0] = 0; rMatrix[2][1] = 0; rMatrix[2][2] = 0; }	
	inline void ZeroMatrix4(_OUT Matrix4& rMatrix) { rMatrix[0][0] = 0; rMatrix[0][1] = 0; rMatrix[0][2] = 0; rMatrix[0][3] = 0; rMatrix[1][0] = 0; rMatrix[1][1] = 0; rMatrix[1][2] = 0; rMatrix[1][3] = 0; rMatrix[2][0] = 0; rMatrix[2][1] = 0; rMatrix[2][2] = 0; rMatrix[2][3] = 0; rMatrix[3][0] = 0; rMatrix[3][1] = 0; rMatrix[3][2] = 0; rMatrix[3][3] = 0; }	

	void __fastcall ComposeMatrix3(_OUT Matrix3& rResult, _IN Float3& rVector1, _IN Float3& rVector2);
	void __fastcall ComposeMatrix4(_OUT Matrix4& rResult, _IN Float4& rVector1, _IN Float4& rVector2);

	inline void ScaleMatrix3(_OUT Matrix3& rResult, _IN Matrix3& rMatrix, _IN float fScale)
	{
		rResult[0][0] = rMatrix[0][0] * fScale;
		rResult[0][1] = rMatrix[0][1] * fScale;
		rResult[0][2] = rMatrix[0][2] * fScale;
		rResult[1][0] = rMatrix[1][0] * fScale;
		rResult[1][1] = rMatrix[1][1] * fScale;
		rResult[1][2] = rMatrix[1][2] * fScale;
		rResult[2][0] = rMatrix[2][0] * fScale;
		rResult[2][1] = rMatrix[2][1] * fScale;
		rResult[2][2] = rMatrix[2][2] * fScale;
	}
	inline void ScaleMatrix4(_OUT Matrix4& rResult, _IN Matrix4& rMatrix, _IN float fScale)
	{
		rResult[0][0] = rMatrix[0][0] * fScale;
		rResult[0][1] = rMatrix[0][1] * fScale;
		rResult[0][2] = rMatrix[0][2] * fScale;
		rResult[0][3] = rMatrix[0][3] * fScale;
		rResult[1][0] = rMatrix[1][0] * fScale;
		rResult[1][1] = rMatrix[1][1] * fScale;
		rResult[1][2] = rMatrix[1][2] * fScale;
		rResult[1][3] = rMatrix[1][3] * fScale;
		rResult[2][0] = rMatrix[2][0] * fScale;
		rResult[2][1] = rMatrix[2][1] * fScale;
		rResult[2][2] = rMatrix[2][2] * fScale;
		rResult[2][3] = rMatrix[2][3] * fScale;
		rResult[3][0] = rMatrix[3][0] * fScale;
		rResult[3][1] = rMatrix[3][1] * fScale;
		rResult[3][2] = rMatrix[3][2] * fScale;
		rResult[3][3] = rMatrix[3][3] * fScale;
	}

	inline void AddMatrices3(_OUT Matrix3& rResult, _IN Matrix3& rMatrix1, _IN Matrix3& rMatrix2)
	{
		rResult[0][0] = rMatrix1[0][0] + (float)rMatrix2[0][0];
		rResult[0][1] = rMatrix1[0][1] + (float)rMatrix2[0][1];
		rResult[0][2] = rMatrix1[0][2] + (float)rMatrix2[0][2];
		rResult[1][0] = rMatrix1[1][0] + (float)rMatrix2[1][0];
		rResult[1][1] = rMatrix1[1][1] + (float)rMatrix2[1][1];
		rResult[1][2] = rMatrix1[1][2] + (float)rMatrix2[1][2];
		rResult[2][0] = rMatrix1[2][0] + (float)rMatrix2[2][0];
		rResult[2][1] = rMatrix1[2][1] + (float)rMatrix2[2][1];
		rResult[2][2] = rMatrix1[2][2] + (float)rMatrix2[2][2];
	}
	inline void AddMatrices4(_OUT Matrix4& rResult, _IN Matrix4& rMatrix1, _IN Matrix4& rMatrix2)
	{
		rResult[0][0] = rMatrix1[0][0] + (float)rMatrix2[0][0];
		rResult[0][1] = rMatrix1[0][1] + (float)rMatrix2[0][1];
		rResult[0][2] = rMatrix1[0][2] + (float)rMatrix2[0][2];
		rResult[0][3] = rMatrix1[0][3] + (float)rMatrix2[0][3];
		rResult[1][0] = rMatrix1[1][0] + (float)rMatrix2[1][0];
		rResult[1][1] = rMatrix1[1][1] + (float)rMatrix2[1][1];
		rResult[1][2] = rMatrix1[1][2] + (float)rMatrix2[1][2];
		rResult[1][3] = rMatrix1[1][3] + (float)rMatrix2[1][3];
		rResult[2][0] = rMatrix1[2][0] + (float)rMatrix2[2][0];
		rResult[2][1] = rMatrix1[2][1] + (float)rMatrix2[2][1];
		rResult[2][2] = rMatrix1[2][2] + (float)rMatrix2[2][2];
		rResult[2][3] = rMatrix1[2][3] + (float)rMatrix2[2][3];
		rResult[3][0] = rMatrix1[3][0] + (float)rMatrix2[3][0];
		rResult[3][1] = rMatrix1[3][1] + (float)rMatrix2[3][1];
		rResult[3][2] = rMatrix1[3][2] + (float)rMatrix2[3][2];
		rResult[3][3] = rMatrix1[3][3] + (float)rMatrix2[3][3];
	}

	inline void SubtractMatrices3(_OUT Matrix3& rResult, _IN Matrix3& rMatrix1, _IN Matrix3& rMatrix2)
	{
		rResult[0][0] = rMatrix1[0][0] - rMatrix2[0][0];
		rResult[0][1] = rMatrix1[0][1] - rMatrix2[0][1];
		rResult[0][2] = rMatrix1[0][2] - rMatrix2[0][2];
		rResult[1][0] = rMatrix1[1][0] - rMatrix2[1][0];
		rResult[1][1] = rMatrix1[1][1] - rMatrix2[1][1];
		rResult[1][2] = rMatrix1[1][2] - rMatrix2[1][2];
		rResult[2][0] = rMatrix1[2][0] - rMatrix2[2][0];
		rResult[2][1] = rMatrix1[2][1] - rMatrix2[2][1];
		rResult[2][2] = rMatrix1[2][2] - rMatrix2[2][2];
	}
	inline void SubtractMatrices4(_OUT Matrix4& rResult, _IN Matrix4& rMatrix1, _IN Matrix4& rMatrix2)
	{
		rResult[0][0] = rMatrix1[0][0] - rMatrix2[0][0];
		rResult[0][1] = rMatrix1[0][1] - rMatrix2[0][1];
		rResult[0][2] = rMatrix1[0][2] - rMatrix2[0][2];
		rResult[0][3] = rMatrix1[0][3] - rMatrix2[0][3];
		rResult[1][0] = rMatrix1[1][0] - rMatrix2[1][0];
		rResult[1][1] = rMatrix1[1][1] - rMatrix2[1][1];
		rResult[1][2] = rMatrix1[1][2] - rMatrix2[1][2];
		rResult[1][3] = rMatrix1[1][3] - rMatrix2[1][3];
		rResult[2][0] = rMatrix1[2][0] - rMatrix2[2][0];
		rResult[2][1] = rMatrix1[2][1] - rMatrix2[2][1];
		rResult[2][2] = rMatrix1[2][2] - rMatrix2[2][2];
		rResult[2][3] = rMatrix1[2][3] - rMatrix2[2][3];
		rResult[3][0] = rMatrix1[3][0] - rMatrix2[3][0];
		rResult[3][1] = rMatrix1[3][1] - rMatrix2[3][1];
		rResult[3][2] = rMatrix1[3][2] - rMatrix2[3][2];
		rResult[3][3] = rMatrix1[3][3] - rMatrix2[3][3];
	}

	inline void MultiplyMatrix3Vector3(_OUT Float3& rResult, _IN Matrix3& rMatrix, _IN Float3& rVector) 
	{ 
		rResult[0] = (float)rMatrix[0][0] * rVector[0] + (float)rMatrix[1][0] * rVector[1] + (float)rMatrix[2][0] * rVector[2]; 
		rResult[1] = (float)rMatrix[0][1] * rVector[0] + (float)rMatrix[1][1] * rVector[1] + (float)rMatrix[2][1] * rVector[2]; 
		rResult[2] = (float)rMatrix[0][2] * rVector[0] + (float)rMatrix[1][2] * rVector[1] + (float)rMatrix[2][2] * rVector[2]; 
	}
	inline void MultiplyMatrix4Vector3(_OUT Float3& rResult, _IN Matrix4& rMatrix, _IN Float3& rVector) 
	{ 
		rResult[0] = (float)rMatrix[0][0] * rVector[0] + (float)rMatrix[1][0] * rVector[1] + (float)rMatrix[2][0] * rVector[2] + (float)rMatrix[3][0]; 
		rResult[1] = (float)rMatrix[0][1] * rVector[0] + (float)rMatrix[1][1] * rVector[1] + (float)rMatrix[2][1] * rVector[2] + (float)rMatrix[3][1]; 
		rResult[2] = (float)rMatrix[0][2] * rVector[0] + (float)rMatrix[1][2] * rVector[1] + (float)rMatrix[2][2] * rVector[2] + (float)rMatrix[3][2]; 
	}
	inline void MultiplyMatrix4Vector4(_OUT Float4& rResult, _IN Matrix4& rMatrix, _IN Float4& rVector) 
	{ 
		rResult[0] = (float)rMatrix[0][0] * rVector[0] + (float)rMatrix[1][0] * rVector[1] + (float)rMatrix[2][0] * rVector[2] + (float)rMatrix[3][0] * rVector[3]; 
		rResult[1] = (float)rMatrix[0][1] * rVector[0] + (float)rMatrix[1][1] * rVector[1] + (float)rMatrix[2][1] * rVector[2] + (float)rMatrix[3][1] * rVector[3]; 
		rResult[2] = (float)rMatrix[0][2] * rVector[0] + (float)rMatrix[1][2] * rVector[1] + (float)rMatrix[2][2] * rVector[2] + (float)rMatrix[3][2] * rVector[3]; 
		rResult[3] = (float)rMatrix[0][3] * rVector[0] + (float)rMatrix[1][3] * rVector[1] + (float)rMatrix[2][3] * rVector[2] + (float)rMatrix[3][3] * rVector[3]; 
	}
	inline void MultiplyMatrixT3Vector3(_OUT Float3& rResult, _IN Matrix3& rMatrix, _IN Float3& rVector) 
	{ 
		rResult[0] = (float)rMatrix[0][0] * rVector[0] + (float)rMatrix[0][1] * rVector[1] + (float)rMatrix[0][2] * rVector[2]; 
		rResult[1] = (float)rMatrix[1][0] * rVector[0] + (float)rMatrix[1][1] * rVector[1] + (float)rMatrix[1][2] * rVector[2]; 
		rResult[2] = (float)rMatrix[2][0] * rVector[0] + (float)rMatrix[2][1] * rVector[1] + (float)rMatrix[2][2] * rVector[2]; 
	}
	inline void MultiplyMatrixT4Vector3(_OUT Float3& rResult, _IN Matrix4& rMatrix, _IN Float3& rVector) 
	{ 
		rResult[0] = (float)rMatrix[0][0] * rVector[0] + (float)rMatrix[0][1] * rVector[1] + (float)rMatrix[0][2] * rVector[2] + (float)rMatrix[0][3]; 
		rResult[1] = (float)rMatrix[1][0] * rVector[0] + (float)rMatrix[1][1] * rVector[1] + (float)rMatrix[1][2] * rVector[2] + (float)rMatrix[1][3]; 
		rResult[2] = (float)rMatrix[2][0] * rVector[0] + (float)rMatrix[2][1] * rVector[1] + (float)rMatrix[2][2] * rVector[2] + (float)rMatrix[2][3]; 
	}
	inline void MultiplyMatrixT4Vector4(_OUT Float4& rResult, _IN Matrix4& rMatrix, _IN Float4& rVector) 
	{ 
		rResult[0] = (float)rMatrix[0][0] * rVector[0] + (float)rMatrix[0][1] * rVector[1] + (float)rMatrix[0][2] * rVector[2] + (float)rMatrix[0][3] * rVector[3]; 
		rResult[1] = (float)rMatrix[1][0] * rVector[0] + (float)rMatrix[1][1] * rVector[1] + (float)rMatrix[1][2] * rVector[2] + (float)rMatrix[1][3] * rVector[3]; 
		rResult[2] = (float)rMatrix[2][0] * rVector[0] + (float)rMatrix[2][1] * rVector[1] + (float)rMatrix[2][2] * rVector[2] + (float)rMatrix[2][3] * rVector[3]; 
		rResult[3] = (float)rMatrix[3][0] * rVector[0] + (float)rMatrix[3][1] * rVector[1] + (float)rMatrix[3][2] * rVector[2] + (float)rMatrix[3][3] * rVector[3]; 
	}

	void __fastcall MultiplyVector3Matrix3(Matrix3& rResult, Float3& rVector, Matrix3& rMatrix);
	void __fastcall MultiplyVector4Matrix4(Matrix4& rResult, Float4& rVector, Matrix4& rMatrix);

	void __fastcall MultiplyMatrices3(Matrix3& rResult, Matrix3& rMatrix1, Matrix3& rMatrix2);
	void __fastcall MultiplyMatrices4(Matrix4& rResult, Matrix4& rMatrix1, Matrix4& rMatrix2);

	void __fastcall MultiplyVectorT4Matrix4(Float4& rResult, Float4& rVector, Matrix4& rMatrix);
	void __fastcall MultiplyVectorT3Matrix3(Float3& rResult, Float3& rVector, Matrix3& rMatrix);

	void __fastcall MultiplyMatrix4Matrix3(Matrix4& rResult, Matrix4& rMatrix1, Matrix3& rMatrix2);
	void __fastcall MultiplyMatrix3Matrix4(Matrix4& rResult, Matrix3& rMatrix1, Matrix4& rMatrix2);

	//Misc functions
	inline int   RandomPercent()   { return   rand() % 100; }
	inline float RandomFloat0To1() { return  (rand() % 100) * 0.01f; }
	inline float RandomFloat1To1() { return ((rand() % 200) - 100) * 0.01f; }
};
#include <SCFMathematics.h>
#include <xmmintrin.h>

using namespace SCFMathematics;

static Float3 DirX = { 1, 0, 0 };
static Float3 DirY = { 0, 1, 0 };
static Float3 DirZ = { 0, 0, 1 };

//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////

void __fastcall SCFMathematics::AxisRotateVector3N(_OUT Float3& rResult, _IN Float3& rVector, _IN Float3& axis, _IN float fAngle) 
{
	Matrix3 RotationMatrix;

	AxisRotationMatrixN(RotationMatrix, axis, fAngle);
	MultiplyMatrix3Vector3(rResult, RotationMatrix, rVector);
}

void __fastcall SCFMathematics::AxisRotationMatrixN(_OUT Matrix3& rResult, _IN Float3& axis, _IN float fAngle) 
{
	/*
	| xx(1-c)+c   xy(1-c)-zs  xz(1-c)+ys  0 |
	| yx(1-c)+zs  yy(1-c)+c   yz(1-c)-xs  0 |
	| xz(1-c)-ys  yz(1-c)+xs  zz(1-c)+c   0 |
	| 0	          0           0           1 |
	*/
	register float Cos = cosf(fAngle * SCFDegToRad);
	register float Sin = sinf(fAngle * SCFDegToRad);
	register float OneSubCos = 1 - Cos;

	rResult[0][0] = __sqr(axis[0])    * OneSubCos + Cos;
	rResult[0][1] = axis[0] * axis[1] * OneSubCos + axis[2] * Sin;
	rResult[0][2] = axis[0] * axis[2] * OneSubCos - axis[1] * Sin;

	rResult[1][0] = axis[1] * axis[0] * OneSubCos - axis[2] * Sin;
	rResult[1][1] = __sqr(axis[1])    * OneSubCos + Cos;
	rResult[1][2] = axis[1] * axis[2] * OneSubCos + axis[0] * Sin;

	rResult[2][0] = axis[2] * axis[0] * OneSubCos + axis[1] * Sin;
	rResult[2][1] = axis[2] * axis[1] * OneSubCos - axis[0] * Sin;
	rResult[2][2] = __sqr(axis[2])    * OneSubCos + Cos;
}

void __fastcall SCFMathematics::DirectionToRotation3N(_OUT Float3& rResult, _IN Float3& rVector)
{
	//Project vector onto x,y plane
	register Float3 Projected;
	CopyVector3(Projected, rVector);
	Projected[1] = 0;

	//Re-normalize, so that we can use the [MeasureAngle3N(..)] function,
	//which works only with normalized vectors
	NormalizeVector3(Projected, Projected);

	//Measure angle between the z vector & the projected vector,
	//which is the rotation about the Y axis
	rResult[1] = MeasureAngle3N(DirZ, Projected);

	//Since [MeasureAngle3N(..)] uses [acosf(..)] we loose the sign information 
	//about the angle, so restore it based on a set of conditions
	//if ((rVector[0] >= 0) && (rVector[2] <  0)) { rResult[1] =   rResult[1]; }
	//if ((rVector[0] <  0) && (rVector[2] <  0)) { rResult[1] = - rResult[1]; }
	//if ((rVector[0] >= 0) && (rVector[2] >= 0)) { rResult[1] =   rResult[1]; }
	//if ((rVector[0] <  0) && (rVector[2] >= 0)) { rResult[1] = - rResult[1]; }

	//Conditions can be collapsed into one :D
	if (rVector[0] < 0) { rResult[1] = - rResult[1]; }

	//Rotate the vector in the opposite direction,
	//this produces a vector with 0 rotation about the Y axis
	register Float3 PartiallyRotated;
	RotateVector3(PartiallyRotated, rVector, 0, -rResult[1], 0);

	//Measure angle between the z vector & the rotated vector,
	//which is the rotation about the X axis
	rResult[0] = MeasureAngle3N(DirZ, PartiallyRotated);
 
	//if ((rVector[1] >= 0) && (rVector[2] <  0)) { rResult[0] = - rResult[0]; return; }
	//if ((rVector[1] <  0) && (rVector[2] <  0)) { rResult[0] =   rResult[0]; return; }
	//if ((rVector[1] >= 0) && (rVector[2] >= 0)) { rResult[0] = - rResult[0]; return; }
	//if ((rVector[1] <  0) && (rVector[2] >= 0)) { rResult[0] =   rResult[0]; return; }

	//Conditions can be collapsed into one :D
	if (rVector[1] >= 0) { rResult[0] = - rResult[0]; }
}

void __fastcall SCFMathematics::RotateVector3(_OUT Float3& rResult, _IN Float3& rVector, _IN float fX, _IN float fY, _IN float fZ) 
{
	register float XInRad = fX * SCFDegToRad; 
	register float YInRad = fY * SCFDegToRad; 
	register float ZInRad = fZ * SCFDegToRad; 

	register Float3 XRotated;

	if (XInRad != 0)
	{
		XRotated[0] = rVector[0];
		XRotated[1] = (float)(rVector[1] * cosf(XInRad) - rVector[2] * sinf(XInRad));		
		XRotated[2] = (float)(rVector[1] * sinf(XInRad) + rVector[2] * cosf(XInRad));
	}
	else
	{
		XRotated[0] = rVector[0];
		XRotated[1] = rVector[1];
		XRotated[2] = rVector[2];
	}

	register Float3 YRotated;

	if (YInRad != 0)
	{
		YRotated[0] = (float)(XRotated[0]  * cosf(YInRad) + XRotated[2] * sinf(YInRad));		
		YRotated[1] = XRotated[1];
		YRotated[2] = (float)(-XRotated[0] * sinf(YInRad) + XRotated[2] * cosf(YInRad));
	}
	else
	{
		YRotated[0] = XRotated[0];
		YRotated[1] = XRotated[1];
		YRotated[2] = XRotated[2];
	}

	if (ZInRad != 0)
	{
		rResult[0] = (float)(YRotated[0] * cosf(ZInRad) - YRotated[1] * sinf(ZInRad));
		rResult[1] = (float)(YRotated[0] * sinf(ZInRad) + YRotated[1] * cosf(ZInRad));		
		rResult[2] = YRotated[2];
	}
	else
	{
		rResult[0] = YRotated[0];
		rResult[1] = YRotated[1];
		rResult[2] = YRotated[2];
	}
}

void __fastcall SCFMathematics::RotateVector3(_OUT Float3& rResult, _IN Float3& rVector, _IN Float3& rRotation) 
{
	register float XInRad = rRotation[0] * SCFDegToRad; 
	register float YInRad = rRotation[1] * SCFDegToRad; 
	register float ZInRad = rRotation[2] * SCFDegToRad; 

	register Float3 XRotated;

	if (XInRad != 0)
	{
		XRotated[0] = rVector[0];
		XRotated[1] = (float)(rVector[1] * cosf(XInRad) - rVector[2] * sinf(XInRad));		
		XRotated[2] = (float)(rVector[1] * sinf(XInRad) + rVector[2] * cosf(XInRad));
	}
	else
	{
		XRotated[0] = rVector[0];
		XRotated[1] = rVector[1];
		XRotated[2] = rVector[2];
	}

	register Float3 YRotated;

	if (YInRad != 0)
	{
		YRotated[0] = (float)(XRotated[0]  * cosf(YInRad) + XRotated[2] * sinf(YInRad));		
		YRotated[1] = XRotated[1];
		YRotated[2] = (float)(-XRotated[0] * sinf(YInRad) + XRotated[2] * cosf(YInRad));
	}
	else
	{
		YRotated[0] = XRotated[0];
		YRotated[1] = XRotated[1];
		YRotated[2] = XRotated[2];
	}

	if (ZInRad != 0)
	{
		rResult[0] = (float)(YRotated[0] * cosf(ZInRad) - YRotated[1] * sinf(ZInRad));
		rResult[1] = (float)(YRotated[0] * sinf(ZInRad) + YRotated[1] * cosf(ZInRad));		
		rResult[2] = YRotated[2];
	}
	else
	{
		rResult[0] = YRotated[0];
		rResult[1] = YRotated[1];
		rResult[2] = YRotated[2];
	}
}

bool __fastcall SCFMathematics::PointInTriangle(_IN Float3& rVertex1, _IN Float3& rVertex2, _IN Float3& rVertex3, _IN Float3& rPoint)
{
	//Will hold the total angle 
	register float TotalAngle = 0;

	register Float3 LineToV1;
	register Float3 LineToV2;
	register Float3 LineToV3;

	MakeVector3(LineToV1, rPoint, rVertex1);
	MakeVector3(LineToV2, rPoint, rVertex2);
	MakeVector3(LineToV3, rPoint, rVertex3);
	
	TotalAngle += MeasureAngle3(LineToV1, LineToV2);  
	TotalAngle += MeasureAngle3(LineToV2, LineToV3);  
	TotalAngle += MeasureAngle3(LineToV3, LineToV1);  

	if (TotalAngle < 359.9999) { return FALSE; }
					
	return TRUE;	
}

bool __fastcall SCFMathematics::IntersectionVectorPlane(_IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float3& planeNormal, _IN Float3& planePoint)
{
	//Will hold the distance of the plane from origin
	register float DistFromOrigin = - MultiplyVectors3(planeNormal, planePoint);

	//Calculate point to plane distances (dist. = A.fX + B.fY + C.fZ + D) => dist. = normal.rPoint + rPlane-to-origin  
	register float Distance1 = MultiplyVectors3(planeNormal, rPoint1) + DistFromOrigin;
	register float Distance2 = MultiplyVectors3(planeNormal, rPoint2) + DistFromOrigin;

	//If distances differ in signs, points lie on opposite sides of the rPlane
	if (Distance1 * Distance2 >= 0) { return FALSE; }	
					
	return TRUE;	
}

bool __fastcall SCFMathematics::IntersectionVectorPlane(_IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float4& rPlane)
{
	//Calculate point to plane distances (dist. = A.fX + B.fY + C.fZ + D) => dist. = Normal.rPoint + PlaneToOrigin  
	register float Distance1 = rPlane[0] * rPoint1[0] + rPlane[1] * rPoint1[1] + rPlane[2] * rPoint1[2] + rPlane[3];
	register float Distance2 = rPlane[0] * rPoint2[0] + rPlane[1] * rPoint2[1] + rPlane[2] * rPoint2[2] + rPlane[3];

	//If Distances differ in signs, points lie on opposite sides of the plane
	if (Distance1 * Distance2 >= 0) { return FALSE; }	
					
	return TRUE;	
}

bool __fastcall SCFMathematics::CrossPointVectorPlane(_IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float3& planeNormal, _IN Float3& planePoint, Float3& rResult)
{
	if (IntersectionVectorPlane(rPoint1, rPoint2, planeNormal, planePoint))
	{
		//Make line direction
		register Float3 Direction;
		MakeVector3(Direction, rPoint1, rPoint2); 

		//Will hold the distance of the plane from origin
		register float DistFromOrigin = - MultiplyVectors3(planeNormal, planePoint);

		//Will hold line factor
		register float LineFactor = -((MultiplyVectors3(planeNormal, rPoint1) + DistFromOrigin) / MultiplyVectors3(planeNormal, Direction)); 

		//Scale direction to get the proper partial vector of our point
		ScaleVector3(Direction, Direction, LineFactor); 
	
		//Add the two partial vectors 
		AddVectors3(rResult, rPoint1, Direction);
		
		return TRUE;
	}

	return FALSE;
}

bool __fastcall SCFMathematics::CrossPointVectorPlane(_IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float4& rPlane, Float3& rResult)
{
	if (IntersectionVectorPlane(rPoint1, rPoint2, rPlane))
	{
		//Make line direction
		register Float3 Direction;
		MakeVector3(Direction, rPoint1, rPoint2); 

		//Will hold line factor, coefficient, ...	
		register float LineFactor = -((rPlane[0] * rPoint1[0] + rPlane[1] * rPoint1[1] + rPlane[2] * rPoint1[2] + rPlane[3]) / (rPlane[0] * Direction[0] + rPlane[1] * Direction[1] + rPlane[2] * Direction[2])); 

		//Scale direction to get the proper partial vector of our point
		ScaleVector3(Direction, Direction, LineFactor); 
	
		//Add the two partial vectors 
		AddVectors3(rResult, rPoint1, Direction);
		
		return TRUE;
	}

	return FALSE;
}

void __fastcall SCFMathematics::CrossPointLinePlane(_IN Float3& rPoint, _IN Float3& rDirection, _IN Float4& rPlane, Float3& rResult)
{
	//Will hold line factor, coefficient, ...	
	register float LineFactor = -((rPlane[0] * rPoint[0] + rPlane[1] * rPoint[1] + rPlane[2] * rPoint[2] + rPlane[3]) / (rPlane[0] * rDirection[0] + rPlane[1] * rDirection[1] + rPlane[2] * rDirection[2])); 

	//Scale direction to get the proper partial vector of our point
	register Float3 PartialLine;
	ScaleVector3(PartialLine, rDirection, LineFactor); 

	//Add the two partial vectors 
	AddVectors3(rResult, rPoint, PartialLine);
}

float __fastcall SCFMathematics::PointLineDistance(_IN Float3& rPoint, _IN Float3& rLinePoint, _IN Float3& rLineDirection)
{
	register Float4 Plane;
	MakePlane(Plane, rPoint, rLineDirection);

	register Float3 CrossPoint;
	CrossPointLinePlane(rLinePoint, rLineDirection, Plane, CrossPoint);

	return MeasureDistance3(rPoint, CrossPoint);
}
bool __fastcall SCFMathematics::CrossPointVectorTriangle(_IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float3& rVertex1, _IN Float3& rVertex2, _IN Float3& rVertex3, Float3& rResult)
{
	register Float4 Plane;
	MakePlane(Plane, rVertex1, rVertex2, rVertex3);

	if (CrossPointVectorPlane(rPoint1, rPoint2, Plane, rResult))
	{
		if (PointInTriangle(rVertex1, rVertex2, rVertex3, rResult))
		{
			return TRUE; 
		}
	}

	return FALSE;
}

bool __fastcall SCFMathematics::CrossPointVectorTriangleExists(_IN Float3& rPoint1, _IN Float3& rPoint2, _IN Float3& rVertex1, _IN Float3& rVertex2, _IN Float3& rVertex3)
{
	Float3 intersection;
	return CrossPointVectorTriangle(rPoint1, rPoint2, rVertex1, rVertex2, rVertex3, intersection);
}

bool __fastcall SCFMathematics::IntersectionTriangle(_IN Float3& rVertex1a, _IN Float3& rVertex1b, _IN Float3& rVertex1c, _IN Float3& rVertex2a, _IN Float3& rVertex2b, _IN Float3& rVertex2c, Float3& rResult1, Float3& rResult2)
{
	register int Found = 0;

	if (CrossPointVectorTriangle(rVertex2a, rVertex2b, rVertex1a, rVertex1b, rVertex1c, rResult1)) { Found++; }

	if (Found == 0) { if (CrossPointVectorTriangle(rVertex2b, rVertex2c, rVertex1a, rVertex1b, rVertex1c, rResult1)) { Found++; } }
	else            { if (CrossPointVectorTriangle(rVertex2b, rVertex2c, rVertex1a, rVertex1b, rVertex1c, rResult2)) { return TRUE; } }

	if (Found == 0) { if (CrossPointVectorTriangle(rVertex2c, rVertex2a, rVertex1a, rVertex1b, rVertex1c, rResult1)) { Found++; } }
	else            { if (CrossPointVectorTriangle(rVertex2c, rVertex2a, rVertex1a, rVertex1b, rVertex1c, rResult2)) { return TRUE; } }

	if (Found == 0) { if (CrossPointVectorTriangle(rVertex1a, rVertex1b, rVertex2a, rVertex2b, rVertex2c, rResult1)) { Found++; } }
	else            { if (CrossPointVectorTriangle(rVertex1a, rVertex1b, rVertex2a, rVertex2b, rVertex2c, rResult2)) { return TRUE; } }

	if (Found == 0) { if (CrossPointVectorTriangle(rVertex1b, rVertex1c, rVertex2a, rVertex2b, rVertex2c, rResult1)) { Found++; } }
	else            { if (CrossPointVectorTriangle(rVertex1b, rVertex1c, rVertex2a, rVertex2b, rVertex2c, rResult2)) { return TRUE; } }

	if (Found == 0) { if (CrossPointVectorTriangle(rVertex1c, rVertex1a, rVertex2a, rVertex2b, rVertex2c, rResult1)) { Found++; } }
	else            { if (CrossPointVectorTriangle(rVertex1c, rVertex1a, rVertex2a, rVertex2b, rVertex2c, rResult2)) { return TRUE; } }

	return FALSE;
}

bool __fastcall SCFMathematics::IntersectionTriangle(_IN Float3& rVertex1a, _IN Float3& rVertex1b, _IN Float3& rVertex1c, _IN Float3& rVertex2a, _IN Float3& rVertex2b, _IN Float3& rVertex2c)
{
	register Float4 Plane1;
	register Float4 Plane2;

	MakePlane(Plane1, rVertex1a, rVertex1b, rVertex1c);
	MakePlane(Plane2, rVertex2a, rVertex2b, rVertex2c);

	register Float3 IntersectionPoint;

	if (CrossPointVectorPlane(rVertex2a, rVertex2b, Plane1, IntersectionPoint)) { if (PointInTriangle(rVertex1a, rVertex1b, rVertex1c, IntersectionPoint)) { return TRUE; } }
	if (CrossPointVectorPlane(rVertex2b, rVertex2c, Plane1, IntersectionPoint)) { if (PointInTriangle(rVertex1a, rVertex1b, rVertex1c, IntersectionPoint)) { return TRUE; } }
	if (CrossPointVectorPlane(rVertex2c, rVertex2a, Plane1, IntersectionPoint)) { if (PointInTriangle(rVertex1a, rVertex1b, rVertex1c, IntersectionPoint)) { return TRUE; } }

	if (CrossPointVectorPlane(rVertex1a, rVertex1b, Plane2, IntersectionPoint)) { if (PointInTriangle(rVertex2a, rVertex2b, rVertex2c, IntersectionPoint)) { return TRUE; } }
	if (CrossPointVectorPlane(rVertex1b, rVertex1c, Plane2, IntersectionPoint)) { if (PointInTriangle(rVertex2a, rVertex2b, rVertex2c, IntersectionPoint)) { return TRUE; } }
	if (CrossPointVectorPlane(rVertex1c, rVertex1a, Plane2, IntersectionPoint)) { if (PointInTriangle(rVertex2a, rVertex2b, rVertex1c, IntersectionPoint)) { return TRUE; } }

	return FALSE;
}

int __fastcall SCFMathematics::InfiniteConePlaneRelation(_IN Float3& rConeDirection, _IN Float3& rConePosition, _IN float fConeHalfAngle, _IN Float4& rPlane)
{
	register float fCos      = MultiplyVectors3((Float3&)rPlane, rConeDirection);
	register float fDistance = PointPlaneDistance(rConePosition, rPlane);
	register float fAngle    = SCFDegToRad * fConeHalfAngle;

	//If cone tip lies in front of or on the rPlane
	if (fDistance >= 0)
	{
		//rPlane normal & cone direction are not "opposite"
		if (fCos >= 0)
		{
			if (((SCFPi/2 - acosf(fCos)) - fAngle) >= 0)
			{
				return 1;
			}
			else { return 0; }
		}
		//rPlane normal & cone direction are "opposite", and because we are in front of the rPlane, the cone axis itself will intersect the rPlane, so the cone will also
		else { return 0; }
	}
	//Cone tip lies behind the rPlane
	else
	{
		//rPlane normal & cone direction are "opposite"
		if (fCos < 0)
		{
			if (((acosf(fCos) - SCFPi/2) - fAngle) >= 0)
			{
				return -1;
			}
			else { return 0; }
		}
		//rPlane normal & cone direction are not "opposite", and because we are behind the rPlane, the cone axis itself will intersect the rPlane, so the cone will also
		else { return 0; }
	}
}

bool __fastcall SCFMathematics::InfiniteConeContainsPoint(_IN Float3& rConeDirection, _IN Float3& rConePosition, _IN float fConeHalfAngle, _IN Float3& rPoint)
{
	//Make a plane that goes through the tip of the cone &
	//check whether the point lies behind this plane.
	//If it does, then the cone does NOT contain it
	register Float4 Plane;
	MakePlane(Plane, rConePosition, rConeDirection);
	if (PointPlaneDistance(rPoint, Plane) < 0) { return FALSE; }

	//Make a normalized vector pointing from the tip of the cone towards the point
	register Float3 Direction;
	MakeVector3(Direction, rConePosition, rPoint);
	NormalizeVector3(Direction, Direction);

	//Measure the angle between the direction of the cone &
	//the direction towards the point & compare with cone half angle
	if (MeasureAngle3N(rConeDirection, Direction) > fConeHalfAngle) { return FALSE; }

	return TRUE;
}

int __fastcall SCFMathematics::InfiniteConeSphereRelation(_IN Float3& rConeDirection, _IN Float3& rConePosition, _IN float fConeHalfAngle, _IN Float3& rPosition, _IN float fRadius)
{
	//Make a plane that goes through the tip of the cone &
	//check whether the sphere lies behind this plane.
	//If it does, then the cone does NOT contain it
	register Float4 Plane;
	MakePlane(Plane, rConePosition, rConeDirection);
	if (PointPlaneDistance(rPosition, Plane) < -fRadius) { return -1; }

	//Make a vector pointing from the tip of the cone towards the sphere 
	//position & measure it's length
	register Float3 Direction;
	MakeVector3(Direction, rConePosition, rPosition);
	float fSphereDistance = MeasureVector3(Direction);
	
	//Normalize the direction vector & measure the angle between the 
	//direction of the cone & the direction towards the sphere position
	DivideVector3(Direction, Direction, fSphereDistance);
	float fAngleToPosition = MeasureAngle3N(rConeDirection, Direction);

	//If the position is outside the cone
	if (fAngleToPosition > fConeHalfAngle)
	{
		if ((fAngleToPosition - SCFRadToDeg * atan2f(fRadius, fSphereDistance)) > fConeHalfAngle)
		{
			return -1;
		}
		else { return 0; }
	}
	else
	{
		if ((fAngleToPosition + SCFRadToDeg * atan2f(fRadius, fSphereDistance)) < fConeHalfAngle)
		{
			return 1;
		}
		else { return 0; }
	}
}

void __fastcall SCFMathematics::MultiplyVectorT4Matrix4(Float4& rResult, Float4& rVector, Matrix4& rMatrix)
{
	rResult[0] = rVector[0] * (float)rMatrix[0][0] + rVector[1] * (float)rMatrix[0][1] + rVector[2] * (float)rMatrix[0][2] + rVector[3] * (float)rMatrix[0][3];
	rResult[1] = rVector[0] * (float)rMatrix[1][0] + rVector[1] * (float)rMatrix[1][1] + rVector[2] * (float)rMatrix[1][2] + rVector[3] * (float)rMatrix[1][3];
	rResult[2] = rVector[0] * (float)rMatrix[2][0] + rVector[1] * (float)rMatrix[2][1] + rVector[2] * (float)rMatrix[2][2] + rVector[3] * (float)rMatrix[2][3];
	rResult[3] = rVector[0] * (float)rMatrix[3][0] + rVector[1] * (float)rMatrix[3][1] + rVector[2] * (float)rMatrix[3][2] + rVector[3] * (float)rMatrix[3][3];
}

void __fastcall SCFMathematics::MultiplyVectorT3Matrix3(Float3& rResult, Float3& rVector, Matrix3& rMatrix)
{
	rResult[0] = rVector[0] * (float)rMatrix[0][0] + rVector[1] * (float)rMatrix[0][1] + rVector[2] * (float)rMatrix[0][2];
	rResult[1] = rVector[0] * (float)rMatrix[1][0] + rVector[1] * (float)rMatrix[1][1] + rVector[2] * (float)rMatrix[1][2];
	rResult[2] = rVector[0] * (float)rMatrix[2][0] + rVector[1] * (float)rMatrix[2][1] + rVector[2] * (float)rMatrix[2][2];
}

void __fastcall SCFMathematics::MultiplyVector4Matrix4(Matrix4& rResult, Float4& rVector, Matrix4& rMatrix)
{
	rResult[0][0] = rVector[0] * rMatrix[0][0] + rVector[0] * rMatrix[0][1] + rVector[0] * rMatrix[0][2] + rVector[0] * rMatrix[0][3]; 
	rResult[1][0] = rVector[0] * rMatrix[1][0] + rVector[0] * rMatrix[1][1] + rVector[0] * rMatrix[1][2] + rVector[0] * rMatrix[1][3]; 
	rResult[2][0] = rVector[0] * rMatrix[2][0] + rVector[0] * rMatrix[2][1] + rVector[0] * rMatrix[2][2] + rVector[0] * rMatrix[2][3]; 
	rResult[3][0] = rVector[0] * rMatrix[3][0] + rVector[0] * rMatrix[3][1] + rVector[0] * rMatrix[3][2] + rVector[0] * rMatrix[3][3]; 

	rResult[0][1] = rVector[1] * rMatrix[0][0] + rVector[1] * rMatrix[0][1] + rVector[1] * rMatrix[0][2] + rVector[1] * rMatrix[0][3]; 
	rResult[1][1] = rVector[1] * rMatrix[1][0] + rVector[1] * rMatrix[1][1] + rVector[1] * rMatrix[1][2] + rVector[1] * rMatrix[1][3]; 
	rResult[2][1] = rVector[1] * rMatrix[2][0] + rVector[1] * rMatrix[2][1] + rVector[1] * rMatrix[2][2] + rVector[1] * rMatrix[2][3]; 
	rResult[3][1] = rVector[1] * rMatrix[3][0] + rVector[1] * rMatrix[3][1] + rVector[1] * rMatrix[3][2] + rVector[1] * rMatrix[3][3]; 

	rResult[0][2] = rVector[2] * rMatrix[0][0] + rVector[2] * rMatrix[0][1] + rVector[2] * rMatrix[0][2] + rVector[2] * rMatrix[0][3]; 
	rResult[1][2] = rVector[2] * rMatrix[1][0] + rVector[2] * rMatrix[1][1] + rVector[2] * rMatrix[1][2] + rVector[2] * rMatrix[1][3]; 
	rResult[2][2] = rVector[2] * rMatrix[2][0] + rVector[2] * rMatrix[2][1] + rVector[2] * rMatrix[2][2] + rVector[2] * rMatrix[2][3]; 
	rResult[3][2] = rVector[2] * rMatrix[3][0] + rVector[2] * rMatrix[3][1] + rVector[2] * rMatrix[3][2] + rVector[2] * rMatrix[3][3]; 

	rResult[0][3] = rVector[3] * rMatrix[0][0] + rVector[3] * rMatrix[0][1] + rVector[3] * rMatrix[0][2] + rVector[3] * rMatrix[0][3]; 
	rResult[1][3] = rVector[3] * rMatrix[1][0] + rVector[3] * rMatrix[1][1] + rVector[3] * rMatrix[1][2] + rVector[3] * rMatrix[1][3]; 
	rResult[2][3] = rVector[3] * rMatrix[2][0] + rVector[3] * rMatrix[2][1] + rVector[3] * rMatrix[2][2] + rVector[3] * rMatrix[2][3]; 
	rResult[3][3] = rVector[3] * rMatrix[3][0] + rVector[3] * rMatrix[3][1] + rVector[3] * rMatrix[3][2] + rVector[3] * rMatrix[3][3]; 
}

void __fastcall SCFMathematics::MultiplyVector3Matrix3(Matrix3& rResult, Float3& rVector, Matrix3& rMatrix)
{
	rResult[0][0] = rVector[0] * rMatrix[0][0] + rVector[0] * rMatrix[0][1] + rVector[0] * rMatrix[0][2]; 
	rResult[1][0] = rVector[0] * rMatrix[1][0] + rVector[0] * rMatrix[1][1] + rVector[0] * rMatrix[1][2]; 
	rResult[2][0] = rVector[0] * rMatrix[2][0] + rVector[0] * rMatrix[2][1] + rVector[0] * rMatrix[2][2]; 

	rResult[0][1] = rVector[1] * rMatrix[0][0] + rVector[1] * rMatrix[0][1] + rVector[1] * rMatrix[0][2]; 
	rResult[1][1] = rVector[1] * rMatrix[1][0] + rVector[1] * rMatrix[1][1] + rVector[1] * rMatrix[1][2]; 
	rResult[2][1] = rVector[1] * rMatrix[2][0] + rVector[1] * rMatrix[2][1] + rVector[1] * rMatrix[2][2]; 

	rResult[0][2] = rVector[2] * rMatrix[0][0] + rVector[2] * rMatrix[0][1] + rVector[2] * rMatrix[0][2]; 
	rResult[1][2] = rVector[2] * rMatrix[1][0] + rVector[2] * rMatrix[1][1] + rVector[2] * rMatrix[1][2]; 
	rResult[2][2] = rVector[2] * rMatrix[2][0] + rVector[2] * rMatrix[2][1] + rVector[2] * rMatrix[2][2]; 
}

void __fastcall SCFMathematics::MultiplyMatrices3(Matrix3& rResult, Matrix3& rMatrix1, Matrix3& rMatrix2)
{
	rResult[0][0] = rMatrix1[0][0] * rMatrix2[0][0] + (float)rMatrix1[1][0] * rMatrix2[0][1] + (float)rMatrix1[2][0] * rMatrix2[0][2];
	rResult[1][0] = rMatrix1[0][0] * rMatrix2[1][0] + (float)rMatrix1[1][0] * rMatrix2[1][1] + (float)rMatrix1[2][0] * rMatrix2[1][2];
	rResult[2][0] = rMatrix1[0][0] * rMatrix2[2][0] + (float)rMatrix1[1][0] * rMatrix2[2][1] + (float)rMatrix1[2][0] * rMatrix2[2][2];

	rResult[0][1] = rMatrix1[0][1] * rMatrix2[0][0] + (float)rMatrix1[1][1] * rMatrix2[0][1] + (float)rMatrix1[2][1] * rMatrix2[0][2];
	rResult[1][1] = rMatrix1[0][1] * rMatrix2[1][0] + (float)rMatrix1[1][1] * rMatrix2[1][1] + (float)rMatrix1[2][1] * rMatrix2[1][2];
	rResult[2][1] = rMatrix1[0][1] * rMatrix2[2][0] + (float)rMatrix1[1][1] * rMatrix2[2][1] + (float)rMatrix1[2][1] * rMatrix2[2][2];

	rResult[0][2] = rMatrix1[0][2] * rMatrix2[0][0] + (float)rMatrix1[1][2] * rMatrix2[0][1] + (float)rMatrix1[2][2] * rMatrix2[0][2];
	rResult[1][2] = rMatrix1[0][2] * rMatrix2[1][0] + (float)rMatrix1[1][2] * rMatrix2[1][1] + (float)rMatrix1[2][2] * rMatrix2[1][2];
	rResult[2][2] = rMatrix1[0][2] * rMatrix2[2][0] + (float)rMatrix1[1][2] * rMatrix2[2][1] + (float)rMatrix1[2][2] * rMatrix2[2][2];
}

void __fastcall SCFMathematics::MultiplyMatrices4(Matrix4& rResult, Matrix4& rMatrix1, Matrix4& rMatrix2)
{
	rResult[0][0] = rMatrix1[0][0] * rMatrix2[0][0] + (float)rMatrix1[1][0] * rMatrix2[0][1] + (float)rMatrix1[2][0] * rMatrix2[0][2] + (float)rMatrix1[3][0] * rMatrix2[0][3]; 
	rResult[1][0] = rMatrix1[0][0] * rMatrix2[1][0] + (float)rMatrix1[1][0] * rMatrix2[1][1] + (float)rMatrix1[2][0] * rMatrix2[1][2] + (float)rMatrix1[3][0] * rMatrix2[1][3]; 
	rResult[2][0] = rMatrix1[0][0] * rMatrix2[2][0] + (float)rMatrix1[1][0] * rMatrix2[2][1] + (float)rMatrix1[2][0] * rMatrix2[2][2] + (float)rMatrix1[3][0] * rMatrix2[2][3]; 
	rResult[3][0] = rMatrix1[0][0] * rMatrix2[3][0] + (float)rMatrix1[1][0] * rMatrix2[3][1] + (float)rMatrix1[2][0] * rMatrix2[3][2] + (float)rMatrix1[3][0] * rMatrix2[3][3]; 

	rResult[0][1] = rMatrix1[0][1] * rMatrix2[0][0] + (float)rMatrix1[1][1] * rMatrix2[0][1] + (float)rMatrix1[2][1] * rMatrix2[0][2] + (float)rMatrix1[3][1] * rMatrix2[0][3]; 
	rResult[1][1] = rMatrix1[0][1] * rMatrix2[1][0] + (float)rMatrix1[1][1] * rMatrix2[1][1] + (float)rMatrix1[2][1] * rMatrix2[1][2] + (float)rMatrix1[3][1] * rMatrix2[1][3]; 
	rResult[2][1] = rMatrix1[0][1] * rMatrix2[2][0] + (float)rMatrix1[1][1] * rMatrix2[2][1] + (float)rMatrix1[2][1] * rMatrix2[2][2] + (float)rMatrix1[3][1] * rMatrix2[2][3]; 
	rResult[3][1] = rMatrix1[0][1] * rMatrix2[3][0] + (float)rMatrix1[1][1] * rMatrix2[3][1] + (float)rMatrix1[2][1] * rMatrix2[3][2] + (float)rMatrix1[3][1] * rMatrix2[3][3]; 

	rResult[0][2] = rMatrix1[0][2] * rMatrix2[0][0] + (float)rMatrix1[1][2] * rMatrix2[0][1] + (float)rMatrix1[2][2] * rMatrix2[0][2] + (float)rMatrix1[3][2] * rMatrix2[0][3]; 
	rResult[1][2] = rMatrix1[0][2] * rMatrix2[1][0] + (float)rMatrix1[1][2] * rMatrix2[1][1] + (float)rMatrix1[2][2] * rMatrix2[1][2] + (float)rMatrix1[3][2] * rMatrix2[1][3]; 
	rResult[2][2] = rMatrix1[0][2] * rMatrix2[2][0] + (float)rMatrix1[1][2] * rMatrix2[2][1] + (float)rMatrix1[2][2] * rMatrix2[2][2] + (float)rMatrix1[3][2] * rMatrix2[2][3]; 
	rResult[3][2] = rMatrix1[0][2] * rMatrix2[3][0] + (float)rMatrix1[1][2] * rMatrix2[3][1] + (float)rMatrix1[2][2] * rMatrix2[3][2] + (float)rMatrix1[3][2] * rMatrix2[3][3]; 

	rResult[0][3] = rMatrix1[0][3] * rMatrix2[0][0] + (float)rMatrix1[1][3] * rMatrix2[0][1] + (float)rMatrix1[2][3] * rMatrix2[0][2] + (float)rMatrix1[3][3] * rMatrix2[0][3]; 
	rResult[1][3] = rMatrix1[0][3] * rMatrix2[1][0] + (float)rMatrix1[1][3] * rMatrix2[1][1] + (float)rMatrix1[2][3] * rMatrix2[1][2] + (float)rMatrix1[3][3] * rMatrix2[1][3]; 
	rResult[2][3] = rMatrix1[0][3] * rMatrix2[2][0] + (float)rMatrix1[1][3] * rMatrix2[2][1] + (float)rMatrix1[2][3] * rMatrix2[2][2] + (float)rMatrix1[3][3] * rMatrix2[2][3]; 
	rResult[3][3] = rMatrix1[0][3] * rMatrix2[3][0] + (float)rMatrix1[1][3] * rMatrix2[3][1] + (float)rMatrix1[2][3] * rMatrix2[3][2] + (float)rMatrix1[3][3] * rMatrix2[3][3]; 
}

void __fastcall SCFMathematics::MultiplyMatrix4Matrix3(Matrix4& rResult, Matrix4& rMatrix1, Matrix3& rMatrix2)
{
	rResult[0][0] = rMatrix1[0][0] * rMatrix2[0][0] + (float)rMatrix1[1][0] * rMatrix2[0][1] + (float)rMatrix1[2][0] * rMatrix2[0][2]; 
	rResult[1][0] = rMatrix1[0][0] * rMatrix2[1][0] + (float)rMatrix1[1][0] * rMatrix2[1][1] + (float)rMatrix1[2][0] * rMatrix2[1][2]; 
	rResult[2][0] = rMatrix1[0][0] * rMatrix2[2][0] + (float)rMatrix1[1][0] * rMatrix2[2][1] + (float)rMatrix1[2][0] * rMatrix2[2][2]; 
	rResult[3][0] = rMatrix1[3][0]; 

	rResult[0][1] = rMatrix1[0][1] * rMatrix2[0][0] + (float)rMatrix1[1][1] * rMatrix2[0][1] + (float)rMatrix1[2][1] * rMatrix2[0][2]; 
	rResult[1][1] = rMatrix1[0][1] * rMatrix2[1][0] + (float)rMatrix1[1][1] * rMatrix2[1][1] + (float)rMatrix1[2][1] * rMatrix2[1][2]; 
	rResult[2][1] = rMatrix1[0][1] * rMatrix2[2][0] + (float)rMatrix1[1][1] * rMatrix2[2][1] + (float)rMatrix1[2][1] * rMatrix2[2][2]; 
	rResult[3][1] = rMatrix1[3][1]; 

	rResult[0][2] = rMatrix1[0][2] * rMatrix2[0][0] + (float)rMatrix1[1][2] * rMatrix2[0][1] + (float)rMatrix1[2][2] * rMatrix2[0][2]; 
	rResult[1][2] = rMatrix1[0][2] * rMatrix2[1][0] + (float)rMatrix1[1][2] * rMatrix2[1][1] + (float)rMatrix1[2][2] * rMatrix2[1][2]; 
	rResult[2][2] = rMatrix1[0][2] * rMatrix2[2][0] + (float)rMatrix1[1][2] * rMatrix2[2][1] + (float)rMatrix1[2][2] * rMatrix2[2][2]; 
	rResult[3][2] = rMatrix1[3][2]; 

	rResult[0][3] = rMatrix1[0][3] * rMatrix2[0][0] + (float)rMatrix1[1][3] * rMatrix2[0][1] + (float)rMatrix1[2][3] * rMatrix2[0][2]; 
	rResult[1][3] = rMatrix1[0][3] * rMatrix2[1][0] + (float)rMatrix1[1][3] * rMatrix2[1][1] + (float)rMatrix1[2][3] * rMatrix2[1][2]; 
	rResult[2][3] = rMatrix1[0][3] * rMatrix2[2][0] + (float)rMatrix1[1][3] * rMatrix2[2][1] + (float)rMatrix1[2][3] * rMatrix2[2][2]; 
	rResult[3][3] = rMatrix1[3][3]; 
}

void __fastcall SCFMathematics::MultiplyMatrix3Matrix4(Matrix4& rResult, Matrix3& rMatrix1, Matrix4& rMatrix2)
{
	rResult[0][0] = rMatrix1[0][0] * rMatrix2[0][0] + (float)rMatrix1[1][0] * rMatrix2[0][1] + (float)rMatrix1[2][0] * rMatrix2[0][2]; 
	rResult[1][0] = rMatrix1[0][0] * rMatrix2[1][0] + (float)rMatrix1[1][0] * rMatrix2[1][1] + (float)rMatrix1[2][0] * rMatrix2[1][2]; 
	rResult[2][0] = rMatrix1[0][0] * rMatrix2[2][0] + (float)rMatrix1[1][0] * rMatrix2[2][1] + (float)rMatrix1[2][0] * rMatrix2[2][2]; 
	rResult[3][0] = rMatrix1[0][0] * rMatrix2[3][0] + (float)rMatrix1[1][0] * rMatrix2[3][1] + (float)rMatrix1[2][0] * rMatrix2[3][2]; 

	rResult[0][1] = rMatrix1[0][1] * rMatrix2[0][0] + (float)rMatrix1[1][1] * rMatrix2[0][1] + (float)rMatrix1[2][1] * rMatrix2[0][2]; 
	rResult[1][1] = rMatrix1[0][1] * rMatrix2[1][0] + (float)rMatrix1[1][1] * rMatrix2[1][1] + (float)rMatrix1[2][1] * rMatrix2[1][2]; 
	rResult[2][1] = rMatrix1[0][1] * rMatrix2[2][0] + (float)rMatrix1[1][1] * rMatrix2[2][1] + (float)rMatrix1[2][1] * rMatrix2[2][2]; 
	rResult[3][1] = rMatrix1[0][1] * rMatrix2[3][0] + (float)rMatrix1[1][1] * rMatrix2[3][1] + (float)rMatrix1[2][1] * rMatrix2[3][2]; 

	rResult[0][2] = rMatrix1[0][2] * rMatrix2[0][0] + (float)rMatrix1[1][2] * rMatrix2[0][1] + (float)rMatrix1[2][2] * rMatrix2[0][2]; 
	rResult[1][2] = rMatrix1[0][2] * rMatrix2[1][0] + (float)rMatrix1[1][2] * rMatrix2[1][1] + (float)rMatrix1[2][2] * rMatrix2[1][2]; 
	rResult[2][2] = rMatrix1[0][2] * rMatrix2[2][0] + (float)rMatrix1[1][2] * rMatrix2[2][1] + (float)rMatrix1[2][2] * rMatrix2[2][2]; 
	rResult[3][2] = rMatrix1[0][2] * rMatrix2[3][0] + (float)rMatrix1[1][2] * rMatrix2[3][1] + (float)rMatrix1[2][2] * rMatrix2[3][2]; 

	rResult[0][3] = rMatrix2[0][3]; 
	rResult[1][3] = rMatrix2[1][3]; 
	rResult[2][3] = rMatrix2[2][3]; 
	rResult[3][3] = rMatrix2[3][3]; 
}

void __fastcall SCFMathematics::ComposeMatrix3(_OUT Matrix3& rResult, _IN Float3& rVector1, _IN Float3& rVector2)
{
	rResult[0][0] = rVector1[0] * rVector2[0];  
	rResult[0][1] = rVector1[0] * rVector2[1];  
	rResult[0][2] = rVector1[0] * rVector2[2];  
	rResult[1][0] = rVector1[1] * rVector2[0];  
	rResult[1][1] = rVector1[1] * rVector2[1];  
	rResult[1][2] = rVector1[1] * rVector2[2];  
	rResult[2][0] = rVector1[2] * rVector2[0];  
	rResult[2][1] = rVector1[2] * rVector2[1];  
	rResult[2][2] = rVector1[2] * rVector2[2];  
}
void __fastcall SCFMathematics::ComposeMatrix4(_OUT Matrix4& rResult, _IN Float4& rVector1, _IN Float4& rVector2)
{
	rResult[0][0] = rVector1[0] * rVector2[0];  
	rResult[0][1] = rVector1[0] * rVector2[1];  
	rResult[0][2] = rVector1[0] * rVector2[2];  
	rResult[0][3] = rVector1[0] * rVector2[3];  
	rResult[1][0] = rVector1[1] * rVector2[0];  
	rResult[1][1] = rVector1[1] * rVector2[1];  
	rResult[1][2] = rVector1[1] * rVector2[2];  
	rResult[1][3] = rVector1[1] * rVector2[3];  
	rResult[2][0] = rVector1[2] * rVector2[0];  
	rResult[2][1] = rVector1[2] * rVector2[1];  
	rResult[2][2] = rVector1[2] * rVector2[2];  
	rResult[2][3] = rVector1[2] * rVector2[3];  
	rResult[3][0] = rVector1[3] * rVector2[0];  
	rResult[3][1] = rVector1[3] * rVector2[1];  
	rResult[3][2] = rVector1[3] * rVector2[2];  
	rResult[3][3] = rVector1[3] * rVector2[3];  
}

float SCFMathematics::TriangleArea2(_IN Float2& rVertex1, _IN Float2& rVertex2, _IN Float2& rVertex3)
{
	return __abs((((rVertex2[0] - rVertex1[0]) * (rVertex3[1] - rVertex1[1])) - ((rVertex2[1] - rVertex1[1]) * (rVertex3[0] - rVertex1[0]))) / 2.0f);
}

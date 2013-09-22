// ======================================================== //
//                                                          //
// Filename : MATRIX.H                                      //
// Content  : Source file of 3D matrix (4x4) declaration    //
// Author   : Roman Zeyde                                   //
// Date     : 21/01/2000                                    //
//                                                          //
// ======================================================== //

#include <matrix.h>
#include <vector.h>

#include <math.h>

TMatrix::TMatrix() // Nullifying the matrix
{
	for (TSize r = 0; r < 4; r++)
		for (TSize c = 0; c < 4; c++)
			mat[r][c] = 0;
}

// Matrix multiplication
TMatrix TMatrix::operator * (const TMatrix &m) const
{
	TMatrix result;

	for (TSize r = 0; r < 4; r++)
		for (TSize c = 0; c < 4; c++)
			for (TSize i = 0; i < 4; i++)
				result.mat[r][c] += mat[r][i] * m.mat[i][c];

	return result;
}

// Unity matrix has 4 1's in main diagonal
TUnity::TUnity() : TMatrix()
{
	mat[0][0] = 1;
	mat[1][1] = 1;
	mat[2][2] = 1;
	mat[3][3] = 1;
}

// Shift factors are in the 4th row
TShift::TShift(Real dx, Real dy, Real dz) : TUnity()
{
	mat[3][0] = dx;
	mat[3][1] = dy;
	mat[3][2] = dz;
}

// Scale factors are in the main diagonal
TScale::TScale(Real sx, Real sy, Real sz) : TUnity()
{
	mat[0][0] = sx;
	mat[1][1] = sy;
	mat[2][2] = sz;
}

// Sine and Cosine of rotation angle are in YZ rows and columns
TRotateX::TRotateX(Real a) : TUnity()
{
	Real SinA = sin(a);
	Real CosA = cos(a);

	mat[1][1] = CosA;
	mat[1][2] = SinA;
	mat[2][1] = -SinA;
	mat[2][2] = CosA;
}

// Sine and Cosine of rotation angle are in XZ rows and columns
TRotateY::TRotateY(Real a) : TUnity()
{
	Real SinA = sin(a);
	Real CosA = cos(a);

	mat[0][0] = CosA;
	mat[0][2] = SinA;
	mat[2][0] = -SinA;
	mat[2][2] = CosA;
}

// Sine and Cosine of rotation angle are in XY rows and columns
TRotateZ::TRotateZ(Real a) : TUnity()
{
	Real SinA = sin(a);
	Real CosA = cos(a);

	mat[0][0] = CosA;
	mat[0][1] = SinA;
	mat[1][0] = -SinA;
	mat[1][1] = CosA;
}

// This matrix is TShift(-px,-py,-pz) * TScale(sx,sy,sz) * TShift(px,py,pz)
TPivotScale::TPivotScale(Real sx, Real sy, Real sz, const TVector &p) :
TScale(sx, sy, sz)
{
	mat[3][0] = (1 - sx) * p.CrdX();
	mat[3][1] = (1 - sy) * p.CrdY();
	mat[3][2] = (1 - sz) * p.CrdZ();
}

// This matrix is TShift(0,-py,-pz) * TRotate(a) * TShift(0,py,pz)
TPivotRotateX::TPivotRotateX(Real a, Real py, Real pz) : TRotateX(a)
{
	mat[3][1] = (1 - mat[1][1])*py + pz*mat[1][2];
	mat[3][2] = (1 - mat[1][1])*pz - py*mat[1][2];
}

// This matrix is TShift(-px,0,-pz) * TRotate(a) * TShift(px,0,pz)
TPivotRotateY::TPivotRotateY(Real a, Real px, Real pz) : TRotateY(a)
{
	mat[3][0] = (1 - mat[0][0])*px + pz*mat[0][2];
	mat[3][2] = (1 - mat[0][0])*pz - px*mat[0][2];
}

// This matrix is TShift(-px,-py,0) * TRotate(a) * TShift(px,py,0)
TPivotRotateZ::TPivotRotateZ(Real a, Real px, Real py) : TRotateZ(a)
{
	mat[3][0] = (1 - mat[0][0])*px + py*mat[0][1];
	mat[3][1] = (1 - mat[0][0])*py - px*mat[0][1];
}

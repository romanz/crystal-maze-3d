// ======================================================== //
//                                                          //
// Filename : VECTOR.CPP                                    //
// Content  : Source file of 3D vector (x,y,z) declaration  //
// Author   : Roman Zeyde                                   //
// Date     : 18/01/2000                                    //
//                                                          //
// ======================================================== //

#include <Vector.h>
#include <Matrix.h>

// Vector's constructor
TVector::TVector(Real vx, Real vy, Real vz) : x(vx), y(vy), z(vz)
{
}

// The implementaion of Vector-by-Matrix multiplication
// See MATRIX.H for details
TVector operator * (const TVector &p, const TMatrix &m)
{
	return TVector
	(
		p.x*m.mat[0][0] +
		p.y*m.mat[1][0] +
		p.z*m.mat[2][0] +
			 m.mat[3][0],

		p.x*m.mat[0][1] +
		p.y*m.mat[1][1] +
		p.z*m.mat[2][1] +
			 m.mat[3][1],

		p.x*m.mat[0][2] +
		p.y*m.mat[1][2] +
		p.z*m.mat[2][2] +
			 m.mat[3][2]
	);
}

// Allow Matrix-by-Vector mutiplication (a change in operands' order)
TVector operator * (const TMatrix &m, const TVector &p)
{
	return (p * m);
}

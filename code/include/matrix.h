// ======================================================== //
//                                                          //
// Filename : MATRIX.H                                      //
// Content  : Header file of 3D matrix (4x4) declaration    //
// Author   : Roman Zeyde                                   //
// Date     : 21/01/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __MATRIX_H
#define __MATRIX_H

#include <defines.h>

class TVector; // used for multiplication by matrix

class TMatrix // 3D space transformation matrix
{

public:

	TMatrix(); // Nullifying constructor

	// Matrix multiplication
	TMatrix operator * (const TMatrix &m) const;

	// Multiplication and assignment
	TMatrix &operator *= (const TMatrix &m);

	// Vector-by-Matrix multiplication (and vice versa)
	friend TVector operator * (const TVector &p, const TMatrix &m);
	friend TVector operator * (const TMatrix &m, const TVector &p);

protected:

	Real mat[4][4]; // Matrix values

};

inline
TMatrix &TMatrix::operator *= (const TMatrix &m)
{
	return *this = *this * m;
}


class TUnity : public TMatrix // Doesn't change its multiplicator
{

public:

	TUnity();

};

class TShift : public TUnity // Shifts by (dx,dy,dz)
{

public:

	TShift(Real dx, Real dy, Real dz);

};

class TScale : public TUnity // Scales by (sx,sy,sz)
{

public:

	TScale(Real sx, Real sy, Real sz);

};

class TRotateX : public TUnity // Rotates around X axis by A radians
{

public:

	TRotateX(Real a);

};

class TRotateY : public TUnity // Rotates around Y axis by A radians
{

public:

	TRotateY(Real a);

};

class TRotateZ : public TUnity // Rotates around Z axis by A radians
{

public:

	TRotateZ(Real a);

};

class TPivotScale : public TScale // Scales from given point (p)
{

public:

	TPivotScale(Real sx, Real sy, Real sz, const TVector &p);

};

class TPivotRotateX : public TRotateX // Rotates around (Y,Z) axis
{

public:

	TPivotRotateX(Real a, Real py, Real pz);

};

class TPivotRotateY : public TRotateY // Rotates around (X,Z) axis
{

public:

	TPivotRotateY(Real a, Real px, Real pz);

};

class TPivotRotateZ : public TRotateZ // Rotates around (X,Y) axis
{

public:

	TPivotRotateZ(Real a, Real px, Real py);

};

#endif

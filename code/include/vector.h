// ======================================================== //
//                                                          //
// Filename : VECTOR.H                                      //
// Content  : Header file of 3D vector (x,y,z) declaration  //
// Author   : Roman Zeyde                                   //
// Date     : 18/01/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __VECTOR_H
#define __VECTOR_H

#include <defines.h>

#include <math.h>

class TMatrix; // used for multiplication by vector

class TVector // 3D space vector
{

public:

	// (Default) Constructor
	TVector(Real vx = 0, Real vy = 0, Real vz = 0);

	Real &CrdX(); // X coordinate
	Real CrdX() const;

	Real &CrdY(); // Y coordinate
	Real CrdY() const;

	Real &CrdZ(); // Z coordinate
	Real CrdZ() const;

	// Vector-by-Matrix multiplication (and vice versa)
	friend TVector operator * (const TVector &p, const TMatrix &m);
	friend TVector operator * (const TMatrix &m, const TVector &p);

	// Multiplication by Matrix and result assignment
	TVector &operator *= (const TMatrix &m);

	// Vector-by-Scalar multiplication (and vice versa)
	friend TVector operator * (const TVector &v, Real t);
	friend TVector operator * (Real t, const TVector &v);

	// Vector-by-Scalar division
	TVector operator / (Real t);

	// Multiplication/Division by Scalar and result assignment
	void operator *= (Real t) { *this = *this * t; }
	void operator /= (Real t) { *this = *this / t; }

	// Unary minus operator
	TVector operator - () const;

	// Vector addition/substraction
	TVector operator + (const TVector &v) const;
	TVector operator - (const TVector &v) const;

	// Vector addition/substraction and result assignment
	TVector &operator += (const TVector &v);
	TVector &operator -= (const TVector &v);

	// Dot product
	Real operator * (const TVector &v) const;

	// Vector's length
	Real Length() const;

protected:

	Real x, y, z; // Coordinates

};

inline
Real &TVector::CrdX()
{
	return x;
}

inline
Real TVector::CrdX() const
{
	return x;
}

inline
Real &TVector::CrdY()
{
	return y;
}

inline
Real TVector::CrdY() const
{
	return y;
}

inline
Real &TVector::CrdZ()
{
	return z;
}

inline
Real TVector::CrdZ() const
{
	return z;
}

inline
TVector TVector::operator - () const
{
	return TVector(-x, -y, -z);
}

inline
TVector TVector::operator + (const TVector &v) const
{
	return TVector(x + v.x, y + v.y, z + v.z);
}

inline
TVector TVector::operator - (const TVector &v) const
{
	return TVector(x - v.x, y - v.y, z - v.z);
}

inline
TVector &TVector::operator += (const TVector &v)
{
	return *this = *this + v;
}

inline
TVector &TVector::operator -= (const TVector &v)
{
	return *this = *this - v;
}

inline
Real TVector::operator * (const TVector &v) const
{
	return x*v.x + y*v.y + z*v.z;
}

inline
TVector &TVector::operator *= (const TMatrix &m)
{
	return *this = *this * m;
}

inline
Real TVector::Length() const
{
	return hypot(hypot(x, y), z);
}

inline
TVector operator * (const TVector &v, Real t)
{
	return TVector(v.x * t, v.y * t, v.z * t);
}

inline
TVector operator * (Real t, const TVector &v)
{
	return (v * t);
}

inline
TVector TVector::operator / (Real t)
{
	return TVector(x / t, y / t, z / t);
}

#endif

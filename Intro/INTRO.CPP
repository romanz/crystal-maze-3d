#include "Key.h"
#include <CType.h>

#include <Viewport.h>
#include <Screen.h>
#include <Shape.h>

TScreen Screen(VGA, VGAMED, "", 2);

TViewport Viewport(
 TVector(-4, -3), TVector(4, 3), 10,
 Screen.Pixel(0,0), Screen.Pixel(100,100)
);

class TOpenMatrix : public TMatrix
{

public:

	TOpenMatrix() : TMatrix() {}
	TOpenMatrix(const TMatrix &m) : TMatrix(m) {}

	Real &Value(TSize i, TSize j)
	{
//		Assert( i < 4 && j < 4);
		return mat[i][j];
	}

};

class TVectorRotate : public TUnity
{

public:

	// Rotates by alpha around given vector [v1,v2]
	TVectorRotate(Real alpha, const TVector &v1, const TVector &v2);

};

TVectorRotate::TVectorRotate(
 Real alpha,
 const TVector &v1,
 const TVector &v2
)
 : TUnity()
{
	static TOpenMatrix T, Rx, Ry; // will be modified
	static TVector u;
	static Real d;

	u = v2 - v1;
	u /= u.Length(); // Unity vector

	const Real &a = u.CrdX();
	const Real &b = u.CrdY();
	const Real &c = u.CrdZ();

	d = hypot(b, c); // for rotation

	T = TShift(-v1.CrdX(), -v1.CrdY(), -v1.CrdZ());

	Rx = Ry = TUnity();

	Rx.Value(2,2) =   Rx.Value(1,1) = d ? c/d : 1;  // CosA
	Rx.Value(2,1) = -(Rx.Value(1,2) = d ? b/d : 0); // SinA

	Ry.Value(2,2) =   Ry.Value(0,0) = d;
	Ry.Value(2,0) = -(Ry.Value(0,2) = a);

	operator *= ( T * Rx * Ry * TRotateZ(alpha) );

	// Transform it back

	T.Value(3,0) = -T.Value(3,0);
	T.Value(3,1) = -T.Value(3,1);
	T.Value(3,2) = -T.Value(3,2);

	// Change SinA to -SinA
	Rx.Value(2,1) = -(Rx.Value(1,2) = Rx.Value(2,1));
	Ry.Value(2,0) = -(Ry.Value(0,2) = Ry.Value(2,0));

	operator *= ( Ry * Rx * T );
}

enum Coord { X, Y, Z };

class TShip : public TPolyhedron
{

public:

	TShip() :
	 TPolyhedron("Ship.dat"),
	 Base(0,0,0),
	 CrdX(1,0,0),
	 CrdY(0,1,0),
	 CrdZ(0,0,1)
	{
		TPolyhedron::operator *= (
		 TScale(0.3, 0.3, 0.3)*
		 TShift(0.0, 0.0,-0.5)
		);

		Move(0.5, Z);
	}

	void Turn(Real a, Coord c)
	{
		TMatrix m = TurnMatrix(a, c);

		TPolyhedron::operator *= (m);
		Base *= m;
		CrdX *= m;
		CrdY *= m;
		CrdZ *= m;
	}

	void Move(Real d, Coord c)
	{
		TMatrix m = MoveMatrix(d, c);

		TPolyhedron::operator *= (m);
		Base *= m;
		CrdX *= m;
		CrdY *= m;
		CrdZ *= m;
	}

protected:

	TMatrix TurnMatrix(Real a, Coord c);
	TMatrix MoveMatrix(Real a, Coord c);

	TVector Base, CrdX, CrdY, CrdZ;

};

TMatrix TShip::TurnMatrix(Real a, Coord c)
{
	switch (c)
	{
		case X:  return TVectorRotate(a, Base, CrdX);
		case Y:  return TVectorRotate(a, Base, CrdY);
		case Z:  return TVectorRotate(a, Base, CrdZ);
		default: return TUnity();
	}
}

TMatrix TShip::MoveMatrix(Real d, Coord c)
{
	TVector v;

	switch (c)
	{
		case X:  v = CrdX - Base; break;
		case Y:  v = CrdY - Base; break;
		case Z:  v = CrdZ - Base; break;
	}

	v *= d;

	return TShift(v.CrdX(), v.CrdY(), v.CrdZ());
}

int main(void)
{
//	TKeyboard &KB = Keyboard;

	TShip ship; // 3D Spaceship
	TViewer viewer;

	viewer << &ship; // The only 3D object
	viewer.Update(); // Update polygons list

	Screen.Switch(); // Switch for first time

	ship.Move(-4, Y);
	ship.Move(-2, Z);

	const Real a = 6*M_PI/360;

	for (int v = 5; !Keyboard.Key(kESC); Keyboard.Clear())
	{
		if (Keyboard.Key(kLARROW)) ship.Turn(-a, Y);
		if (Keyboard.Key(kRARROW)) ship.Turn( a, Y);
		if (Keyboard.Key(kUARROW)) ship.Turn( a, X);
		if (Keyboard.Key(kDARROW)) ship.Turn(-a, X);
		if (Keyboard.Key(kA)) v++;
		if (Keyboard.Key(kZ)) v--;

		if (Keyboard.Key(kLEFTCTRL)) ship.Turn( a, Z);;
		if (Keyboard.Key(kLEFTALT )) ship.Turn(-a, Z);;

		ship.Move(v/100.0, Z);

		viewer.Sort();
		viewer.Show();

		Screen.Switch(20);
	}

	return 0;
}

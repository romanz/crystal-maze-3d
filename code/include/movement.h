// ======================================================== //
//                                                          //
// Filename : MOVEMENT.H                                    //
// Content  : Header file of player movement class          //
// Author   : Roman Zeyde                                   //
// Date     : 11/04/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __MOVEMENT_H
#define __MOVEMENT_H

#include <defines.h>
#include <tarray.h>

//
// Position and directions of the player in the maze
//

// Direction enum
enum Direction {
 dirLeft,
 dirRight,
 dirUp,
 dirDown,
 dirForward,
 dirBack
};

// Directions class is used for 3D rotations
class TDirection
{

public:

	TDirection() :

	 // Directions
	 left(dirLeft),       right(dirRight),
	 up(dirUp),           down(dirDown),
	 forward(dirForward), back(dirBack)

	{
	}

	// Turning functions:
	// The idea is "rotating" the values of the relevant
	// variables to keep the directions correct.

	void TurnLeft()  { Turn(forward, left,  back, right); } // Turn left
	void TurnRight() { Turn(forward, right, back, left);  } // Turn right
	void TurnUp()    { Turn(forward, up,    back, down);  } // Turn up
	void TurnDown()  { Turn(forward, down,  back, up);    } // Turn down

	Direction Left()    const { return left;    } // "Left" direction
	Direction Right()   const { return right;   } // "Right" direction
	Direction Up()      const { return up;      } // "Up" direction
	Direction Down()    const { return down;    } // "Down" direction
	Direction Forward() const { return forward; } // "Forward" direction
	Direction Back()    const { return back;    } // "Back" direction

	Direction Dir(Direction d); // switch version

protected:

	// Left-rotates {a,b,c,d} directions
	void Turn(Direction &a, Direction &b, Direction &c, Direction &d);

	Direction left, right, up, down, forward, back;

};

class TCoordinate
{

public:

	TCoordinate(int cx = 0, int cy = 0, int cz = 0) : x(cx), y(cy), z(cz) {}

	// Move function moves the coordinate in given direction
	// Returns 1 - if the move have succeeded, 0 - otherwise
	virtual int Move(Direction dir);

	int CrdX() const { return x; }
	int CrdY() const { return y; }
	int CrdZ() const { return z; }

	int operator == (const TCoordinate &d)
	{
		return x == d.x && y == d.y && z == d.z;
	}

	int operator != (const TCoordinate &d)
	{
		return !operator == (d);
	}

protected:

	int x, y, z;

};

class TCell
{

public:

	TCell();

	Byte &Wall(Direction d) { return walls[d]; }
	Byte Wall(Direction d) const { return walls[d]; }

protected:

	Byte walls[6];

};

// Maps 3D coordinates (i,j,k) to 1D index of n-size cube
// Used in MAZE.CPP for initializing the walls
#define MAP_3D(i,j,k,n) ((i)*SQUARE(n) + (j)*(n) + (k))

class TMazeCube : public TCoordinate, public TDirection
{

public:

	TMazeCube(TSize n, const char *path):
	 TCoordinate(0, 0, 0),
	 TDirection(),
	 cells(CUBE(n)),
	 number(n)
	{
		CreateOutsideWalls();
		CreateInsideWalls(path);
	}

	TCell &Cell(const TCoordinate &p) // Specified p(x,y,z) cell
	{
		return cells[Index(p)];
	}

	TCell &Cell() // Current cell
	{
		return cells[Index(*this)];
	}

	virtual int Move(Direction dir); // Overloaded TCoordinate::Move()

protected:

	TSize Index(const TCoordinate &p);

	TArray<TCell> cells;
	TSize number;

private:

	void CreateOutsideWalls();
	void CreateInsideWalls(const char *path); // data file path

};

// Left-rotates {a,b,c,d} directions
inline void TDirection::Turn
(
 Direction &a,
 Direction &b,
 Direction &c,
 Direction &d
)

{
	Direction x = a; // Saves a value

	a = b;
	b = c;
	c = d;
	d = x; // Using a value
}

// Conerting from 3D index to 1D one
inline TSize TMazeCube::Index(const TCoordinate &p)
{
	int index = MAP_3D(p.CrdX(), p.CrdY(), p.CrdZ(), number);
	Assert(index < cells.Size()); // Validation

	return index;
}

// Moving in the cube (considering the walls)
inline int TMazeCube::Move(Direction dir) // Overloaded TCoordinate::Move()
{
	return !Cell().Wall(dir) ? TCoordinate::Move(dir) : False;
	// If there is no wall, move (and return True)
	// Otherwise, don't move (and return False)
}

#endif // __MOVEMENT_H

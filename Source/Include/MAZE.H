// ======================================================== //
//                                                          //
// Filename : MAZE.H                                        //
// Content  : Header file of 3D maze classes                //
// Author   : Roman Zeyde                                   //
// Date     : 11/04/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __MAZE_H
#define __MAZE_H

#include <Shape.h>
#include <Defines.h>
#include <Matrix.h>

// The class derives from Polyhedron class
class TMazeView : public TPolyhedron
{

public:

	// Size of the maze (nxnxn), Path to walls file
	TMazeView(TSize n, const char *path);

	TVector Vector(Real x, Real y, Real z)
	{
		return
			Base +
			(VecX-Base) * x +
			(VecY-Base) * y +
			(VecZ-Base) * z;
	}

	void operator *= (const TMatrix &m)
	{
		TPolyhedron::operator *= (m);
		Base *= m;
		VecX *= m;
		VecY *= m;
		VecZ *= m;
	}

protected:

	// Returns [i][j][k] vertex
	TVertex &Vertex(TSize i, TSize j, TSize k);

	TSize size; // Maze's size

	TVector Base, VecX, VecY, VecZ;

};

#endif // __MAZE_H
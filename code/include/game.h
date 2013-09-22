// ======================================================== //
//                                                          //
// Filename : GAME.H                                        //
// Content  : Header file of the maze game                  //
// Author   : Roman Zeyde                                   //
// Date     : 19/04/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __GAME_H
#define __GAME_H

#include <controls.h>
#include <movement.h>
#include <screen.h>
#include <matrix.h>
#include <shape.h>
#include <maze.h>

// Maze class contains everything to enable the game
class TMaze : public TRectControl
{

public:

	TMaze(const char *DataFile, const char *t); // Reads maze data

	~TMaze() // Frees allocated memory
	{
		if (Crystal)
			delete Crystal;
	}

	void Movement(Direction d); // Makes the move in d direction

	int Found() { return found; } // Is the crystal found?

	int Update(); // Main function
	// It is overloaded TControl::Update(), so it has to be called
	// one time each iteration. The result is: 0 - no change,
	// 1 - changed, needs to be redrawn.

	void Show(); // Shows everything

	TCoordinate Position() { return Cube; } // Current position
	TCoordinate Destination() { return Dest; } // Destination

	// Is there a wall in "d" direction
	int Wall(Direction d) { return Cube.Cell().Wall(Cube.Dir(d)); }

	int Moving() { return Step; } // Is the maze moving?

protected:

	enum Animation { aniNone, aniMovement, aniCrystal };

	TMatrix MoveMaze(Direction d); // Matrix for moving maze
	TMatrix MoveShip(Direction d); // Matrix for moving maze

	int found;

	TCoordinate Random(); // Create random coordinate (all except current)
	TPolyhedron *NewCrystal(); // Creates new crystal

	TSize Step; // Animation counter
	Direction Move; // Direction of move
	Animation AnimationType; // Type of animation

	TCoordinate Dest; // Destination

	const TSize Frames; // Number of frames in each transfromation
	const Real DeltaZ; // dZ - How much to move the viewport forward

	// 3D bodies
	TMazeView Maze;
	TPolyhedron Ship;
	TPolyhedron *Crystal; // To create multiple crystals

	// Cube for checks
	TMazeCube Cube;

	// 3D viewer
	TViewer Viewer;

};

// Size of the maze
const TSize MazeSize = 4;

#endif

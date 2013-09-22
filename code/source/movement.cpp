// ======================================================== //
//                                                          //
// Filename : MOVEMENT.CPP                                  //
// Content  : Source file of player movement class          //
// Author   : Roman Zeyde                                   //
// Date     : 11/04/2000                                    //
//                                                          //
// ======================================================== //

#include <iostream.h>
#include <fstream.h>

#include <movement.h>

Direction TDirection::Dir(Direction d) // switch version
{
	switch (d)
	{
		case dirRight:   	return right;
		case dirLeft:    	return left;
		case dirUp:      	return up;
		case dirDown:    	return down;
		case dirForward: 	return forward;
		case dirBack:    	return back;
		default:          return Direction(-1); // Error value
	}
}

// Moves the position in specified direction (results in success)
int TCoordinate::Move(Direction dir)
{
	switch (dir)
	{
		case dirRight:   x++; break;
		case dirLeft:    x--; break;
		case dirUp:      y++; break;
		case dirDown:    y--; break;
		case dirForward: z++; break;
		case dirBack:    z--; break;
	}

	return True; // By deafault, the move is successful
}

// TCell's constructor
TCell::TCell()
{
	for (TSize i = 0; i < 6; i++)
		walls[i] = False;
}

// Creating outside walls
void TMazeCube::CreateOutsideWalls()
{
	for (TSize i = 0; i < number; i++)
		for (TSize j = 0; j < number; j++)
		{
			Cell(TCoordinate(i,j,0)).Wall(dirBack) 		= True;
			Cell(TCoordinate(i,j,number-1)).Wall(dirForward) 	= True;

			Cell(TCoordinate(i,0,j)).Wall(dirDown) 		= True;
			Cell(TCoordinate(i,number-1,j)).Wall(dirUp) 		= True;

			Cell(TCoordinate(0,i,j)).Wall(dirLeft) 		= True;
			Cell(TCoordinate(number-1,i,j)).Wall(dirRight) 	= True;
		}
}

// The file contains the walls in X, Y, Z order
void TMazeCube::CreateInsideWalls(const char *path)
{
	ifstream in(path);
	Assert(in); // Assertion: the file is opened

	TSize i, j, k; // k is the "dimension" variable
	int wall; // Boolean wall presence

	// Reading X-normal walls (having the same X)
	for (k = 1; k < number; k++)
		for (i = 0; i < number; i++)
			for (j = 0; j < number; j++)
			{
				Assert(in >> wall); // Can read from file

				if (wall)
				{
					Cell(TCoordinate(k,   i, j)).Wall(dirLeft) 		= True;
					Cell(TCoordinate(k-1, i, j)).Wall(dirRight) 		= True;
				}
			}

	// Reading Y-normal walls (having the same Y)
	for (k = 1; k < number; k++)
		for (i = 0; i < number; i++)
			for (j = 0; j < number; j++)
			{
				Assert(in >> wall); // Can read from file

				if (wall)
				{
					Cell(TCoordinate(i, k,   j)).Wall(dirDown) 		= True;
					Cell(TCoordinate(i, k-1, j)).Wall(dirUp) 			= True;
				}
			}

	// Reading Z-normal walls (having the same Z)
	for (k = 1; k < number; k++)
		for (i = 0; i < number; i++)
			for (j = 0; j < number; j++)
			{
				Assert(in >> wall); // Can read from file
				if (wall)
				{
					Cell(TCoordinate(i, j, k  )).Wall(dirBack) 		= True;
					Cell(TCoordinate(i, j, k-1)).Wall(dirForward) 	= True;
				}
			}


	in.close();
}

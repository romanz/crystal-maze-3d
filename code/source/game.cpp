// ======================================================== //
//                                                          //
// Filename : GAME.CPP                                      //
// Content  : Source file of the maze game                  //
// Author   : Roman Zeyde                                   //
// Date     : 19/04/2000                                    //
//                                                          //
// ======================================================== //

#include <Game.h>
#include <ConIO.h>

TMaze::TMaze(const char *DataFile, const char *t):
 TRectControl(
  TPixel(Viewport.ScreenLeft(), Viewport.ScreenTop()),
  TPixel(Viewport.ScreenRight(), Viewport.ScreenBottom())
 ),
 Maze(MazeSize, DataFile),
 Cube(MazeSize, DataFile),
 Ship("Ship.DAT"),
 Crystal(NULL),
 Step(0), Frames(3), DeltaZ(0.3),
 Viewer(), AnimationType(aniNone),
 found(False)
{
	tag = t;

	// First of all, "prepare" Maze
	Maze *= TShift(-0.5,-0.5,-0.5+DeltaZ);

	// Create new crystal
	Crystal = NewCrystal();

	// View the maze and the ship
	Viewer << &Maze << &Ship << Crystal;
	Viewer.Update();

	Ship
	 *= TScale(0.05, 0.08, 0.08)
	 *= TShift(0, -0.2 , 0.1)
	 *= TPivotRotateX(-M_PI/30,0,0.5)
	;


	Viewer.Sort();
}

int TMaze::Update()
{
	found = False; // Crystal isn't found
	if (Step) // Transform only if necessary
	{
		switch (AnimationType)
		{
			case aniMovement: // There is a movement
				Maze *= MoveMaze(Move);
				*Crystal *= MoveMaze(Move);

				if (Step == Frames) // First step
					Ship *= MoveShip(Move); // Move ship
				else if (Step == 1) // Last step
					Ship *= MoveShip(Move ^ 1); // Move it back

				break;

			case aniCrystal: // Animating crystal
				*Crystal *=
				 TPivotRotateY(M_PI/12, 0, DeltaZ) *
				 TPivotScale(0.9, 1.0, 0.9, TVector(0,0,DeltaZ));

				sound(900+40*Step);

				break;

		} // switch (AnimationType)

		if (!--Step) // Last step is done
		{
			nosound();

			switch (AnimationType)
			{
				case aniMovement:
					if (Dest == Cube) // Check if crystal is found
					{
						AnimationType = aniCrystal;
						Step = 15; // Animation steps (frames)
					}
					break;

				case aniCrystal: // Crystal animation is done

					delete Crystal; // Delete crystal
					Crystal = NewCrystal(); // Create new crystal (continue)

					Viewer.RemoveFromTail(); // Remove old crystal
					Viewer.AddToTail(Crystal); // Add new crystal (continue)
					Viewer.Update(); // Update viewer's list

					found = True; // Now the crystal is found
					AnimationType = aniNone;

					break;

			} // switch (AnimationType)

		} // if (!--Step)

		return 1;

	} // if (Step)

	return 0;
}

void TMaze::Show() // Shows everything
{
	Viewport.Set();

	Viewer.Sort();
	Viewer.Show();

	Screen.Set();

	TRectangle(
		TPixel(Viewport.ScreenLeft(), Viewport.ScreenTop()),
		TPixel(Viewport.ScreenRight(), Viewport.ScreenBottom())
	).Draw(BLACK, BLACK, WHITE, WHITE);
}

void TMaze::Movement(Direction d) // Makes the move in d direction
{
	if (Moving())
		return;

	switch (d)
	{
		case dirForward:
			if (!Cube.Move( Cube.Forward() )) // Can't move forward
				return;
			else
				break;

		case dirBack:
			if (!Cube.Move( Cube.Back() )) // Can't move back
				return;
			else
				break;

		case dirUp:
			Cube.TurnUp();
			break;

		case dirDown:
			Cube.TurnDown();
			break;

		case dirLeft:
			Cube.TurnLeft();
			break;

		case dirRight:
			Cube.TurnRight();
			break;
	}

	Move = d;
	Step = Frames;
	AnimationType = aniMovement;
}

// Chooses the matrix to move the maze
TMatrix TMaze::MoveMaze(Direction d)
{
	switch (d)
	{
		case dirForward:
			return TShift(0, 0, -1.0/Frames);

		case dirBack:
			return TShift(0, 0, 1.0/Frames);

		case dirUp:
			return TPivotRotateX(M_PI/2 / Frames, 0, DeltaZ);

		case dirDown:
			return TPivotRotateX(-M_PI/2 / Frames, 0, DeltaZ);

		case dirLeft:
			return TPivotRotateY(-M_PI/2 / Frames, 0, DeltaZ);

		case dirRight:
			return TPivotRotateY(M_PI/2 / Frames, 0, DeltaZ);

		default:
			return TUnity();
	}
}

// Chooses the matrix to move the ship
TMatrix TMaze::MoveShip(Direction d)
{
	switch (d)
	{
		case dirForward:
			return TShift(0, 0, 0.1);

		case dirBack:
			return TShift(0, 0, -0.1);

		case dirUp:
			return TPivotRotateX(-M_PI/32, 0.0, 0.4);

		case dirDown:
			return TPivotRotateX(M_PI/32, 0.0, 0.4);

		case dirLeft:
			return TPivotRotateZ(M_PI/18, 0.0, -0.1);

		case dirRight:
			return TPivotRotateZ(-M_PI/18, 0.0, -0.1);

		default:
			return TUnity();
	}
}

TCoordinate TMaze::Random() // Creates random coordinte in the maze
{
	TCoordinate p;

	do
	{
		p = TCoordinate(
		 random(MazeSize),
		 random(MazeSize),
		 random(MazeSize)
		);
	}
	while (p == Cube); // Until p != current coordinate

	return p;
}

TPolyhedron *TMaze::NewCrystal() // Creates new crystal
{
	TPolyhedron *p = new TPolyhedron("Crystal.Dat");

	Assert(p);

	// New destination coordinate
	Dest = Random();
	TVector v = Maze.Vector(Dest.CrdX(), Dest.CrdY(), Dest.CrdZ());

	*p *=
	 TPivotScale(0.2, 0.2, 0.2, TVector()) *
	 TShift(v.CrdX(), v.CrdY(), v.CrdZ());

	return p;
}

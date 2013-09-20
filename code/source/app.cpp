// ======================================================== //
//                                                          //
// Filename : APP.CPP                                       //
// Content  : Source file of the maze application           //
// Author   : Roman Zeyde                                   //
// Date     : 21/04/2000                                    //
//                                                          //
// ======================================================== //

#include <App.h>
#include <ASCII.h>

#include <StrStrea.h>
#include <IOManip.h>

#include <ConIO.h>

#include <Alloc.h>

const TSize NumberOfCrystals = 10; // to find

// Data file constructor
App::App(const char *MazeFile):

// The constructor initializes all the controls

 maze   (MazeFile, "Game viewport"),

 panel(
  Screen.Pixel(0,0), Screen.Pixel(100,100),
  0, LIGHTGRAY, "Point any control to get instant help"
 ),

 control(
  Screen.Pixel(72,30), Screen.Pixel(99,38),
	"Control Panel", -1, LIGHTGRAY, "Ship control panel"
  ),

 move(
  Screen.Pixel(72,41), Screen.Pixel(99,46),
  "Move direction", -1, LIGHTGRAY, "Move ship: forward or back"
 ),

 forward(
  Screen.Pixel(72,47), Screen.Pixel(85,52),
  "Forward", "Moves the ship forward (+)"
 ),

 back(
  Screen.Pixel(86,47), Screen.Pixel(99,52),
  "Back", "Moves the ship back (-)"
 ),

 turn(
  Screen.Pixel(72,53), Screen.Pixel(99,58),
  "Turn direction", -1, LIGHTGRAY, "Turn ship: left, right, up or down"
 ),

 left(
  Screen.Pixel(72,59), Screen.Pixel(85,64),
  "Left", "Turns the ship to the left (Left arrow)"
 ),
 right(
  Screen.Pixel(86,59), Screen.Pixel(99,64),
  "Right", "Turns the ship to the right (Right arrow)"
 ),

 up(
  Screen.Pixel(72,65), Screen.Pixel(85,70),
  "Up", "Turns the ship up (Up arrow)"
 ),

 down(
  Screen.Pixel(86,65), Screen.Pixel(99,70),
  "Down", "Turns the ship down (Down arrow)"
 ),

 info(
  Screen.Pixel( 1,30), Screen.Pixel(28,38),
  "Information", -1, LIGHTGRAY, "Game information"
 ),

 pos(
  Screen.Pixel( 1,44), Screen.Pixel(28,49),
  "", -1, LIGHTGREEN, "Current position of the ship (X,Y,Z)"
 ),

 dist(
  Screen.Pixel( 1,50), Screen.Pixel(28,55),
  "", -1, LIGHTGREEN, "Distance from the crystal"
 ),

 found(
  Screen.Pixel( 1,59), Screen.Pixel(28,64),
  "", -1, LIGHTBLUE, "Number of crystals left to find"
 ),

 timer(
  Screen.Pixel( 1,65), Screen.Pixel(28,70),
  "", -1, LIGHTBLUE, "Time elapsed from the beginning"
 ),

 status(Screen.Pixel(0,95), Screen.Pixel(100,100), "", -1, LIGHTGRAY),

 caption(
  Screen.Pixel( 1, 5), Screen.Pixel(99,25),
  "Crystal Maze 3D 2000"
 ),

 done(
  Screen.Pixel( 30, 75), Screen.Pixel(70,85),
  "Return to menu", "Exits the maze and returns to the main menu (Escape)"
 ),

 crystals(NumberOfCrystals),

 Start(clock())
{
	controls
	 << panel << maze
	 << forward << back
	 << left << right
	 << up << down
	 << control << move << turn
	 << info << found << pos << dist << timer
	 << caption << status << done;
	;
/*
	Screen.Switch(); // Switch pages (active != visual)

	Mouse.Page(Screen.Visual());
	Mouse.Show();*/
}

// Destructor
App::~App()
{
}

// Delay between 2 frames
const TSize Delay = 13;

// Runs the game
TSize App::Run()
{
	Timer t; // to switch the clock

	t.Start(1);

	for (;;)
	{
		Mouse.Update();

		// Current control tag
		status = controls.Tag();

		// If anything was updated, show it
		if (controls.Update() != 0 || t.Done())
		{
			if (t.Done()) // Start new timer
				t.Start(1);

			if (maze.Found()) // Increase number of crystals
				if (!--crystals)
					return (clock() - Start) / CLK_TCK;

			UpdateInfo(); // Updates information labels

			controls.Show(); // Shows the controls

			Mouse.Hide(); // Hide mouse

			Screen.Switch(Delay); // Switch pages
			Mouse.Page(Screen.Visual()); // Switch mouse

			Mouse.Show(); // Show mouse
		}

		ButtonInput(); // Get buttons input

		if (done.Click()) // Exit if "Exit" is clicked
			return 0;

		if (!KeyInput()) // Escape pressed
			return 0;

	}
}

// Updates information labels
void App::UpdateInfo()
{
	char str[80]; // String buffer

	ostrstream(str, sizeof(str))
	 << "Crystals left: " << crystals << ends;

	found = str;

	TCoordinate p = maze.Position();

	ostrstream(str, sizeof(str))
	 << "Coordinate: ("
	 << p.CrdX() << ","
	 << p.CrdY() << ","
	 << p.CrdZ() << ")"
	 << ends;

	pos = str;

	TCoordinate d = maze.Destination();

	ostrstream(str, sizeof(str))
	 << "Distance: "
	 << setprecision(2)
	 << TVector(
		  p.CrdX() - d.CrdX(),
		  p.CrdY() - d.CrdY(),
		  p.CrdZ() - d.CrdZ()
		 ).Length()
	 << ends;

	dist = str;

	TSize t = (clock() - Start) / CLK_TCK;

	ostrstream(str, sizeof(str))
	 << "Time: " << setfill('0')
	 << t/3600 << ":" << setw(2) << (t/60)%60 << "." << setw(2) << t%60
//	 << " " << coreleft()
	 << ends;

	timer = str;

}

// Gets input from the buttons
int App::ButtonInput()
{
	if      (forward.Click()) maze.Movement(dirForward);
	else if (back.Click())    maze.Movement(dirBack);
	else if (left.Click())    maze.Movement(dirLeft);
	else if (right.Click())   maze.Movement(dirRight);
	else if (up.Click())      maze.Movement(dirUp);
	else if (down.Click())    maze.Movement(dirDown);

	return 1;
}

// Gets input from keyboard
int App::KeyInput()
{
	if (!kbhit()) // If animation is running or no key is hit
		return 1; // Continue

	switch (getch()) // Convert to upper case
	{
		case '+':
			maze.Movement(dirForward);
			break;

		case '-':
			maze.Movement(dirBack);
			break;

		case 0:
			switch (getch())
			{
				case arrUp: // Turn the cockpit up
					maze.Movement(dirUp);
					break;

				case arrDown:  // Turn the cockpit down
					maze.Movement(dirDown);
					break;

				case arrLeft:  // Turn the cockpit left
					maze.Movement(dirLeft);
					break;

				case arrRight:  // Turn the cockpit right
					maze.Movement(dirRight);
					break;
			}
			break;

		case Escape:
			return 0;  // Exit
	}
	return 1; // Continue

}

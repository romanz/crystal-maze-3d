// ======================================================== //
//                                                          //
// Filename : MAIN.CPP                                      //
// Content  : Source file of the main function              //
// Author   : Roman Zeyde                                   //
// Date     : 23/04/2000                                    //
//                                                          //
// ======================================================== //

#include <Menu.h>
#include <Screen.h>

#include <Stdlib.h>
#include <Alloc.h>
#include <ConIO.h>

#include <ASCII.h>

// Graphics screen class (Driver, Mode, BGI path, Pages #)
TScreen Screen(VGA, VGAMED, "", 2); // Look for BGI in current directory

// ============================== Main ==============================

int main(void)
{
	Screen.Switch(); // Switch pages (active != visual)

	Mouse.Page(Screen.Visual()); // Show mouse on visual page
	Mouse.Show();

	randomize(); // Initializes random number generator

	TSize before = coreleft();

	switch ( TMenu().Run() )
	{
		case 1:
			outtextxy(0, 0,
			 "Error: No mazes found. "
			 "Please copy your .MAZ files to current directory.");
			Screen.Switch(); // Switch pages to see the message

			getch();
			break;

		default:
			// Everything's OK
			break;
	}

	TSize after = coreleft();

	Assert(before == after); // Checking for memory leak

	return 0;
}

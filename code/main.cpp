// ======================================================== //
//                                                          //
// Filename : MAIN.CPP                                      //
// Content  : Source file of the main function              //
// Author   : Roman Zeyde                                   //
// Date     : 23/04/2000                                    //
//                                                          //
// ======================================================== //

#include <menu.h>
#include <screen.h>

#include <stdlib.h>
#include <alloc.h>
#include <conio.h>

#include <ascii.h>

// Graphics screen class (Driver, Mode, BGI path, Pages #)
TScreen g_Screen(VGA, VGAMED, NULL, 2); // Look for BGI in current directory

// ============================== Main ==============================

inline int coreleft() { return 0; }

int main(void)
{
	g_Screen.Switch(); // Switch pages (active != visual)

	Mouse.Page(g_Screen.Visual()); // Show mouse on visual page
	Mouse.Show();

	srand(clock()); // Initializes random number generator

	TSize before = coreleft();

	switch ( TMenu().Run() )
	{
		case 1:
			outtextxy(0, 0,
			 "Error: No mazes found. "
			 "Please copy your .MAZ files to current directory.");
			g_Screen.Switch(); // Switch pages to see the message

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

// ======================================================== //
//                                                          //
// Filename : SCREEN.CPP                                    //
// Content  : Source file of screen class declaration       //
// Author   : Roman Zeyde                                   //
// Date     : 21/01/2000                                    //
//                                                          //
// ======================================================== //

#include <Screen.h>
#include <Defines.h>
#include <Mouse.h>

#include <IOStream.h>
#include <Graphics.h>
#include <Dos.h>

TScreen::TScreen(int gd, int gm, const char *path, int pages):

 number(pages), active(0), visual(0), width(0), height(0)

{
	initgraph(&gd, &gm, path);

	Assert(graphresult() == grOk);

	width  = getmaxx() + 1;
	height = getmaxy() + 1;

	// Sets active and visual pages to be #0
	setactivepage(0);
	setvisualpage(0);

	cleardevice();

	settextstyle(SMALL_FONT, HORIZ_DIR, 4); // Default text style
}

TScreen::~TScreen()
{
	closegraph();
	cout << "<<< Programmed by Roman Zeyde >>>";
}

void TScreen::Switch(unsigned t)
{
	setvisualpage(visual = active); // Switch visual page to this one

	if (t) // Switching pages takes some time
		delay(t);

	setactivepage(++active %= number); // Switch to new active page

	cleardevice(); // Clears new page
}

void TScreen::Set()
{
	setviewport(0, 0, getmaxx(), getmaxy(), True);
}
// ======================================================== //
//                                                          //
// Filename : SCREEN.H                                      //
// Content  : Header file of screen class declaration       //
// Author   : Roman Zeyde                                   //
// Date     : 21/01/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __SCREEN_H
#define __SCREEN_H

#include <pixel.h>
#include <defines.h>

//
// Screen class is used for graphics handling
//

class TScreen
{

public:

	// Initializes graphics system
	TScreen(int gd, int gm, char *path, int pages);
	// gd    - GraphDriver
	// gm    - GraphMode
	// path  - Path to BGI
	// pagse - Number of pages available

	// Closes graphics system
	~TScreen();

	void Switch(unsigned t = 0); // Page switch with delay(t)

	void Set(); // Set the screen to be current viewport

	int Active() const { return active; } // Active page
	int Visual() const { return visual; } // Visual page

	int Width() const { return width; }   // Screen's width
	int Height() const { return height; } // Screen's height

	TPixel Pixel(Real x, Real y);

protected:

	int active, visual, number; // pages
	int width, height; // size
};

inline
TPixel TScreen::Pixel(Real x, Real y)
{
	return TPixel( x/100*(width-1), y/100*(height-1) );
}

extern TScreen g_Screen; // At least one screen has to be used

#endif

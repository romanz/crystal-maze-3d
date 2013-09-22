// ======================================================== //
//                                                          //
// Filename : MOUSE.H                                       //
// Content  : Header file of mouse class declaration        //
// Author   : Roman Zeyde                                   //
// Date     : 21/01/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __MOUSE_H
#define __MOUSE_H

#include <pixel.h>

//
// Mouse button's state
//

enum TState { Up /*0*/, Down/*1*/, Released /*2*/, Pressed /*3*/ };

// Up, Down          - button is stable in one state
// Released, Pressed - button is changing its state

// "Up" states (Up, Released) & 1 == 0
// "Down" states (Down, Pressed) & 1 != 0

class TMouse
{

public:

	// Initializes mouse
	TMouse();

	// Closes mouse
	~TMouse();

	// Mouse's visibility
	void Show() const;
	void Hide() const;

	TMouse &Update(); // Updates mouse's data

	operator TPixel() const; // Current position

	// Buttons state information
	TState Left() const;
	TState Right() const;

	// Page handling functions
	void Page(int page);
	int Page();

protected:

	TState l, r; // Left and Right buttons' states
	TPixel p;    // Mouse position

	// Used for switching button's state
	TState Switch(TState old, int button);

};

extern TMouse Mouse; // One global mouse

inline
TMouse::operator TPixel() const
{
	return p;
}

inline
TState TMouse::Left() const
{
	return l;
}

inline
TState TMouse::Right() const
{
	return r;
}


#endif

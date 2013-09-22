// ======================================================== //
//                                                          //
// Filename : MOUSE.CPP                                     //
// Content  : Source file of mouse class declaration        //
// Author   : Roman Zeyde                                   //
// Date     : 21/01/2000                                    //
//                                                          //
// ======================================================== //

#include <mouse.h>

TMouse Mouse; // The one and only global mouse

// TODO: re-implement using Xbgi

TMouse::TMouse() : l(Up), r(Up), p() // (0,0)
{
	// INT 33H, AX=0000H : Initializes mouse
	Update(); // Update mouse data
}

TMouse::~TMouse()
{
	// INT 33H, AX=001FH : Closes mouse
}

void TMouse::Show() const
{
	// INT 33H, AX=0001H : Shows mouse's pointer
}

void TMouse::Hide() const
{
	// INT 33H, AX=0002H : Hides mouse's pointer
}

void TMouse::Page(int page)
{
	// INT 33H, AX=001DH : Sets mouse's page (BX)
}

int TMouse::Page()
{
	// INT 33H, AX=001DH : Gets mouse's page (BX)
}

TState TMouse::Switch(TState old, int button)
{
	if (old & 1) // if button was in "down" state
	{
		if (button) // if it's still "down"
			return Down;
		else // it has been released
			return Released;
	}
	else // it was in "up" state
	{
		if (button) // if it has been pressed
			return Pressed;
		else // it is still "up"
			return Up;
	}
}

TMouse &TMouse::Update()
{
	// INT 33H, AX=0003H : Gets mouse's data (state=BX,x=CX,y=DX)
	p.CrdX() = 0;
	p.CrdY() = 0;

	l = Switch(l, 0);
	r = Switch(r, 0);

	return *this;
}

// TODO: re-implement

// ======================================================== //
//                                                          //
// Filename : MOUSE.CPP                                     //
// Content  : Source file of mouse class declaration        //
// Author   : Roman Zeyde                                   //
// Date     : 21/01/2000                                    //
//                                                          //
// ======================================================== //

#include <Mouse.h>

REGS TMouse::regs; // Registers for interrupts

TMouse Mouse; // The one and only global mouse

TMouse::TMouse() : l(Up), r(Up), p() // (0,0)
{
	// INT 33H, AX=0000H : Initializes mouse
	regs.x.ax = 0x00;
	int86(0x33, &regs, &regs);

	Update(); // Update mouse data
}

TMouse::~TMouse()
{
	// INT 33H, AX=001FH : Closes mouse
	regs.x.ax = 0x1F;
	int86(0x33, &regs, &regs);
}

void TMouse::Show() const
{
	// INT 33H, AX=0001H : Shows mouse's pointer
	regs.x.ax = 0x01;
	int86(0x33, &regs, &regs);
}

void TMouse::Hide() const
{
	// INT 33H, AX=0002H : Hides mouse's pointer
	regs.x.ax = 0x02;
	int86(0x33, &regs, &regs);
}

void TMouse::Page(int page)
{
	// INT 33H, AX=001DH : Sets mouse's page (BX)
	regs.x.ax = 0x1D;
	regs.x.bx = page;
	int86(0x33, &regs, &regs);
}

int TMouse::Page()
{
	// INT 33H, AX=001DH : Gets mouse's page (BX)
	regs.x.ax = 0x1E;
	int86(0x33, &regs, &regs);
	return regs.x.bx;
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
	regs.x.ax = 0x03;
	int86(0x33, &regs, &regs);

	p.CrdX() = regs.x.cx;
	p.CrdY() = regs.x.dx;

	l = Switch(l, regs.x.bx & 0x01);
	r = Switch(r, regs.x.bx & 0x02);

	return *this;
}

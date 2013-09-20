// ======================================================== //
//                                                          //
// Filename : PIXEL.H                                       //
// Content  : Header file of screen pixel declaration       //
// Author   : Roman Zeyde                                   //
// Date     : 26/02/2000                                    //
//                                                          //
// ======================================================== //

#ifndef __PIXEL_H
#define __PIXEL_H

class TPixel
{

public:

	TPixel(int px = 0, int py = 0) : x(px), y(py)
	{
	}

	int &CrdX() { return x; }
	int CrdX() const { return x; }

	int &CrdY() { return y; }
	int CrdY() const { return y; }

protected:

	int x, y;

};

#endif // __PIXEL_H
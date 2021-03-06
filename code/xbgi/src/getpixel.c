/*
 * $Id: getpixel.c,v 0.1 1993/12/10 00:15:31 king Exp king $
 * Gets the colour of a specified pixel.
 *
 * $Log: getpixel.c,v $
 * Revision 0.1  1993/12/10  00:15:31  king
 * Initial version.
 * 
 * Revision 0.2 2012/12/17 GG
 *
 */
#include "graphics.h"

unsigned int getpixel(int x, int y)
{
  unsigned int col;
  XImage *ximage;
  unsigned long pixel;
  
  ximage = XGetImage(dpy, window, x, y, 1, 1, AllPlanes, XYPixmap);
  pixel = XGetPixel(ximage, 0, 0);	
  XDestroyImage(ximage);
  if (rgb_mode)
    return pixel;
  else
    for (col = 0; col < MAXCOLORS + 1; col++)
      if (vga_palette[col].pixel_value == pixel)
	return (col);
}




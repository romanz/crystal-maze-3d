/* mandelbrot.c  -*- C -*-
 * 
 * To compile:
 * gcc -o mandelbrot mandelbrot.c /usr/lib/libXbgi.a -lX11 -lm
 * 
 * By Guido Gonzato, March 2013.
 * 
 * This is an unoptimised, simple but effective program for plotting
 * the Mandelbrot set. Left click to zoom in on a point, right click
 * to zoom out, middle click to restore the initial boundary,
 * ESC to quit.

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "../graphics.h"

int max_iter = 100;
int maxx, maxy;

void mandelbrot (double, double, double, double);
void explain (void);
void amber_palette (void);
void blue_palette (void);
void purple_palette (void);

/* ----- */

void mandelbrot (double x1, double y1, double x2, double y2)
{
  int xx, yy, counter;
  double dx, dy, x, y, a, b, tx, d;

  dx = (x2 - x1) / maxx;
  dy = (y2 - y1) / maxy;
  
  x = x1;
  for (xx = 0; xx < maxx; xx++) {
    
    y = y1;
    for (yy = 0; yy < maxy; yy++) {
      
      counter = 0;
      a = b = 0.0;
      
      // iteration: z(n+1) = z(n)^2 + c; z = ai + b; c = yi + x
      
      do {
	tx = a*a - b*b + x;
	b = 2*b*a + y;
	a = tx;
	d = a*a + b*b;
	counter++;
      } while (d <= 4.0 && counter < max_iter);
      
      // 16 colour mode:
      // int color;
      // if (counter == max_iter)
      //   color = BLACK;
      // else
      //   color = 1 + counter % 15;
      
      setrgbcolor (counter);
      _putpixel (xx, yy);
      y += dy;
      
    } // y
    x += dx;
    
  } // x
}

/* ----- */

void amber_palette (void)
{
  int c;
  
  // initialize rgb palette entries 0 - (max_iter - 1)
  for (c = 0; c < max_iter; c++)
    setrgbpalette (c, max_iter - c, 50 + 2 * c, c);
  setrgbpalette (max_iter, 0x30, 0, 0x30); // the Mandelbrot set is purple
}

/* ----- */

void purple_palette (void)
{
  int c;
  
  for (c = 0; c < max_iter; c++)
    setrgbpalette (c, 50 + 2 * c, c, max_iter - c);
  setrgbpalette (max_iter, 0, 0, 0); // the Mandelbrot set is black
}

/* ----- */

void blue_palette (void)
{
  int c;
  
  for (c = 0; c < max_iter; c++)
    setrgbpalette (c, 0, c, 50 + 2 * c);
  setrgbpalette (max_iter, 0, 0, 0); // the Mandelbrot set is black
}

/* ----- */

void explain (void)
{
  int i = 0, inc = 1, c;
  
  setbkcolor (COLOR (0, 0, 32)); // don't use a palette
  setcolor (COLOR (255, 255, 0));
  // alternatively, use setrgbcolor() or setbkrgbcolor()
  cleardevice ();
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  settextstyle (TRIPLEX_FONT, HORIZ_DIR, 1);
  c = textheight ("H");
  outtextxy (maxx / 2, maxy / 2 - 3*c,
	   "Press '1', '2', or '3' to change the palette;");
  outtextxy (maxx / 2, maxy / 2 - 2*c,
	   "left click to zoom in on a point;");
  outtextxy (maxx / 2, maxy / 2 - c,
	   "right click to zoom out;");
  outtextxy (maxx / 2, maxy / 2,
	   "middle click to restore the initial boundary;");
  outtextxy (maxx / 2, maxy / 2 + c,
	   "ESC to quit the program.");
  
  while (! kbhit () && ! mouseclick ()) {
    setcolor(COLOR (i, 0, 0));
    outtextxy (maxx / 2, maxy / 2 + 3*c, "PRESS A KEY OR CLICK TO BEGIN");
    i += inc;
    if (255 == i)
      inc = -1;
    if (0 == i)
      inc = 1;
    delay (1);
  }
  cleardevice ();
}

/* ----- */

int main (void)
{
  int palette, c, init, redraw, gd, gm;
  double xm, ym, xstep, ystep, x1, y1, x2, y2;
  
  gd = X11;
  gm = X11_1024x768;
  initgraph (&gd, &gm, "");
  
  maxx = getmaxx ();
  maxy = getmaxy ();
  xm = -0.75;
  ym = 0.0;
  xstep = 1.6;
  ystep = 1.2;
  redraw = 1;
  init = 1;
  
  explain ();
  
  purple_palette ();
  palette = 1;
  
  while (c != KEY_ESC) {
    
    x1 = xm - xstep;
    y1 = ym - ystep;
    x2 = xm + xstep;
    y2 = ym + ystep;

    if (redraw) {
      mandelbrot (x1, y1, x2, y2);
      redraw = 0;
    }
    
    c = getevent (); // wait for a key or mouse click
    
    switch (c) {
      
    case WM_LBUTTONDOWN:
      xm = x1 + (x2 - x1) * mousex () / maxx;
      ym = y1 + (y2 - y1) * mousey () / maxy;
      xstep /= 2;
      ystep /= 2;
      init = 0;
      redraw = 1;
      break;
      
    case WM_RBUTTONDOWN:
      xstep *= 2;
      ystep *= 2;
      init = 0;
      redraw = 1;
      break;
      
    case WM_MBUTTONDOWN:
      if (0 == init) {
	xm = -0.75;
	ym = 0.0;
	xstep = 1.6;
	ystep = 1.2;
	redraw = 1;
      }
      break;
      
    case '1':
      purple_palette ();
      if (1 != palette) {
	redraw = 1;
	palette = 1;
      }
      break;
      
    case '2':
      blue_palette ();
      if (2 != palette) {
	redraw = 1;
	palette = 2;
      }
      break;
      
    case '3':
      amber_palette ();
      if (3 != palette) {
	redraw = 1;
	palette = 3;
      }
      break;

    default:
      redraw = 0;
    }
    
  } // while

  return 0;
  
}

/*
 * turtledemo.c	-*- C -*-
 * 
 * By Guido Gonzato <guido.gonzato at gmail.com>
 * 2012-12-20
 * 
 * To compile this program:
 * 
 * gcc -o turtledemo turtledemo.c turtle.c /usr/lib/libXbgi.a -lX11 -lm
 * 
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
#include <math.h>
#include "../graphics.h"

#include "turtle.h"

/* ----- */

void koch (int len, int level)
{
  int newlen;
  if (0 == level) {
    forwd (len);
    return;
  }
  else {
    newlen = (int) ceil (len / 3.0);
    koch (newlen, level - 1);
    turnleft (60);
    koch (newlen, level - 1);
    turnright (120);
    koch (newlen, level - 1);
    turnleft (60);
    koch (newlen, level - 1);
  }
}

/* ----- */

void tree (int len, int level)
{
  if (0 == level) {
    return;
  }
  else {
    setcolor (level);
    forwd (len);
    turnleft (45);
    tree (len * 6 / 10, level - 1);
    turnleft (90);
    tree (len * 3 / 4, level - 1);
    turnleft (45);
    penup ();
    forwd (len);
    pendown ();
  }
}

/* ----- */

void sq_koch (int len, int level)
{
  if (0 == level) {
    forwd (len);
    return;
  }
  else {
    sq_koch (len / 4, level - 1);
    turnleft (90);
    sq_koch (len / 4, level - 1);
    turnright (90);
    sq_koch (len / 4, level - 1);
    turnright (90);
    sq_koch (len / 4, level - 1);
    sq_koch (len / 4, level - 1);
    turnleft (90);
    sq_koch (len / 4, level - 1);
    turnleft (90);
    sq_koch (len / 4, level - 1);
    turnright (90);
    sq_koch (len / 4, level - 1);
  }
}

/* ----- */

void star (int len)
{
  int i;
  for (i = 0; i < 5; i++) {
    forwd (len);
    turnright (144);
  }
}

/* ----- */

void star_6 (int len)
{
  int i;
  for (i = 0; i < 6; i++) {
    forwd (len);
    turnright (120);
    forwd (len);
    turnleft (60);
  }
}

/* ----- */

void star_20 (int len)
{
  int i;
  for (i = 0; i < 20; i++) {
    forwd (len);
    turnright (162);
  }
}

/* ----- */

void hilbert_left (int, int);
void hilbert_right (int, int);

/* ----- */

void hilbert_left (int len, int level)
{
  int bearing;
  
  if (level > 0) {
    
    bearing = heading ();
    turnleft (90);
    hilbert_right (len, level - 1);
    
    setheading (bearing);
    forwd (len);
    
    bearing = heading ();
    hilbert_left (len, level - 1);
    
    setheading (bearing);
    turnleft (90);
    forwd (len);
    
    bearing = heading ();
    turnright (90);
    hilbert_left (len, level - 1);
    
    setheading (bearing);
    turnleft (90);
    forwd (len);
    
    bearing = heading ();
    turnleft (90);
    hilbert_right (len, level - 1);
    setheading (bearing);
    
  }
  
} /* hilbert_left () */

/* ----- */

void hilbert_right (int len, int level)
{
  int bearing;
  
  if (level > 0) {
    
    bearing = heading ();
    turnright (90);
    hilbert_left (len, level - 1);
    
    setheading (bearing);
    forwd (len);
    
    bearing = heading ();
    hilbert_right (len, level - 1);
    
    setheading (bearing);
    turnright (90);
    forwd (len);
    
    bearing = heading ();
    turnleft (90);
    hilbert_right (len, level - 1);
    
    setheading (bearing);
    turnright (90);
    forwd (len);
    
    bearing = heading ();
    turnright (90);
    hilbert_left (len, level - 1);
    setheading (bearing);
    
  }

}

/* ----- */

int powerof2 (int ex)
{
  int i, n = 1;
  for (i = 0; i < ex; i++)
    n *= 2;
  return (n);
}

/* ----- */

int main (void)
{
  int i, l, x, y, xc, gd, gm;
  char s[32];

  gd = X11;
  gm = X11_1024x768;
  initgraph (&gd, &gm, "");

  setbkcolor (BLACK);

  /* Koch */
  for (i = 0; i < 6; i++) {
    cleardevice ();
    setcolor (GREEN);
    outtextxy (0, 0, "Standard Koch curve:");
    setposition (0, getmaxy () / 2);
    setheading (T_EAST);
    setcolor (i + 1);
    koch (getmaxx () + 1, i);
    usleep (500000);
  }
  setcolor (YELLOW);
  outtextxy (0, 20, "PRESS A KEY TO CONTINUE:");
  getch ();

  /* fractal tree */
  for (i = 0; i < 14; i++) {
    cleardevice ();
    setcolor (YELLOW);
    outtextxy (0, 0, "Tree:");
    setposition (getmaxx () *4/10, getmaxy ());
    setheading (T_NORTH);
    tree (getmaxy () / 3, i);
    usleep (500000);
  }
  setcolor (YELLOW);
  outtextxy (0, 20, "PRESS A KEY TO CONTINUE:");
  getch ();

  /* square Koch */
  for (i = 0; i < 6; i++) {
    cleardevice ();
    setcolor (RED);
    outtextxy (0, 0, "Square Koch curve:");
    setposition (0, getmaxy () / 2);
    setheading (T_EAST);
    setcolor (i + 1);
    sq_koch (getmaxx () + 1, i);
    usleep (500000);
  }
  setcolor (YELLOW);
  outtextxy (0, 20, "PRESS A KEY TO CONTINUE:");
  getch ();

  /* rotating square */
  cleardevice ();
  setcolor (RED);
  outtextxy (0, 0, "Rotating square:");
  home ();
  setheading (0);
  l = getmaxx () / 2;
  
  for (i = 1; i < l; i++) {
    setcolor (1 + i % 15);
    forwd (i);
    turnright (89);
    delay (10);
    if (kbhit ())
      break;
  }
  setcolor (YELLOW);
  outtextxy (0, 20, "PRESS A KEY TO CONTINUE:");
  getch ();

  /* Hilbert */
  setbkcolor (WHITE);
  cleardevice ();
  xc = getmaxx () / 2;
  x = xc;
  
  for (i = 1; i < 8; i++) {
    l = getmaxy () / powerof2 (i);
    x += l / 2;
    y = l / 2;
    setposition (x, y);
    setheading (T_WEST);
    setcolor (BLUE);
    hilbert_left (l, i);
    sprintf (s, "Hilbert curve at level %d", i);
    setcolor (BLUE);
    outtextxy (0, 0, s);
    getch ();
    cleardevice ();
  }
  outtextxy (0, 0, "PRESS A KEY TO EXIT:");
  
  /* stars */
  while (! kbhit ()) {
    setposition (random (getmaxx ()), random (getmaxy ()));
    setheading (random (360));
    setcolor (1 + random (15));
    star (random (80));
    star_6 (random (20));
    star_20 (random (40));
    usleep (50000);
  }
  
  closegraph ();
  return (0);
  
}

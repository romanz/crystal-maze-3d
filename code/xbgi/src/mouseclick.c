#include "graphics.h"

/* by GG */

int mouseclick (void)
{
  XFlush (dpy);
  
  while (1) {
    XEvent event;
    if (XCheckMaskEvent (dpy, -1, &event)) {
      if (event.type == ButtonPress) {
	mouse_x = event.xkey.x;
	mouse_y = event.xkey.y;
	button = event.xbutton.button;
	return button;
      }
      else 
	return 0;
    }
    else
      return 0;
  }
}

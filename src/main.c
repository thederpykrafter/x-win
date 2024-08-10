#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
   Display *d;
   Window w;
   XEvent e;
   int width, height, s;

   d = XOpenDisplay(NULL);
   if (d == NULL) {
      fprintf(stderr, "Cannot open display\n");
      exit(1);
   }

   s = DefaultScreen(d);
   w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1,
                           BlackPixel(d, s), WhitePixel(d, s));
   XSelectInput(d, w, ExposureMask | KeyPressMask);
   XMapWindow(d, w);

   width = DisplayWidth(d, s);
   height = DisplayWidth(d, s);
   
   static char swidth[16];
   static char sheight[16];

   // FIX: not program resolution
   snprintf(swidth, sizeof(swidth), "%d", width);
   snprintf(sheight, sizeof(sheight), "%d", height);

   const char* msg = strcat(swidth, sheight);

   while (1) {
      XNextEvent(d, &e);
      if (e.type == Expose) {
         XFillRectangle(d, w, DefaultGC(d, s), 100, 100, 10, 10);
         XDrawString(d, w, DefaultGC(d, s), 40, 50, msg, strlen(msg));
      }
      if (e.type == KeyPress)
         break;
   }

   XCloseDisplay(d);
   return 0;
}


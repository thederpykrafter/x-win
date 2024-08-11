#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdlib.h>
#include <stdio.h>

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 50

void draw_button(Display *display, Window window, GC gc, int x, int y) {
    XFillRectangle(display, window, gc, x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
    XSetForeground(display, gc, WhitePixel(display, DefaultScreen(display)));
    XDrawRectangle(display, window, gc, x, y, BUTTON_WIDTH, BUTTON_HEIGHT);
    XSetForeground(display, gc, BlackPixel(display, DefaultScreen(display)));
}

int main() {
    Display *display;
    Window window;
    XEvent event;
    int screen;
    GC gc;
    XGCValues gc_values;
    Colormap colormap;
    XColor color;
    KeyCode keycode_q;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 400, 300, 1, BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | ButtonPressMask | KeyPressMask);
    XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, &gc_values);

    colormap = DefaultColormap(display, screen);
    XParseColor(display, colormap, "black", &color);
    XAllocColor(display, colormap, &color);
    XSetForeground(display, gc, color.pixel);

    XParseColor(display, colormap, "white", &color);
    XAllocColor(display, colormap, &color);
    XSetBackground(display, gc, color.pixel);

    // Get the KeyCode for 'q'
    keycode_q = XKeysymToKeycode(display, XStringToKeysym("q"));

    // Event loop
    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            int win_width = event.xexpose.width;
            int win_height = event.xexpose.height;
            int button_x = (win_width - BUTTON_WIDTH) / 2;
            int button_y = (win_height - BUTTON_HEIGHT) / 2;

            draw_button(display, window, gc, button_x, button_y);
        } else if (event.type == ButtonPress) {
            int button_x = (event.xbutton.x - (BUTTON_WIDTH / 2));
            int button_y = (event.xbutton.y - (BUTTON_HEIGHT / 2));
            
            // Check if click is within the button area
            if (event.xbutton.x >= (button_x) && 
                event.xbutton.x <= (button_x + BUTTON_WIDTH) &&
                event.xbutton.y >= (button_y) && 
                event.xbutton.y <= (button_y + BUTTON_HEIGHT)) {
                printf("Button pressed!\n");
            }
        } else if (event.type == KeyPress) {
            if (event.xkey.keycode == keycode_q) {
                printf("Exiting program...\n");
                break;
            }
        }
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}


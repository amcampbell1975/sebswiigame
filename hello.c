#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// comment

// Minimal X11 code to open a window, draw text, and handle events.
// Complete example based on {Link: xlib-example tutorials https://rtime.felk.cvut.cz/osp/prednasky/gui/_xlib-example/}
int main() {
    Display *d = XOpenDisplay(NULL);
    if (d == NULL) return 0; // Exit if no display

    int s = DefaultScreen(d);
    Window w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 200, 100, 1,
                                   BlackPixel(d, s), WhitePixel(d, s));

    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);

    XEvent e;
    while (1) {
        XNextEvent(d, &e);
        if (e.type == Expose) {
            XDrawString(d, w, DefaultGC(d, s), 50, 50, "Hello, World!", 13);
        }
        if (e.type == KeyPress) break;
    }
    XCloseDisplay(d);
    return 0;
}

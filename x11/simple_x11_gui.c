// gcc simple_x11_gui.c   -o simple_x11_gui -lX11
#include <X11/Xlib.h>
#include <stdio.h>
#include <unistd.h>   // for usleep
//#include <stdlib.h>
//#include <math.h>

Display *display;
Window window;
int screen;
GC gc;

int main() {
    display = XOpenDisplay(NULL);
    screen = DefaultScreen(display);
    window = XCreateSimpleWindow( display,RootWindow(display, screen),0, 0,600, 400,1,BlackPixel(display, screen),WhitePixel(display, screen));

    //XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, NULL);
    //XSetForeground(display, gc, BlackPixel(display, screen));

    for(int i=10;i<400;i++) {
        // Clear window
        XClearWindow(display, window);
        
        XDrawRectangle(display, window, gc, i,100,50,25);

        XFlush(display);
        usleep((1/60.0)/ 0.000001);     // ~60 FPS
    }

    XCloseDisplay(display);
    return 0;
}

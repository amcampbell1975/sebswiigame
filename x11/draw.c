#include <X11/Xlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "draw.h"

typedef struct {
    float x, y;
} Point;

void rotate_point(Point *p, float cx, float cy, float angle) {
    float s = sin(angle);
    float c = cos(angle);

    // translate point back to origin
    p->x -= cx;
    p->y -= cy;

    // rotate
    float xnew = p->x * c - p->y * s;
    float ynew = p->x * s + p->y * c;

    // translate back
    p->x = xnew + cx;
    p->y = ynew + cy;
}

void draw_rotated_rect(Display *d, Window w, GC gc,
                       int x, int y, int width, int height,

                       float angle)
{
    XDrawArc(d, w, gc, x-2, y-2, 4, 4, 0, 360 * 64);

    Point pts[4] = {
        {x - width/2, y - height/2},
        {x + width/2, y - height/2},
        {x + width/2, y + height/2},
        {x - width/2, y + height/2}
    };

    for (int i = 0; i < 4; i++) {
        rotate_point(&pts[i], x, y, angle);
    }

    XPoint xpts[5];
    for (int i = 0; i < 4; i++) {
        xpts[i].x = (short)pts[i].x;
        xpts[i].y = (short)pts[i].y;
    }
    xpts[4] = xpts[0]; // close polygon

    XDrawLines(d, w, gc, xpts, 5, CoordModeOrigin);

    XDrawLine(d, w, gc, (int)pts[0].x, (int)pts[0].y, x, y);
    // Draw angle text inside the rectangle (not rotated) 
    if(0){
        char buf[64]; 
        snprintf(buf, sizeof(buf), "%3.1f", angle* (180.0 / M_PI) ); 
        // Draw text near the center 
        XDrawString(d, w, gc, x - 30, y, buf, strlen(buf));        
    }
}
// #include <X11/Xlib.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <string.h>/// old


/*
void draw_rotated_rect(Display *d, Window w, GC gc,
                       int x, int y, int width, int height,
                       double angle_deg) {
    double angle = angle_deg * M_PI / 180.0;


    double x1 = x,         y1 = y;
    double x2 = x+width,   y2 = y;
    double x3 = x+width,   y3 = y+height;
    double x4 = x,         y4 = y+height;

    double cx = x + width/2.0;
    double cy = y + height/2.0;

    rotate_point(cx, cy, angle, &x1, &y1);
    rotate_point(cx, cy, angle, &x2, &y2);
    rotate_point(cx, cy, angle, &x3, &y3);
    rotate_point(cx, cy, angle, &x4, &y4);

    XPoint pts[5] = {
        { (short)x1, (short)y1 },
        { (short)x2, (short)y2 },
        { (short)x3, (short)y3 },
        { (short)x4, (short)y4 },
        { (short)x1, (short)y1 }
    };

    XDrawLines(d, w, gc, pts, 5, CoordModeOrigin);
    XDrawArc(d, w, gc, x, y, 10, 10, 0, 360 * 64);
    x-=width/2;
    y-=height/2;
    XDrawArc(d, w, gc, x, y, 5, 5, 0, 360 * 64);
    
    
    // Draw angle text inside the rectangle (not rotated) 
    {
        char buf[64]; 
        snprintf(buf, sizeof(buf), "%3.1f", angle_deg); 
        // Draw text near the center 
        XDrawString(d, w, gc, (int)cx - 30, (int)cy, buf, strlen(buf));        
    }

}
void rotate_point(double cx, double cy, double angle,
    double *x, double *y) {
double s = sin(angle);
double c = cos(angle);

*x -= cx;
*y -= cy;

double xnew = *x * c - *y * s;
double ynew = *x * s + *y * c;

*x = xnew + cx;
*y = ynew + cy;
}


*/
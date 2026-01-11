#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <X11/Xlib.h>
#include <unistd.h>   // for usleep
#include <string.h>

//#include <box2d/box2d.h>
#include "box2d/box2d.h"

#include "../x11/draw.h"

#define BOXES 20

Display *display;
Window window;
int screen;
GC gc;

const float SCALE = 10.0;
int WorldToPixelX(float x){  return ( x*SCALE) +200;}
int WorldToPixelY(float y){  return (-y*SCALE) +400;}

void Testing(void){
	// Construct a world object, which will hold and simulate the rigid bodies.
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = (b2Vec2){0.0f, -10.0f};
	
	b2WorldId worldId = b2CreateWorld(&worldDef);

	b2BodyDef groundBodyDef = b2DefaultBodyDef();
	groundBodyDef.position = (b2Vec2){0.0f, -10.0f};

	b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);

	b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);

	b2ShapeDef groundShapeDef = b2DefaultShapeDef();
	b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);



    b2BodyDef bodyDefs[BOXES];
    b2BodyId bodyIds[BOXES];
    b2Polygon dynamicBoxs[BOXES];
    b2ShapeDef shapeDefs[BOXES];

    for(int i =0;i<BOXES;i++){

        bodyDefs[i] = b2DefaultBodyDef();
        
        bodyDefs[i].type = b2_dynamicBody;
        bodyDefs[i].position = (b2Vec2){0.0f + i*0.01 , 14.0f+ i*2.5};
        bodyDefs[i].rotation= b2MakeRot((3.14/10)*(i*0.1)  ); // angularVelocity=b2Ro; 
        bodyIds[i] = b2CreateBody(worldId, &bodyDefs[i]);
        
        b2Body_SetAngularVelocity(bodyIds[i],0*i);
        b2Body_SetLinearVelocity( bodyIds[i], (b2Vec2){0.1*i, 0} );

        dynamicBoxs[i] = b2MakeBox(1.0f, 1.0f);
        
        shapeDefs[i] = b2DefaultShapeDef();
        shapeDefs[i].density = 1.0f;
        shapeDefs[i].material.friction = 0.3f;
        b2CreatePolygonShape(bodyIds[i], &shapeDefs[i], &dynamicBoxs[i]);

    }


	float timeStep = 1.0f / 60.0f;
	int subStepCount = 4;

    
    
	for (int j = 0; j < 1000; ++j)
	{
        XClearWindow(display, window);
        
        
        //printf("Box 2D [%2d] ",i);
        b2World_Step(worldId, timeStep, subStepCount);
        for(int i=0;i<BOXES;i++){
            b2Vec2 position = b2Body_GetPosition(bodyIds[i]);
            b2Rot rotation = b2Body_GetRotation(bodyIds[i]);
            //printf("%4.2f %4.2f %4.2f ", position.x, position.y, b2Rot_GetAngle(rotation));

            draw_rotated_rect(display, window, gc,
                WorldToPixelX(position.x), WorldToPixelY( position.y),
                1*SCALE*2,  1*SCALE*2,
                b2Rot_GetAngle(rotation));
    
        }
        //printf("\n");
        // draw_rotated_rect(display, window, gc,
        //     200, 200,
        //     100,  25,
        //     j);
        XFlush(display);

        usleep(16000);  // ~60 FPS
        
    }


}


int main(void)
{
    display = XOpenDisplay(NULL);
    screen = DefaultScreen(display);
    window = XCreateSimpleWindow( display,RootWindow(display, screen),50, 50,600, 400,1,BlackPixel(display, screen),WhitePixel(display, screen));
    XMapWindow(display, window);
    gc = XCreateGC(display, window, 0, NULL);


    Testing();

    XCloseDisplay(display);

	return 0;
}

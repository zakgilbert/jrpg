
#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include "Header.h"

/**
 * render_function
 * 
 * definition of a render_function later to be passed into the render
 * Q as a function pointer
 */
typedef void render_function(void *obj, struct SDL_Renderer *renderer);

/**
 * Node
 * 
 * for the render_q
*/
struct Node
{
    render_function(*funct);
    void *obj;
    struct Node *next;
};

/**
 * Render_Q: 
 * 
 * a typedef Queue for holding render functions. 
 * 
*/
typedef struct _Render_Q
{
    /**
     *  destroy:
     * 
     *  frees the Queue.
     */
    void (*destroy)(struct _Render_Q *this);
    /**
     * render:
     * renders every object in the Q and then returns the q as it was.
     */
    struct _Render_Q *(*render)(struct _Render_Q *this, struct SDL_Renderer *renderer);
    void (*add)(struct _Render_Q *this, struct Node *data);
    struct Node *(*new_node)(void *obj, render_function target);
    struct Node *(*pop)(struct _Render_Q *this);
    struct Node *front;
    struct Node *rear;
    int size;

} Render_Q;
Render_Q *CREATE_RENDER_Q();

extern Render_Q *render_q;
Render_Q *render_q;
#endif /* RENDER_H */

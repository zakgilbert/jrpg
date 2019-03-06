//
// Created by zachary on 1/24/19.
//

#ifndef JRPG_HEADER_H
#define JRPG_HEADER_H
#include <stdint.h>
#define WINDOW_WIDTH (356)
#define WINDOW_HEIGHT (324)

extern int NUM_STATS;
extern char **STAT_MATRIX;
extern int NUM_CHARACTERS;
extern int ITEMS_IN_BAG;
extern int READY_TO_INTERACT;
extern int *ITEM_QUANTITIES;
extern char **BAG;
extern int *BAG_QUANTITIES;
extern int inputs[6];
extern int EDGE_DETECTION[4];
extern int MOVEMENT_DISABLED;
extern int IS_MOVING;
extern int X;
extern int Y;
extern int MAP_WIDTH;
extern int MAP_HEIGHT;
extern int TICK;
extern int REFRESH_ITEMS;
extern int WAITING_FOR_MESSAGE;
extern struct SDL_Color WHITE;
extern struct SDL_Color GREY;
extern struct SDL_Color GREY;
extern struct SDL_Color MENU_BACKGROUND;
extern struct SDL_Color RED;
extern struct SDL_Color BLU;
extern struct SDL_Color GRN;
/*
extern struct GLOBALS {
    int TEST;
}G; */ 

enum states
{
    DARK_FOREST,
    MAIN_MENU,
    ITEMS_MENU,
    USE_ITEM,
    MESSAGE,
    CONFIG
};
enum states state, previous_state;
enum INPUTS
{
    NONE,
    OKAY,
    QUIT,
    CANCEL
};
enum INPUTS INPUT;

enum COLLIDABLE_TYPES
{
    CHEST,
    NPC,
    ENEMY,
    BUTTON,
};

enum BUTTON
{
    ON,
    OFF
};

enum BUTTON INTERACT;



int FPS;
long double TIME_PER_TICK;
long double TIME_DELAY_PER_SECOND;
double DELTA;
uint64_t TIME_NOW;
uint64_t TIME_LAST;
uint64_t NANO_TIMER;
uint64_t TICKS_PER_SECOND;
int FRAMES_RENDERED;
int refresh_inputs(int *, int, int);
void set_items(int *);


#endif //JRPG_HEADER_H

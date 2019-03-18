//
// Created by zachary on 1/23/19.
//

#include "Player_Input.h"

int wait_for_okay()
{
    MOVEMENT_DISABLED = 0;
    while (!USER_INPUTS[4])
    {
        SDL_Delay(1);
    }
    MOVEMENT_DISABLED = 1;
    return 0;
}
int input_thread(void *data)
{
    int in_loop = 1;

    while (in_loop)
    {
        get_player_input();
        if (INPUT == QUIT)
        {
            in_loop = 0;
        }
        SDL_Delay(1);
    }
    return 0;
}
void get_player_input()
{
    union SDL_Event ev;
    // printf("\nInput: ");
    /* for (int i = 0; i < 6; i++)
    {
        printf(" %d", USER_INPUTS[i]);
    }*/

    while (SDL_PollEvent(&ev) != 0)
    {
        int anything_pressed = 0;
        switch (ev.type)
        {
        case SDL_QUIT:
            INPUT = QUIT;
            break;
        case SDL_KEYDOWN:
            switch (ev.key.keysym.scancode)
            {
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                USER_INPUTS[0] = 1;
                anything_pressed = 1;
                continue;
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                USER_INPUTS[1] = 1;
                anything_pressed = 1;
                continue;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                USER_INPUTS[2] = 1;
                anything_pressed = 1;
                continue;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                USER_INPUTS[3] = 1;
                anything_pressed = 1;
                continue;
            case SDL_SCANCODE_J:
                USER_INPUTS[4] = 1;
                INPUT = OKAY;
                continue;
            case SDL_SCANCODE_L:
                USER_INPUTS[5] = 1;
                INPUT = CANCEL;
                continue;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (ev.key.keysym.scancode)
            {
            case SDL_SCANCODE_O:
                INPUT = QUIT;
                continue;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                USER_INPUTS[0] = 0;
                continue;
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                USER_INPUTS[1] = 0;
                continue;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                USER_INPUTS[2] = 0;
                continue;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                USER_INPUTS[3] = 0;
                continue;
            case SDL_SCANCODE_J:
                USER_INPUTS[4] = 0;
                INPUT = NONE;
                continue;
            case SDL_SCANCODE_L:
                INPUT = NONE;
                USER_INPUTS[5] = 0;
                continue;
            default:
                break;
            }
            break;
        default:
            break;
        }
        if (!anything_pressed)
        {
            IS_MOVING = 0;
        }
        else
        {
            IS_MOVING = 1;
        }
    }
}
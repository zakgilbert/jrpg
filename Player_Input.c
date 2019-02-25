//
// Created by zachary on 1/23/19.
//

#include "Player_Input.h"

int input_thread (void *data)
{
    int in_loop = 1;

    while (in_loop) {
        get_player_input();

        if (INPUT == QUIT) {
            in_loop = 0;
        }
        SDL_Delay(1);
    }
    return 0;
}
void get_player_input ()
{
    union SDL_Event ev;

    while (SDL_PollEvent(&ev) != 0) {
        switch (ev.type) {
            case SDL_QUIT:
                INPUT = QUIT;
                break;
            case SDL_KEYDOWN:
                switch (ev.key.keysym.scancode) {
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        inputs[0] = 1;
                        IS_MOVING = 1;
                        break;
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        inputs[1] = 1;
                        IS_MOVING = 1;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        inputs[2] = 1;
                        IS_MOVING = 1;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        inputs[3] = 1;
                        IS_MOVING = 1;
                        break;
                    case SDL_SCANCODE_J:
                        inputs[4] = 1;
                        INPUT = OKAY;
                        break;
                    case SDL_SCANCODE_L:
                        inputs[5] = 1;
                        INPUT = CANCEL;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (ev.key.keysym.scancode) {
                    case SDL_SCANCODE_O:
                        INPUT = QUIT;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        inputs[0] = 0;
                        break;
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        inputs[1] = 0;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        inputs[2] = 0;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        inputs[3] = 0;
                        break;
                    case SDL_SCANCODE_J:
                        inputs[4] = 0;
                        break;
                    case SDL_SCANCODE_L:
                        //INPUT = NONE;
                        inputs[5] = 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}




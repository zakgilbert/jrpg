
#include "Header.h"
#include "H.h"
#include "Render.h"
#include "Words.h"
#include "Graphics.h"
#include "Floor.h"
#include "Hero.h"
#include "Item.h"
#include "Area.h"
#include "Player_Input.h"
#include "Movement.h"
#include "Window.h"
#include "Menu.h"
#include "Hand.h"
#include "Character.h"
#include "Affect.h"
#include "Message.h"
#include "Time.h"
#include "Lootable.h"
#include "Npc.h"
#include "Collision.h"
#include "Battle.h"
#include "Assets.h"
#include "Enemy.h"
#include "Battle_Q.h"
#include "Text.h"

int main(int argc, char **argv)
{
    set_up_timer(60);
    int running;
    int w, h;
    SET_GLOBALS();
    running = 1;

    create_load_info();
    refresh_inputs(USER_INPUTS, 6, 1);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        return 1;
    }
    TTF_Init();
    struct SDL_Window *window = NULL;
    struct SDL_Renderer *renderer = NULL;
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    Area *dark_forest = CREATE_AREA(DARK_FOREST);
    Hero *hero = CREATE_HERO();
    Hand *hand = CREATE_HAND();
    Item *bag = CREATE_BAG();
    Collision *game_collision = CREATE_COLLISION();
    r_Q = CREATE_RENDER_Q();

    bag = load_bag(bag, 0);

    SDL_Thread *hand_thread;
    SDL_Thread *input_thread;

    Battle *current_battle = NULL;
    Character **party = load_party(0, renderer);

    Menu *menu = CREATE_MENU(party, hand, bag);
    int dark_forest_npcs[2] = {GIGAS, SASH};
    int dark_forest_npc_types[2] = {ONE_FRAME, SPRITE};
    int dark_forest_npcs_x[2] = {400, 350};
    int dark_forest_npcs_y[2] = {400, 350};

    int dark_forest_items[2] = {POTION, ETHER};
    int dark_forest_items_x[2] = {300, 400};
    int dark_forest_items_y[2] = {300, 300};

    dark_forest->create_assets(dark_forest, renderer, game_collision, dark_forest_items, 2,
                               dark_forest_npcs, dark_forest_npc_types, 2,
                               dark_forest_items_x, dark_forest_items_y, dark_forest_npcs_x, dark_forest_npcs_y);

    hero->set_texture(hero, renderer, "graphics/locke_map.png");
    hand->create_texture(hand, "graphics/hand.png", renderer, 233, 11);

    hand_thread = SDL_CreateThread(animate_hand_thread, "animate_hand_thread", hand);
    input_thread = SDL_CreateThread(input_handler, "input_handler", NULL);
    party[0]->in_animation = -1;
    while (running)
    {
        start_timer();
        refresh_inputs(EDGE_DETECTION, 4, movement());
        game_collision->update_collidables(game_collision, state);
        set_fullscreen(window, hero);

        SDL_RenderClear(renderer);
        r_Q = r_Q->render(r_Q, renderer);
        SDL_RenderPresent(renderer);

        switch (state)
        {
        case DARK_FOREST:;
            Message *message_being_displayed = dark_forest->render_area(dark_forest, renderer, hero, bag);
            break;

        case MAIN_MENU:
            TICK = 1;
            menu->update_main_menu(menu);
            break;
            /*
        case ITEMS_MENU:
            TICK = 1;
            menu->update_items_menu(menu);
            break;
*/
            /**
            case USE_ITEM:
                TICK = 1;
                SDL_RenderClear(renderer);
                menu->render_use_item_menu(menu, renderer, hand, party, bag);
                hand->render(hand, renderer);
                SDL_RenderPresent(renderer);
                break;
    
            case CONFIG:
                TICK = 1;
                SDL_RenderClear(renderer);
                menu->render_config_menu(menu, renderer, hand);
                hand->render(hand, renderer);
                SDL_RenderPresent(renderer);
                break;
    
            case SAVE:
                TICK = 1;
                SDL_RenderClear(renderer);
                menu->render_save_menu(menu, renderer, hand);
                hand->render(hand, renderer);
                SDL_RenderPresent(renderer);
                break;

        case MESSAGE:
            if (message_being_displayed->type == ONE_LINER)
            {
                message_being_displayed->render_one_liner(message_being_displayed, renderer);
                SDL_RenderPresent(renderer);
                wait_for_okay();
                state = previous_state;
                previous_state = MESSAGE;
                message_being_displayed->destroy(message_being_displayed);
                message_being_displayed = NULL;
            }
            else if (message_being_displayed->type == DIALOGUE)
            {
                while (!message_being_displayed->render_dialogue(message_being_displayed, renderer))
                {
                    SDL_RenderPresent(renderer);
                    wait_for_okay();
                    USER_INPUTS[4] = 0;
                }
                USER_INPUTS[4] = 0;
                state = previous_state;
                previous_state = MESSAGE;
                message_being_displayed->destroy(message_being_displayed);
                message_being_displayed = NULL;
            }
            break;
        case BATTLE:
            if (current_battle == NULL)
            {
                current_battle = CREATE_BATTLE(previous_state, ROLL, renderer, party, 4);
            }

            SDL_RenderClear(renderer);
            current_battle->render(current_battle, renderer, hand);
            SDL_RenderPresent(renderer);
            if (previous_state == BATTLE)
            {
                current_battle->destroy(current_battle);
                current_battle = NULL;
                ROLL = -1;
            }
*/
            break;
        default:
            break;
        }
        TICKS++;
        FRAMES_RENDERED++;
        delay();
        reset_timer();
        running = quit();
    }
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &menu->transition);
    SDL_RenderPresent(renderer);

    SDL_WaitThread(hand_thread, NULL);
    SDL_WaitThread(input_thread, NULL);

    dark_forest->destroy(dark_forest);
    hero->destroy(hero);
    menu->destroy(menu);
    hand->destroy(hand);
    save_bag(bag, 0);
    bag->destroy(bag);

    free(STAT_MATRIX);
    party[0]->destroy_party(party);
    SDL_DestroyRenderer(renderer);
    SDL_Delay(400);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

void set_fullscreen(struct SDL_Window *window, Hero *hero)
{
    if (FULLSCREEN_ON)
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }
    else
    {
        SDL_SetWindowFullscreen(window, 0);
    }
}
int refresh_inputs(int *array, int size, int bol)
{
    if (!bol)
    {
        return 0;
    }
    int i;

    for (i = 0; i < size; i++)
    {
        array[i] = 0;
    }
    return 1;
}
int quit()
{
    if (INPUT == QUIT)
    {
        return 0;
    }
    return 1;
}

/************************
 *  Battle.c
*/

#include "Battle.h"
static void _destroy(Battle *this)
{
    if (NULL != this)
    {
        free(this);
        this = NULL;
    }
}
static void _create_battle_textures(Battle *this, struct SDL_Renderer *renderer)
{
    this->textures = malloc(sizeof(struct SDL_Texture *) * BATTLE_LINEUP[this->area]->num_ints[this->roll]);
    this->party_textures = malloc(sizeof(struct SDL_Texture *));
    for (size_t i = 0; i < this->num_enemies; i++)
    {
        this->textures[i] = create_texture(renderer, ENEMY_PATHS->list[BATTLE_LINEUP[this->area]->list[this->roll][i]], &this->rect);
    }
    for (size_t k = 0; k < this->num_party; k++)
    {
        this->party_textures[k] = create_texture(renderer, BATTLE_CHARACTER_GRAPHICS->list[k], &this->party_rect_2);
    }
    this->party_rect_1.x = 200;
    this->party_rect_1.y = 100;
    this->party_rect_2.x = 0;
    this->party_rect_2.y = 0;
    this->party_rect_1.w = SPRITE_FRAME_WIDTH;
    this->party_rect_1.h = SPRITE_FRAME_HEIGHT;
    this->party_rect_2.w = SPRITE_FRAME_WIDTH;
    this->party_rect_2.h = SPRITE_FRAME_HEIGHT;
}
static void _render(Battle *this, struct SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, this->back_ground, NULL, &this->bg_rect);
    for (size_t i = 0; i < this->num_enemies; i++)
    {
        SDL_RenderCopy(renderer, this->textures[i], NULL, &this->rect);
    }
    for(size_t k = 0; k < this->num_party; k++)
    {
        SDL_RenderCopy(renderer, this->party_textures[k], &this->party_rect_2, &this->party_rect_1);
    }
    
}

Battle *CREATE_BATTLE(int area, int roll, struct SDL_Renderer *renderer, Character **party, int num_party)
{
    Battle *this = malloc(sizeof(*this));
    this->destroy = _destroy;
    this->render = _render;
    this->create_battle_textures = _create_battle_textures;
    this->bg_rect.x = 0;
    this->bg_rect.y = 0;
    this->party = party;
    this->num_party = num_party;

    this->back_ground = create_texture(renderer, BATTLE_BACKGROUNDS->list[area], &this->bg_rect);

    this->roll = roll;
    this->area = area;
    this->rect.x = 35;
    this->rect.y = WINDOW_HEIGHT / 2;
    this->num_enemies = BATTLE_LINEUP[this->area]->num_ints[this->roll];
    this->create_battle_textures(this, renderer);
    return this;
}
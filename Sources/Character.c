/* Character.c */

#include "Character.h"

void stand_battle_ani(struct SDL_Rect *rect)
{
    rect->x = 0;
    rect->y = 0;
}
int cast_1_battle_ani(struct SDL_Rect *rect)
{
    rect->x = 32;
    rect->y = 0;
    return 0;
}
int cast_2_battle_ani(struct SDL_Rect *rect)
{
    rect->x = 32;
    rect->y = 32;
    return 0;
}
static void _destroy(Character *this)
{
    SDL_DestroyTexture(this->character_texture);
    this->character_texture = NULL;

    if (NULL != this)
    {
        free(this);
        this = NULL;
    }
}

static void _destroy_party(Character **party)
{
    int i;

    for (i = 0; i < NUM_CHARACTERS; i++)
    {
        party[i]->destroy(party[i]);
    }
    free(party);
    party = NULL;
}

static int _update_party_stats(Character **these)
{
    for (size_t i = 0; i < NUM_CHARACTERS; i++)
    {
        these[i]->check_stats(these[i]);
    }
    return 0;
}

static void _check_stats(Character *this)
{
    sprintf(this->HP.str_current, "%d", this->HP.current);
    sprintf(this->MP.str_current, "%d", this->MP.current);
    sprintf(this->EXP.str_current, "%d", this->EXP.current);
    strcat(strcat(strcat(strcpy(this->HP.display, this->HP.name), this->HP.str_current), "/"), this->HP.str_max);
    strcat(strcat(strcat(strcpy(this->MP.display, this->MP.name), this->MP.str_current), "/"), this->MP.str_max);
    strcat(strcat(strcat(strcpy(this->EXP.display, this->EXP.name), this->EXP.str_current), "/"), this->EXP.str_max);
}

static void _create_battle_textures(Character *this, struct SDL_Renderer *renderer)
{
    this->textures_party = malloc(sizeof(struct SDL_Texture *) * NUM_CHARACTERS);
    for (size_t k = 0; k < NUM_CHARACTERS; k++)
    {
        this->textures_party[k] = create_texture(renderer, BATTLE_CHARACTER_GRAPHICS->list[k], &this->party_rect_2);
    }
    this->party_rect_1.x = 240;
    this->party_rect_1.y = 90;
    this->party_rect_2.x = 0;
    this->party_rect_2.y = 0;
    this->party_rect_1.w = SPRITE_FRAME_WIDTH;
    this->party_rect_1.h = SPRITE_FRAME_HEIGHT;
    this->party_rect_2.w = SPRITE_FRAME_WIDTH;
    this->party_rect_2.h = SPRITE_FRAME_HEIGHT;
}
static int _cast(Character *this)
{
    if (!this->in_animation)
    {
        this->in_animation = 1;
        this->current_animation_frame = 0;
        this->animation_total_frames = 1000;
    }
    else if (this->current_animation_frame >= this->animation_total_frames)
    {
        this->in_animation = 0;
    }
    if (time_to_animate())
    {
        (*this->cast_ptr[this->ani_i])(&this->party_rect_2);
        this->ani_i = !(this->ani_i);
    }
    return this->in_animation;
}

static int _animate(Character *this)
{
    switch (CURRENT_CHARACTER_BATTLE_ANIMATION)
    {
    case ATK:

        break;
    case CST:

        break;
    case NOT_ANIMATING_BATTLE_CHARACTER:
        break;

    default:
        break;
    }
}
static void _render_battle_textures(Character *this, struct SDL_Renderer *renderer)
{
    int party_x, party_y;
    party_x = this->party_rect_1.x;
    party_y = this->party_rect_1.y;

    this->cast(this);
    for (size_t k = 0; k < NUM_CHARACTERS; k++)
    {
        SDL_RenderCopy(renderer, this->textures_party[k], &this->party_rect_2, &this->party_rect_1);
        this->party_rect_1.x += 15;
        this->party_rect_1.y += 30;
    }
    this->party_rect_1.x = party_x;
    this->party_rect_1.y = party_y;
}

Character *CREATE_CHARACTER(int key)
{
    Character *this = (Character *)malloc(sizeof(*this));

    this->destroy = _destroy;
    this->check_stats = _check_stats;
    this->destroy_party = _destroy_party;
    this->update_party_stats = _update_party_stats;
    this->create_battle_textures = _create_battle_textures;
    this->render_battle_textures = _render_battle_textures;
    this->animate = _animate;
    this->cast = _cast;
    this->cast_ptr[0] = cast_1_battle_ani;
    this->cast_ptr[1] = cast_2_battle_ani;
    this->key = key;
    this->num_stats = 1;
    this->in_action_queue = 0;
    this->type = PARTY_MEMBER;
    this->in_animation = 0;
    this->ani_i = 0;

    strcpy(this->HP.name, "HP: ");
    strcpy(this->MP.name, "MP: ");
    strcpy(this->EXP.name, "EXP:");

    return this;
}
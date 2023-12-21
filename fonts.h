#ifndef FONTS_H
#define FONTS_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

struct Message {
    TTF_Font *font;
    int font_ptsize;
    SDL_Color text_color;
    SDL_Color text_bg_color;
    SDL_Texture *text;
    SDL_Rect text_rect;
};

struct Message *fonts_message_create(void);
int fonts_set_font(struct Message *message, char *font_path);
int fonts_set_message(SDL_Renderer *renderer, struct Message *message, char *msg);
void fonts_set_dimensions(struct Message *message, SDL_Surface *surface);
void fonts_set_position(struct Message *message, int xpos, int ypos);
void fonts_message_destroy(struct Message *message);
void fonts_render(SDL_Renderer *renderer, struct Message *message);

#endif /* FONTS_H */
#include "fonts.h"

struct Message *fonts_message_create(void)
{
    struct Message *msg_out = malloc(sizeof(struct Message));
    msg_out->font = NULL;
    msg_out->font_ptsize = 30;
    msg_out->text = NULL;
    msg_out->text_color = (struct SDL_Color) { 0xff, 0xff, 0xff, 0xff };
    msg_out->text_bg_color = (struct SDL_Color) { 0x00, 0x00, 0x00, 0xff };
    fonts_set_font(msg_out, "assets/vinasans.ttf");
    return msg_out;
}

void fonts_message_destroy(struct Message *message)
{
    free(message);
}

int fonts_set_font(struct Message *message, char *font_path)
{
    message->font = TTF_OpenFont(font_path, message->font_ptsize);
    if (message->font == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "could not open font: %s\n",
                SDL_GetError());
        return -1;
    }
    return 0;
}

int fonts_set_message(SDL_Renderer *renderer, struct Message *message, char *msg)
{
    // create surface
    SDL_Surface *text_surface = TTF_RenderText_Shaded(message->font, msg, 
            message->text_color, message->text_bg_color);
    if (!text_surface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "could not render text surface: %s\n",
                TTF_GetError());
        return -1;
    } 

    // create texture from surface pixels
    message->text = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!message->text)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "could not create texture: %s\n",
                TTF_GetError());
        return -2;
    }

    fonts_set_dimensions(message, text_surface);
    fonts_set_position(message, message->text_rect.x, message->text_rect.y);
    SDL_FreeSurface(text_surface);

    return 0;
}

void fonts_set_dimensions(struct Message *message, SDL_Surface *surface)
{
    message->text_rect.w = surface->w;
    message->text_rect.h = surface->h;
}

void fonts_set_position(struct Message *message, int xpos, int ypos)
{
    message->text_rect.x = xpos;
    message->text_rect.y = ypos;
}

void fonts_render(SDL_Renderer *renderer, struct Message *message)
{
    SDL_SetRenderDrawColor(renderer, message->text_color.r, message->text_color.g, 
            message->text_color.b, message->text_color.a);

    SDL_RenderFillRect(renderer, &message->text_rect);
    SDL_RenderCopy(renderer, message->text, NULL, &message->text_rect);

}
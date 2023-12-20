#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "object.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define NUM_OBJECTS 3

#define BALL_SPEED 10
#define PADDLE_SPEED 10

#define FONT_PTSIZE 20

struct Game
{
    struct Object *objects[NUM_OBJECTS];
    int ball_velocity_x;
    int ball_velocity_y;
    float ball_angle;
    int player1_score;
    int player2_score;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    TTF_Font *font;
    
    bool is_running;
};

int game_init(struct Game *game);
int game_create_window(struct Game *game, char *title, int width, int height, bool fullscreen);
int game_create_objects(struct Game *game);
void game_destroy_objects(struct Game *game);
void game_handle_events(struct Game *game);
void game_update(struct Game *game);
void game_render(struct Game *game);
void game_clean(struct Game *game);
void game_new_round(struct Game *game);
bool game_is_running(struct Game *game);
void game_set_is_running(struct Game *game, bool is_running);
int game_set_font(struct Game *game);

#endif /* GAME_H */
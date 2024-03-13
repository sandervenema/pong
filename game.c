#include <time.h>
#include <math.h>

#include "game.h"
#include "util.h"
#include "fonts.h"

# define M_PI 3.14159265358979323846

int game_init(struct Game *game)
{  
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't initialise SDL: %s",
                SDL_GetError());
        return 3;
    }

    if (TTF_Init() < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't initialize SDL TTF: %s\n",
                SDL_GetError());
        return 4;
    }

    game->is_running = true;
    game->ball_velocity_x = BALL_SPEED;
    game->ball_velocity_y = BALL_SPEED;
    game->score = fonts_message_create();
    game->score_str = malloc(sizeof(char *));
    fonts_set_position(game->score, WINDOW_WIDTH / 2, 10);

    srand(time(NULL));
    return 0;
}

int game_create_window(struct Game *game, char *title, int width, int height, bool fullscreen)
{
    Uint32 flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_CreateWindowAndRenderer(width, height, flags,
		&game->window, &game->renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't create window and renderer: %s",
                SDL_GetError());
        return 3;
    }
    fonts_set_message(game->renderer, game->score, "0 | 0");
    SDL_SetWindowTitle(game->window, title);
    return 0;
}

int game_toggle_window_fullscreen(struct Game *game)
{
    bool is_fullscreen = SDL_GetWindowFlags(game->window) & SDL_WINDOW_FULLSCREEN;
    return SDL_SetWindowFullscreen(game->window, !is_fullscreen);
}

int game_create_objects(struct Game *game)
{    
    struct Resource {
        char *url;
        int xpos;
        int ypos;
    };

    struct Resource resources[NUM_OBJECTS] = {
        {.url = "assets/ball.png", .xpos = WINDOW_WIDTH/2, .ypos = WINDOW_HEIGHT/2},
        {.url = "assets/paddle.png", .xpos = 10, .ypos = WINDOW_HEIGHT/2},
        {.url = "assets/paddle.png", .xpos = WINDOW_WIDTH-42, .ypos = WINDOW_HEIGHT/2}
    };

    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        game->objects[i] = object_create(resources[i].url, game->renderer);
        if (game->objects[i] == NULL)
        {
            printf("ERROR: asset missing: %s\n", resources[i].url);
            game_set_is_running(game, false);
            return -1;
        }
        game->objects[i]->dest_rect.x = resources[i].xpos;
        game->objects[i]->dest_rect.y = resources[i].ypos - (game->objects[i]->dest_rect.h/2);
    }
    return 0;
}

void game_destroy_objects(struct Game *game)
{
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        object_destroy(game->objects[i]);
    }
}

void game_handle_events(struct Game *game)
{
    SDL_PollEvent(&game->event);
    switch (game->event.type)
    {
    case SDL_QUIT:
        game_set_is_running(game, false);
        break;
    default:
        break;
    }

    SDL_Rect *paddle1_pos = &game->objects[1]->dest_rect;
    SDL_Rect *paddle2_pos = &game->objects[2]->dest_rect;

    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_UP])
    {
        if (paddle1_pos->y > 0)
        {
            paddle1_pos->y -= PADDLE_SPEED;
        }
    }

    if (keystates[SDL_SCANCODE_DOWN])
    {
        if (paddle1_pos->y < WINDOW_HEIGHT - paddle1_pos->h)
        {
            paddle1_pos->y += PADDLE_SPEED;
        }
    }

    if (keystates[SDL_SCANCODE_W])
    {
        if (paddle2_pos->y > 0)
        {
            paddle2_pos->y -= PADDLE_SPEED;
        }
    }

    if (keystates[SDL_SCANCODE_S])
    {
        if (paddle2_pos->y < WINDOW_HEIGHT - paddle2_pos->h)
        {
            paddle2_pos->y += PADDLE_SPEED;
        }
    }

    if (keystates[SDL_SCANCODE_R])
    {
        game_new_round(game, true);
    }

    if (keystates[SDL_SCANCODE_Q])
    {
        game_set_is_running(game, false);
    }

    if (keystates[SDL_SCANCODE_F])
    {
        game_toggle_window_fullscreen(game);
    }
}

void game_update_score(struct Game *game)
{
    sprintf(game->score_str, "%d | %d", game->player1_score, game->player2_score);
    fonts_set_message(game->renderer, game->score, game->score_str);
}

void game_update(struct Game *game)
{
    SDL_Rect *ball_pos = &game->objects[0]->dest_rect;
    // move ball
    ball_pos->x += game->ball_velocity_x * cos(game->ball_angle);
    ball_pos->y += game->ball_velocity_y * sin(game->ball_angle);

    // ball needs to stay within screen boundaries
    // left edge
    if (ball_pos->x < 0)
    {
        game->player2_score++;
        game_update_score(game);
        game_new_round(game, false);
    }

    // right edge
    if (ball_pos->x > WINDOW_WIDTH - ball_pos->w)
    {
        game->player1_score++;
        game_update_score(game);
        game_new_round(game, false);
    }

    // upper edge or lower edge: bounce ball back in opposite direction
    if (ball_pos->y < 0 || ball_pos->y > WINDOW_HEIGHT - ball_pos->h)
    {
        game->ball_velocity_y *= -1;
    }

    // collision detection: bounce ball off in opposite direction
    if (objects_collide(game->objects[0], game->objects[1])
        || objects_collide(game->objects[0], game->objects[2]))
    {
        game->ball_velocity_x *= -1;
        game->ball_velocity_y *= -1;
    }
}

void game_render(struct Game *game)
{
    SDL_RenderClear(game->renderer);
    SDL_SetRenderDrawColor(game->renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderFillRect(game->renderer, NULL);

    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        SDL_RenderCopy(game->renderer, game->objects[i]->texture, NULL, &game->objects[i]->dest_rect);
    }

    fonts_render(game->renderer, game->score);

    SDL_RenderPresent(game->renderer);
}

void game_clean(struct Game *game)
{
    free(game->score_str);
    game_destroy_objects(game);
    fonts_message_destroy(game->score);
    SDL_DestroyRenderer(game->renderer);
    TTF_Quit();
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

void game_new_round(struct Game *game, bool restart)
{
    if (restart)
    {
        fonts_set_message(game->renderer, game->score, "0 | 0");
        game->player1_score = 0;
        game->player2_score = 0;
    }
    game->ball_angle = random_between(0, 2*M_PI);
    SDL_Rect *ball_pos = &game->objects[0]->dest_rect;
    ball_pos->x = WINDOW_WIDTH/2;
    ball_pos->y = WINDOW_HEIGHT/2;
}

bool game_is_running(struct Game *game)
{
    return game->is_running;
}

void game_set_is_running(struct Game *game, bool is_running)
{
    game->is_running = is_running;
}

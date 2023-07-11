#include <time.h>
#include <math.h>

#include "game.h"
#include "util.h"

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

    game->is_running = true;
    game->ball_velocity_x = BALL_SPEED;
    game->ball_velocity_y = BALL_SPEED;

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
    SDL_SetWindowTitle(game->window, title);
    return 0;
}

void game_create_objects(struct Game *game)
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
        if (game->objects[i] == NULL) continue;
        game->objects[i]->dest_rect.x = resources[i].xpos;
        game->objects[i]->dest_rect.y = resources[i].ypos - (game->objects[i]->dest_rect.h/2);
    }
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
        game_new_round(game);
    }
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
        printf("scores: p1=%d p2=%d\n", game->player1_score, game->player2_score);
        game_new_round(game);
    }

    // right edge
    if (ball_pos->x > WINDOW_WIDTH - ball_pos->w)
    {
        game->player1_score++;
        printf("scores: p1=%d p2=%d\n", game->player1_score, game->player2_score);
        game_new_round(game);
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

    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        SDL_RenderCopy(game->renderer, game->objects[i]->texture, NULL, &game->objects[i]->dest_rect);
    }

    SDL_RenderPresent(game->renderer);
}

void game_clean(struct Game *game)
{
    game_destroy_objects(game);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

void game_new_round(struct Game *game)
{
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

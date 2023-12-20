#include "game.h"

int main(void)
{
    const int fps = 60;
    const Uint32 time_per_frame_ms = 1000 / fps;
    Uint32 start_time, end_time, delta;

    struct Game game = {0};
    game_init(&game);
    if (game_set_font(&game) == -1) {
        game_clean(&game);
        return -1;
    }
    game_create_window(&game, "Pong", WINDOW_WIDTH, WINDOW_HEIGHT, false);
    if (game_create_objects(&game) == -1)
    {
        game_clean(&game);
        return -1;
    }
    game_new_round(&game);

    while (game_is_running(&game))
    {
        start_time = SDL_GetTicks();
        game_handle_events(&game);
        game_update(&game);
        game_render(&game);
        end_time = SDL_GetTicks();
        delta = end_time - start_time;
        if (delta < time_per_frame_ms)
        {
            SDL_Delay(time_per_frame_ms - delta);
        }
    }

    game_clean(&game);
    return 0;
}

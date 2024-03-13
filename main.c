#include <getopt.h>
#include "game.h"

int main(int argc, char *argv[])
{
    const int fps = 60;
    const Uint32 time_per_frame_ms = 1000 / fps;
    Uint32 start_time, end_time, delta;

    bool fullscreen = false;
    if (argc > 1)
    {
        char c;
        while ((c = getopt(argc, argv, "f")) != -1)
        {
            switch (c)
            {
            case 'f':
                fullscreen = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-f]\n\n", argv[0]);
                fprintf(stderr, "\t-f\trender window in full screen\n");
                return -1;
            }
        }
    }

    struct Game game = {0};
    game_init(&game);
    game_create_window(&game, "Pong", WINDOW_WIDTH, WINDOW_HEIGHT, fullscreen);
    if (game_create_objects(&game) == -1)
    {
        game_clean(&game);
        return -1;
    }
    game_new_round(&game, true);

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

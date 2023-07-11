#ifndef OBJECT_H
#define OBJECT_H

#include "SDL2/SDL.h"

struct Object
{
    char *file_path;
    SDL_Texture *texture;
    SDL_Rect dest_rect;
};

struct Object *object_create(char *file_path, SDL_Renderer *renderer);
void object_destroy(struct Object *object);
SDL_bool objects_collide(struct Object *obj1, struct Object *obj2);

#endif /* OBJECT_H */
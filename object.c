#include "object.h"

#include "SDL2/SDL_image.h"

struct Object *object_create(char *file_path, SDL_Renderer *renderer)
{
    struct Object *obj_out = malloc(sizeof(struct Object));
    SDL_Surface *surface = IMG_Load(file_path);
    if (surface == NULL) return NULL;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
    if (tex == NULL) return NULL;
    obj_out->texture = tex;
    SDL_QueryTexture(obj_out->texture, NULL, NULL, &obj_out->dest_rect.w, &obj_out->dest_rect.h);
    obj_out->dest_rect.x = 0;
    obj_out->dest_rect.y = 0;
    SDL_FreeSurface(surface);
    return obj_out;
}

void object_destroy(struct Object *object)
{
    free(object);
}

SDL_bool objects_collide(struct Object *obj1, struct Object *obj2)
{
    return SDL_HasIntersection(&obj1->dest_rect, &obj2->dest_rect);
}
#ifndef SHAPES_H
#define SHAPES_H

#include <SDL3/SDL.h>

/*
 * SDL has no circle primitive: these build circles out of triangles and
 * send them in one SDL_RenderGeometry call each.
 */
void shape_circle(SDL_Renderer *r, SDL_FPoint center, float radius,
                  SDL_FColor color);
/* Outline only, `thickness` pixels drawn inward from `radius`. */
void shape_ring(SDL_Renderer *r, SDL_FPoint center, float radius,
                float thickness, SDL_FColor color);

#endif

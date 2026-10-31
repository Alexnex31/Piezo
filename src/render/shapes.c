#include "render/shapes.h"

#define SEGMENTS 48     /* enough to look round at note size */

static SDL_Vertex vertex(SDL_FPoint center, float radius, int i,
                         SDL_FColor color)
{
    float angle = 2.0f * SDL_PI_F * i / SEGMENTS;
    SDL_Vertex v = { { center.x + SDL_cosf(angle) * radius,
                       center.y + SDL_sinf(angle) * radius },
                     color, { 0, 0 } };

    return v;
}

/* A fan of triangles around the center: (center, i, i + 1). */
void shape_circle(SDL_Renderer *r, SDL_FPoint center, float radius,
                  SDL_FColor color)
{
    SDL_Vertex v[SEGMENTS + 1];
    int idx[SEGMENTS * 3];
    int i;

    v[0] = (SDL_Vertex){ center, color, { 0, 0 } };
    for (i = 0; i < SEGMENTS; i++) {
        v[i + 1] = vertex(center, radius, i, color);
        idx[i * 3] = 0;
        idx[i * 3 + 1] = i + 1;
        idx[i * 3 + 2] = (i + 1) % SEGMENTS + 1;
    }
    SDL_RenderGeometry(r, NULL, v, SEGMENTS + 1, idx, SEGMENTS * 3);
}

/* Outer and inner points alternate; each segment is a quad of 2 triangles. */
void shape_ring(SDL_Renderer *r, SDL_FPoint center, float radius,
                float thickness, SDL_FColor color)
{
    SDL_Vertex v[SEGMENTS * 2];
    int idx[SEGMENTS * 6];
    int i;
    int next;

    for (i = 0; i < SEGMENTS; i++) {
        v[i * 2] = vertex(center, radius, i, color);
        v[i * 2 + 1] = vertex(center, radius - thickness, i, color);
    }
    for (i = 0; i < SEGMENTS; i++) {
        next = (i + 1) % SEGMENTS;
        idx[i * 6] = i * 2;
        idx[i * 6 + 1] = i * 2 + 1;
        idx[i * 6 + 2] = next * 2;
        idx[i * 6 + 3] = i * 2 + 1;
        idx[i * 6 + 4] = next * 2 + 1;
        idx[i * 6 + 5] = next * 2;
    }
    SDL_RenderGeometry(r, NULL, v, SEGMENTS * 2, idx, SEGMENTS * 6);
}

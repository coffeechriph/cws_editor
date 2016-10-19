#ifndef TERRAINEDIT_H
#define TERRAINEDIT_H
#include "./gfx/video.h"
#include "./scene/scene.h"
#include "./gui/gui.h"
#include "./util/cws_array.h"

extern cwsMaterial terrainMaterial;
extern cwsGuiSurface *terrainEditSurface;

typedef struct 
{
    i32 width,depth;
    cwsMesh mesh;
    cwsRenderer *renderer;

    cws_array(f32, vertices);
    cws_array(i32, indices);

}Terrain;

typedef enum
{
    BRUSH_CIRCLE,
    BRUSH_RECT,
    BRUSH_SMOOTHER,
    BRUSH_PAINT
} BrushTypes;

struct 
{
    f32 strength;
    i32 brush_size;
    i32 brush_type;
} TerrainEditSettings;

void init_terrainEdit();
void update_terrain_edit(vec2 xz);
void destroy_terrain_edit();

Terrain *new_terrain(i32 width, i32 depth);
void update_terrain(Terrain *t);

void terrain_edit(vec2 p1, vec2 p2);
#endif
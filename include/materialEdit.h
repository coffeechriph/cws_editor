#ifndef MATERIAL_EDIT_H
#define MATERIAL_EDIT_H
#include "./gui/gui.h"
#include "./gfx/video.h"
#include "./scene/scene.h"

extern cwsGuiSurface *materialEditSurface;
extern cwsGuiButton *loadTextureBtn, *loadVertexShader, *loadFragmentShader;
extern cwsGuiSlider *materialColorR, *materialColorG, *materialColorB;

void create_material_edit();
void update_material_edit();
void destroy_material_edit();

#endif
#include "materialEdit.h"

cwsGuiSurface *materialEditSurface;
cwsGuiButton *loadTextureBtn, *loadVertexShader, *loadFragmentShader;
cwsGuiSlider *materialColorR, *materialColorG, *materialColorB;

void create_material_edit()
{
    vec2 scrsz = cwsScreenSize();
    materialEditSurface = cwsNewSurface(NULL);
    materialEditSurface->transform->pos = (vec3){.x = 0, .y = scrsz.y - 400};
    materialEditSurface->transform->size = (vec2){.x = 200, .y = 400};
    materialEditSurface->renderer->fill = true;
    
    loadTextureBtn = cwsSurfaceAddButton(materialEditSurface);
    loadTextureBtn->pos = (vec2){.x = 3, .y = 2}; 
    loadTextureBtn->size = (vec2){.x = 194, .y = 24};
    cwsRebuildText(loadTextureBtn->text->context, loadTextureBtn->text, "Load Texture!");
    
    loadVertexShader = cwsSurfaceAddButton(materialEditSurface);
    loadVertexShader->pos = (vec2){.x = 3, .y = 28}; 
    loadVertexShader->size = (vec2){.x = 194, .y = 24};
    cwsRebuildText(loadVertexShader->text->context, loadVertexShader->text, "Load Vertex Shader!");
    
    loadFragmentShader = cwsSurfaceAddButton(materialEditSurface);
    loadFragmentShader->pos = (vec2){.x = 3, .y = 54}; 
    loadFragmentShader->size = (vec2){.x = 194, .y = 24};
    cwsRebuildText(loadFragmentShader->text->context, loadFragmentShader->text, "Load Fragment Shader!");
    
    materialColorR = cwsSurfaceAddSlider(materialEditSurface);
    materialColorR->pos = (vec2){.x = 3, .y = 94};
    materialColorR->size = (vec2){194, 20};
    materialColorR->min = 0;
    materialColorR->max = 255;
    materialColorR->value = 255;
    cwsRebuildText(materialColorR->text->context, materialColorR->text, "255");
    
    materialColorG = cwsSurfaceAddSlider(materialEditSurface);
    materialColorG->pos = (vec2){.x = 3, .y = 126};
    materialColorG->size = (vec2){194, 20};
    materialColorG->min = 0;
    materialColorG->max = 255;
    materialColorG->value = 255;
    cwsRebuildText(materialColorG->text->context, materialColorG->text, "255");
    
    materialColorB = cwsSurfaceAddSlider(materialEditSurface);
    materialColorB->pos = (vec2){.x = 3, .y = 158};
    materialColorB->size = (vec2){194, 20};
    materialColorB->min = 0;
    materialColorB->max = 255;
    materialColorB->value = 255;
    cwsRebuildText(materialColorB->text->context, materialColorB->text, "255");
    cwsRefreshSurface(materialEditSurface);
    cwsShowSurface(materialEditSurface, false);
}

void update_material_edit()
{
}

void destroy_material_edit()
{
    
}

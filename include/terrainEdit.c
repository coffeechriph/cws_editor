#include "terrainEdit.h"
cwsMaterial terrainMaterial;
cwsTexture2D terrainTexture;
cws_array(Terrain*, terrains);

cwsGuiSurface *terrainEditSurface;
cwsGuiSlider *strengthSlider;
cwsGuiSlider *brushSizeSlider;
cwsGuiSlider *limitHeightSlider;
cwsGuiCheckbox *limitHeightBox;

Terrain *dummy_terrain;

i32 brushStrength = 1;
i32 brushSize = 1;

void init_terrainEdit()
{
    vec2 scrsz = cwsScreenSize();
    terrainEditSurface = cwsNewSurface(NULL);
    terrainEditSurface->transform->pos = (vec3){.x = 0, .y = scrsz.y - 400};
    terrainEditSurface->transform->size = (vec2){.x = 200, .y = 400};
    terrainEditSurface->renderer->fill = true;
  
    strengthSlider = cwsSurfaceAddSlider(terrainEditSurface);
    strengthSlider->pos = (vec2){.x = 10, .y = 2};
    strengthSlider->size = (vec2){.x = 180, .y = 25};
    strengthSlider->min = -10;
    strengthSlider->max = 10;
    strengthSlider->value = 1;
    cwsRebuildText(strengthSlider->text->context, strengthSlider->text, "1");
    
    brushSizeSlider = cwsSurfaceAddSlider(terrainEditSurface);
    brushSizeSlider->pos = (vec2){.x = 10, .y = 30};
    brushSizeSlider->size = (vec2){.x = 180, .y = 25};
    brushSizeSlider->min = 1;
    brushSizeSlider->max = 10;
    brushSizeSlider->value = 1;
    cwsRebuildText(brushSizeSlider->text->context, brushSizeSlider->text, "1");
    
    limitHeightBox = cwsSurfaceAddCheckbox(terrainEditSurface);
    limitHeightBox->pos = (vec2){.x = 10, .y = 60};
    limitHeightBox->size = (vec2){.x = 100, .y = 25};
    cwsRebuildText(limitHeightBox->text->context, limitHeightBox->text, "Limit height");
    
    limitHeightSlider = cwsSurfaceAddSlider(terrainEditSurface);
    limitHeightSlider->pos = (vec2){.x = 10, .y = 90};
    limitHeightSlider->size = (vec2){.x = 180, .y = 25};
    limitHeightSlider->min = 0;
    limitHeightSlider->max = 100;
    limitHeightSlider->value = 1;
    cwsRebuildText(limitHeightSlider->text->context, limitHeightSlider->text, "1");
    
    cwsRefreshSurface(terrainEditSurface);
    cwsShowSurface(terrainEditSurface, false);
    
    cwsMaterialInit(terrainMaterial);
    cwsShaderFromfile(&terrainMaterial.shader, "./data/shaders/grid_v", "./data/shaders/grid_f");
    cwsTextureFromfile(&terrainTexture, "./data/gfx/tgrid.png", IF_LINEAR_MIP_LINEAR);
    cwsMaterialAddTexture(&terrainMaterial, terrainTexture);
    
    //Add dummy terrain
    dummy_terrain = new_terrain(128,128);
    update_terrain(dummy_terrain);
}

void update_terrain_edit()
{
    if((strengthSlider->event_flags & EVENT_SLIDER_CHANGED))
    {
        brushStrength = strengthSlider->value;
    }
    
    if((brushSizeSlider->event_flags & EVENT_SLIDER_CHANGED))
    {
        brushSize = brushSizeSlider->value;
    }
}

Terrain *new_terrain(i32 width, i32 depth)
{
    Terrain *t = malloc(sizeof(Terrain));
    
    i32 attribs[] = {3, 3, 2, 3};
    cwsEmptyMesh(&t->mesh, attribs, 4);
   
    t->renderer = cwsNewRenderer(&terrainMaterial, &t->mesh);    
    t->renderer->position = (vec3){-width/2,0.5,-depth/2};
    t->width = width;
    t->depth = depth;
    
    cws_array_init(f32, t->vertices, (width*depth)*11);
    cws_array_init(i32, t->indices, (width*depth)*6);
    
    f32 x = 0, z = 0;
    for(i32 i = 0; i < width*depth; ++i)
    {
        cws_array_push(t->vertices, x);
        cws_array_push(t->vertices, 0);
        cws_array_push(t->vertices, z);
        
        cws_array_push(t->vertices,0);
        cws_array_push(t->vertices,1);
        cws_array_push(t->vertices,0);
        
        cws_array_push(t->vertices,x);
        cws_array_push(t->vertices,z);
        
        cws_array_push(t->vertices,1.0f);
        cws_array_push(t->vertices,1.0f);
        cws_array_push(t->vertices,1.0f);
        
        x += 1;
        if(x >= width)
        {
            x = 0;
            z += 1;
        }
    }
    
    x = 0;
    z = 0;
    
    for(i32 j = 0; j < width*depth; ++j)
    {
        if(x < width-1 && z < depth - 1)
        {
            i32 ax = x + width * z;
            cws_array_push(t->indices,ax);
            cws_array_push(t->indices,ax+width);
            cws_array_push(t->indices,ax+1);
            cws_array_push(t->indices,ax+1);
            cws_array_push(t->indices,ax+width);
            cws_array_push(t->indices,ax+width+1);
         }
        
        x += 1;
        if(x >= width)
        {
            z += 1;
            x = 0;
        }
    }
    
    return t;
}

void update_terrain(Terrain *t)
{
    cwsFillMesh(&t->mesh, t->vertices.data, t->vertices.length, t->indices.data, t->indices.length);
}

void terrain_raise(Terrain *t, vec2 xz)
{
    if(xz.x < 0 || xz.x >= t->width ||
       xz.y < 0 || xz.y >= t->depth)
    {
        return;
    }
    
    if(brushSize > 1)
    {
        for(i32 x = xz.x - brushSize/2; x < xz.x + brushSize/2; ++x)
        {
            for(i32 z = xz.y - brushSize/2; z < xz.y + brushSize/2; ++z)
            {
                if(x < 0 || x >= t->width ||
                   z < 0 || z >= t->depth)
                {
                    continue;
                }            
                
                i32 i = (i32)x + t->width * (i32)z;
                
                if(!limitHeightBox->checked)
                {
                    t->vertices.data[i*11+1] += ((f32)brushStrength) * 0.1f;
                }
                else if(t->vertices.data[i*11+1] < limitHeightSlider->value)
                {
                    t->vertices.data[i*11+1] += ((f32)brushStrength) * 0.1f;
                }
                else
                {
                    t->vertices.data[i*11+1] = limitHeightSlider->value;
                }
            }
        }
    }
    else
    {
        i32 i = (i32)xz.x + t->width * (i32)xz.y;
        if(!limitHeightBox->checked)
        {
            t->vertices.data[i*11+1] += ((f32)brushStrength) * 0.1f;
        }
        else if(t->vertices.data[i*11+1] < limitHeightSlider->value)
        {
            t->vertices.data[i*11+1] += ((f32)brushStrength) * 0.1f;
        }
        else
        {
            t->vertices.data[i*11+1] = limitHeightSlider->value;
        }
    }
    
    update_terrain(t);
}
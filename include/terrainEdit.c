#include "terrainEdit.h"
cwsMaterial terrainMaterial;
cwsTexture2D grassTex, dirtTex, rockTex;
cws_array(Terrain*, terrains);

cwsGuiSurface *terrainEditSurface;
cwsGuiButton *newTerrainBtn;
cwsGuiSlider *strengthSlider;
cwsGuiSlider *brushSizeSlider;
cwsGuiSlider *limitHeightSlider;
cwsGuiCheckbox *limitHeightBox;
cwsGuiSlider *brushTypeSlider;
cwsGuiSlider *brushTextureSlider;
cwsText *brushTypeText, *strengthText, *brushSizeText, *heightLimitText, *brushTextureText;

i32 brushType = BRUSH_CIRCLE;
f32 brushStrength = 0.01f;
i32 brushSize = 1;

void init_terrainEdit()
{
    vec2 scrsz = cwsScreenSize();
    terrainEditSurface = cwsNewSurface(NULL);
    terrainEditSurface->transform->pos = (vec3){.x = 0, .y = scrsz.y - 400};
    terrainEditSurface->transform->size = (vec2){.x = 200, .y = 400};
    terrainEditSurface->renderer->fill = true;
  
    newTerrainBtn = cwsSurfaceAddButton(terrainEditSurface);
    newTerrainBtn->pos = (vec2){.x = 10, .y = 10};
    newTerrainBtn->size = (vec2){.x = 100, .y = 20};
    cwsRebuildText(newTerrainBtn->text->context, newTerrainBtn->text, "New terrain");
    
    strengthSlider = cwsSurfaceAddSlider(terrainEditSurface);
    strengthSlider->pos = (vec2){.x = 10, .y = 55};
    strengthSlider->size = (vec2){.x = 180, .y = 25};
    strengthSlider->min = -20;
    strengthSlider->max = 20;
    strengthSlider->value = 1;
    cwsRebuildText(strengthSlider->text->context, strengthSlider->text, "1");
    
    brushSizeSlider = cwsSurfaceAddSlider(terrainEditSurface);
    brushSizeSlider->pos = (vec2){.x = 10, .y = 100};
    brushSizeSlider->size = (vec2){.x = 180, .y = 25};
    brushSizeSlider->min = 1;
    brushSizeSlider->max = 10;
    brushSizeSlider->value = 1;
    cwsRebuildText(brushSizeSlider->text->context, brushSizeSlider->text, "1");
    
    limitHeightBox = cwsSurfaceAddCheckbox(terrainEditSurface);
    limitHeightBox->pos = (vec2){.x = 10, .y = 145};
    limitHeightBox->size = (vec2){.x = 100, .y = 22};
    cwsRebuildText(limitHeightBox->text->context, limitHeightBox->text, "Limit height");
    
    limitHeightSlider = cwsSurfaceAddSlider(terrainEditSurface);
    limitHeightSlider->pos = (vec2){.x = 10, .y = 190};
    limitHeightSlider->size = (vec2){.x = 180, .y = 25};
    limitHeightSlider->min = 0;
    limitHeightSlider->max = 100;
    limitHeightSlider->value = 0;
    cwsRebuildText(limitHeightSlider->text->context, limitHeightSlider->text, "0");
    
    brushTypeSlider = cwsSurfaceAddSlider(terrainEditSurface);
    brushTypeSlider->pos = (vec2){.x = 10, .y = 237};
    brushTypeSlider->size = (vec2){.x = 180, .y = 25};
    brushTypeSlider->min = 0;
    brushTypeSlider->max = 3;
    brushTypeSlider->value = 0;
    cwsRebuildText(brushTypeSlider->text->context, brushTypeSlider->text, "0");
    
    brushTextureSlider = cwsSurfaceAddSlider(terrainEditSurface);
    brushTextureSlider->pos = (vec2){.x = 10, .y = 284};
    brushTextureSlider->size = (vec2){.x = 180, .y = 25};
    brushTextureSlider->min = 0;
    brushTextureSlider->max = 2;
    brushTextureSlider->value = 0;
    cwsRebuildText(brushTextureSlider->text->context, brushTextureSlider->text, "0");
    
    brushTypeText = cwsSurfaceAddText(terrainEditSurface, (vec2){.x = 10, .y = 219}, (vec2){.x = 0.5f, .y = 0.5f}, "Brush Type: Circle");
    strengthText = cwsSurfaceAddText(terrainEditSurface, (vec2){.x = 10, .y = 37}, (vec2){.x = 0.5f, .y = 0.5f}, "Brush Strength");
    brushSizeText = cwsSurfaceAddText(terrainEditSurface, (vec2){.x = 10, .y = 82}, (vec2){.x = 0.5f, .y = 0.5f}, "Brush Size");
    heightLimitText = cwsSurfaceAddText(terrainEditSurface, (vec2){.x = 10, .y = 172}, (vec2){.x = 0.5f, .y = 0.5f}, "Height Limit");
    brushTextureText = cwsSurfaceAddText(terrainEditSurface, (vec2){.x = 10, .y = 266}, (vec2){.x = 0.5f, .y = 0.5f}, "Brush Texture");
    
    cwsRefreshSurface(terrainEditSurface);
    cwsShowSurface(terrainEditSurface, false);
    
    cwsMaterialInit(terrainMaterial);
    cwsShaderFromfile(&terrainMaterial.shader, "./data/shaders/terrain_v", "./data/shaders/terrain_f", NULL);
    cwsTextureFromfile(&grassTex, "./data/gfx/grass.png", IF_LINEAR_MIP_LINEAR);
    cwsTextureFromfile(&dirtTex, "./data/gfx/dirt.png", IF_LINEAR_MIP_LINEAR);
    cwsTextureFromfile(&rockTex, "./data/gfx/rock.png", IF_LINEAR_MIP_LINEAR);
    cwsMaterialAddTexture(&terrainMaterial, grassTex);
    cwsMaterialAddTexture(&terrainMaterial, dirtTex);
    cwsMaterialAddTexture(&terrainMaterial, rockTex);
    cwsShaderCreateUniform(&terrainMaterial.shader, "cursor");
    terrainMaterial.rflags = RF_CULLING_ENABLED;
}

const char *BRUSH_NAMES[4] = {"Brush Type: Circle", "Brush Type: Rect", "Brush Type: Smoother", "Brush Type: Paint"};
void update_terrain_edit(vec2 xz)
{
    if(newTerrainBtn->event_flags & EVENT_CLICKED)
    {
        Terrain *t = new_terrain(64,64);
        update_terrain(t);
        cws_array_push(terrains, t);
    }
    
    if((strengthSlider->event_flags & EVENT_SLIDER_CHANGED))
    {
        brushStrength = (f32)strengthSlider->value * 0.01f;
    }
    
    if((brushSizeSlider->event_flags & EVENT_SLIDER_CHANGED))
    {
        brushSize = brushSizeSlider->value;
    }
    
    if((brushTypeSlider->event_flags & EVENT_SLIDER_CHANGED))
    {
        brushType = brushTypeSlider->value;
        cwsRebuildText(brushTypeText->context, brushTypeText, BRUSH_NAMES[brushType]);
    }
    
    cwsBindMaterial(&terrainMaterial);
    f32 ca[3] = {xz.x, xz.y, brushSize};
    cwsShaderBufferUniform(&terrainMaterial.shader, "cursor", ca, 3);
//    glUniform3f(glGetUniformLocation(terrainMaterial.shader.id, "cursor"), xz.x, xz.y, brushSize);
}

Terrain *new_terrain(i32 width, i32 depth)
{
    Terrain *t = malloc(sizeof(Terrain));
    
    i32 attribs[] = {3, 3, 2, 3};
    cwsEmptyMesh(&t->mesh, attribs, 4);
   
    t->renderer = cwsNewRenderer(&terrainMaterial, &t->mesh);    
    t->renderer->position = (vec3){0,0.5,0};
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
        
        cws_array_push(t->vertices,0.0f);
        cws_array_push(t->vertices,0.0f);
        cws_array_push(t->vertices,0.0f);
        
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

void destroy_terrain_edit()
{
    cwsDeleteShader(&terrainMaterial.shader);
    cwsDeleteTexture(&grassTex);
    cwsDeleteTexture(&dirtTex);
    cwsDeleteTexture(&rockTex);
    cwsDeleteMaterial(&terrainMaterial);
}

f32 height_at(Terrain *t, i32 x, i32 z)
{
    if(x < 0) return 0.0f;
    else if(x > t->width) return 0.0f;
    else if(z < 0) return 0.0f;
    else if(z > t->depth) return 0.0f;
    
    return t->vertices.data[(x + t->width * z)*11+1];
}

void update_terrain(Terrain *t)
{
    i32 x = 0, z = 0;
    for(i32 i = 0; i < t->vertices.length; i += 11)
    {
        if(z > 1 && x < t->width && z < t->depth)
        {
            f32 hL = height_at(t, x-1,z);
            f32 hR = height_at(t, x+1,z);
            f32 hD = height_at(t, x,z-1);
            f32 hU = height_at(t, x,z+1);
            
            // deduce terrain normal
            vec3 N;
            N.x = hL - hR;
            N.y = 255.0f;;
            N.z = hD - hU;
            N = vec3_normalize(N);
            t->vertices.data[i+3] = N.x;
            t->vertices.data[i+4] = N.y;
            t->vertices.data[i+5] = N.z;
        }
        
        x += 1;
        if(x >= t->width)
        {
            x = 0;
            z += 1;
        }
    }
    
    cwsFillMesh(&t->mesh, t->vertices.data, t->vertices.length, t->indices.data, t->indices.length);
}

void terrain_raise_rect(Terrain *t, vec2 xz)
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
                    t->vertices.data[i*11+1] += ((f32)brushStrength);
                }
                else if((brushStrength > 0 && t->vertices.data[i*11+1] < limitHeightSlider->value))
                {
                    t->vertices.data[i*11+1] += ((f32)brushStrength);
                }
                else if((brushStrength < 0 && t->vertices.data[i*11+1] > limitHeightSlider->value))
                {
                    t->vertices.data[i*11+1] += ((f32)brushStrength);
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
            t->vertices.data[i*11+1] += ((f32)brushStrength);
        }
        else if((brushStrength > 0 && t->vertices.data[i*11+1] < limitHeightSlider->value))
        {
            t->vertices.data[i*11+1] += ((f32)brushStrength);
        }
        else if((brushStrength < 0 && t->vertices.data[i*11+1] > limitHeightSlider->value))
        {
            t->vertices.data[i*11+1] += ((f32)brushStrength);
        }        
        else
        {
            t->vertices.data[i*11+1] = limitHeightSlider->value;
        }
    }
}

void terrain_raise_circle(Terrain *t, vec2 xz)
{
    if(xz.x < 0 || xz.x >= t->width ||
       xz.y < 0 || xz.y >= t->depth)
    {
        return;
    }
    
    if(brushSize > 1)
    {
        for(f32 x = xz.x - (f32)brushSize; x < xz.x + (f32)brushSize; x++)
        {
            for(f32 z = xz.y - (f32)brushSize; z < xz.y + (f32)brushSize; z++)
            {
                f32 p = (x-xz.x)*(x-xz.x)+(z-xz.y)*(z-xz.y);
                f32 r = (f32)brushSize;
                
                if((x < 0 || x >= t->width ||
                    z < 0 || z >= t->depth) ||
                   p > (r*r))
                {
                    continue;
                }            
                
                i32 i = (i32)x + t->width * (i32)z;
                f32 d = abs((r*r)-p);
                
                f32 b = 1.0f / (brushSize*brushSize);
                if(!limitHeightBox->checked)
                {
                    t->vertices.data[i*11+1] += ((f32)brushStrength) * (d*b);
                }
                else if((brushStrength > 0 && t->vertices.data[i*11+1] < limitHeightSlider->value))
                {
                    t->vertices.data[i*11+1] += ((f32)brushStrength)  * (d*b);
                }
                else if((brushStrength < 0 && t->vertices.data[i*11+1] > limitHeightSlider->value))
                {
                    t->vertices.data[i*11+1] += ((f32)brushStrength) * (d*b);
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
            t->vertices.data[i*11+1] += ((f32)brushStrength);
        }
        else if((brushStrength > 0 && t->vertices.data[i*11+1] < limitHeightSlider->value))
        {
            t->vertices.data[i*11+1] += ((f32)brushStrength);
        }
        else if((brushStrength < 0 && t->vertices.data[i*11+1] > limitHeightSlider->value))
        {
            t->vertices.data[i*11+1] += ((f32)brushStrength);
        }        
        else
        {
            t->vertices.data[i*11+1] = limitHeightSlider->value;
        }
    }
}

void terrain_smooth(Terrain *t, vec2 pt)
{
    if(pt.x < 0 || pt.x >= t->width ||
       pt.y < 0 || pt.y >= t->depth)
    {
        return;
    }
    
    if(brushSize > 1)
    {
        for(f32 x = pt.x - (f32)brushSize; x < pt.x + (f32)brushSize; x ++)
        {
            for(f32 z = pt.y - (f32)brushSize; z < pt.y + (f32)brushSize; z ++)
            {
                f32 p = (x-pt.x)*(x-pt.x)+(z-pt.y)*(z-pt.y);
                f32 r = (f32)brushSize/2;
                
                if((x < 0 || x >= t->width ||
                    z < 0 || z >= t->depth) ||
                   p > (r*r))
                {
                    continue;
                }            
                
                i32 i = (i32)x + t->width * (i32)z;
              
                //Average current Y to neighbours
                f32 nb[8];
                if(x > 0)         nb[0] = t->vertices.data[(i-1)*11+1];
                else               nb[0] = t->vertices.data[i*11+1];
                
                if(x < t->width) nb[1] = t->vertices.data[(i+1)*11+1];
                else                  nb[1] = t->vertices.data[i*11+1];
                
                if(z > 0) nb[2] = t->vertices.data[(i-t->width)*11+1];
                else       nb[2] = t->vertices.data[i*11+1];
                
                if(z < t->depth) nb[3] = t->vertices.data[(i+t->width)*11+1];
                else                  nb[3] = t->vertices.data[i*11+1];
                
                if(x > 0 && z > 0 ) nb[4] = t->vertices.data[(i-t->width-1)*11+1];
                else                      nb[4] = t->vertices.data[i*11+1];
                
                if(x < t->width && z > 0 ) nb[5] = t->vertices.data[(i-t->width+1)*11+1];
                else                                  nb[5] = t->vertices.data[i*11+1];
                
                if(x < t->width && z < t->depth ) nb[6] = t->vertices.data[(i+t->width+1)*11+1];
                else                                             nb[6]  = t->vertices.data[i*11+1];
                
                if(x >0 && z < t->depth ) nb[7] = t->vertices.data[(i+t->width-1)*11+1];
                else                                 nb[7]  = t->vertices.data[i*11+1];
                
                f32 sum = t->vertices.data[i*11+1];
                for(i32 i = 0; i < 8; ++i)
                {
                    sum += nb[i];
                }
                sum /= 9.0f;
                t->vertices.data[i*11+1] = (t->vertices.data[i*11+1]*0.9f) + (sum*0.1f);
            }
        }
    }
    else
    {
    }    
}

void terrain_paint(Terrain *t, vec2 xz)
{
    if(xz.x < 0 || xz.x >= t->width ||
       xz.y < 0 || xz.y >= t->depth)
    {
        return;
    }
    
    if(brushSize > 1)
    {
        for(f32 x = xz.x - (f32)brushSize; x < xz.x + (f32)brushSize; x++)
        {
            for(f32 z = xz.y - (f32)brushSize; z < xz.y + (f32)brushSize; z++)
            {
                f32 p = (x-xz.x)*(x-xz.x)+(z-xz.y)*(z-xz.y);
                f32 r = (f32)brushSize;
                
                if((x < 0 || x >= t->width ||
                    z < 0 || z >= t->depth) ||
                   p > (r*r))
                {
                    continue;
                }            
                
                i32 i = (i32)x + t->width * (i32)z;
                f32 d = abs((r*r)-p);
                
                f32 b = 1.0f / (brushSize*brushSize);
                f32 v = d*b*0.1f;
                if(v > 1) v = 1.0f;
                t->vertices.data[i*11+8+brushTextureSlider->value] += v;
                if(t->vertices.data[i*11+8+brushTextureSlider->value] > 1.0f) t->vertices.data[i*11+8] = 1.0f;
            }
        }
    }
    else
    {
        i32 i = (i32)xz.x + t->width * (i32)xz.y;
        t->vertices.data[i*11+8+brushTextureSlider->value] += 0.01f;
        if(t->vertices.data[i*11+8+brushTextureSlider->value] > 1.0f) t->vertices.data[i*11+8+brushTextureSlider->value] = 1.0f;
    }
}

void terrain_edit(vec2 p1, vec2 p2)
{
    for(i32 i = 0; i < terrains.length; ++i)
    {
        Terrain *t = terrains.data[i];
        p1 = vec2_sub(p1, (vec2){t->renderer->position.x, t->renderer->position.z});
        p2 = vec2_sub(p2, (vec2){t->renderer->position.x, t->renderer->position.z});
        if(brushType == BRUSH_RECT)
        {
            vec2 d = vec2_sub(p2,p1);
            for(f32 i = 0.0f; i < 1.0f; i += 0.25f)
            {
                terrain_raise_rect(t,(vec2){
                                   .x = p1.x + (d.x*i),
                                   .y = p1.y + (d.y*i)
                                   });
            }
        }
        else if(brushType == BRUSH_CIRCLE)
        {
            vec2 d = vec2_sub(p2,p1);
            for(f32 i = 0.0f; i < 1.0f; i += 0.25f)
            {
                terrain_raise_circle(t,(vec2){
                                     .x = p1.x + (d.x*i),
                                     .y = p1.y + (d.y*i)
                                     });
            }
        }
        else if(brushType == BRUSH_SMOOTHER)
        {
            terrain_smooth(t,(vec2){
                           .x = p2.x,
                           .y = p2.y
                           });
        }
        else if(brushType == BRUSH_PAINT)
        {
            vec2 d = vec2_sub(p2,p1);
            f32 l = vec2_length(d);
            f32 iv = 1.0f / l;
            
            vec2_mul_scalar(d, iv);
            cws_log("%f", iv);
            for(f32 i = 0; i < 1.0f; i += iv)
            {
                terrain_paint(t, (vec2){
                              .x = p1.x + (d.x * i),
                              .y = p1.y + (d.y * i)
                              });
            }
        }
        update_terrain(t);
    }
}

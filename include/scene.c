#include "scene.h"
//Grid
cwsRenderer *gridRenderer;
cwsMesh gridMesh;
cwsMaterial gridMaterial;
cwsTexture2D gridTexture;

cwsDirLight *sceneLight;
cwsCamera *sceneCam;

f32 grid_size = 1.0f;
void init_scene()
{
    sceneCam = cwsNewCamera();
    cwsSetActiveCamera(sceneCam);
    sceneCam->position = (vec3){.x = 0, .y = 4, .z = 0};
    sceneCam->rot = (vec3){.x = -45, .y = -135, .z = 0};
    
    i32 attribs[1] = {3};
    cwsEmptyMesh(&gridMesh, attribs, 1);
    f32 x = 0, y = 0;
    cws_array(f32, vertices);
    cws_array_init(f32, vertices,0);
    cws_array(i32, indices);
    cws_array_init(i32, indices,0);
    
    for(i32 i = 0; i < 400; ++i)
    {
        if(x < 19*grid_size && y < 19 * grid_size)
        {
            i32 l = vertices.length/3;
            cws_array_push(indices, l);
            cws_array_push(indices, l+1);
            cws_array_push(indices, l+20);
            
            cws_array_push(indices, l+20);
            cws_array_push(indices, l+21);
            cws_array_push(indices, l);
        }
        
        cws_array_push(vertices, x);
        cws_array_push(vertices, 0);
        cws_array_push(vertices, y);
        
        x += grid_size;
        if(x >= 20*grid_size)
        {
            x = 0;
            y += grid_size;
        }
    }
    cwsFillMesh(&gridMesh, vertices.data, vertices.length, indices.data, indices.length);
    
    cwsMaterialInit(gridMaterial);
    cwsShaderFromfile(&gridMaterial.shader, "./data/shaders/grid_v", "./data/shaders/grid_f");
    cwsTextureFromfile(&gridTexture, "./data/gfx/grid.png", IF_LINEAR_MIP_LINEAR);
    cwsMaterialAddTexture(&gridMaterial, gridTexture);
    gridMaterial.rflags = RF_WIREFRAME_ENABLED;
    gridRenderer = cwsNewRenderer(&gridMaterial, &gridMesh);
    gridRenderer->position = (vec3){.x = 0, .y = 0.0f, .z = 0};
    gridRenderer->scale = (vec3){.x = 1, .y = 1, .z = 1};

    init_terrainEdit();
    
    sceneLight = cwsNewDirLight();
    sceneLight->rot = (vec3){35,45,0};
}

vec2 last_local = (vec2){.x = -1, .y = -1};
void update_scene()
{
    if(selected_tool == TOOL_TERRAIN_EDIT)
    {
        ray r = cwsBuildPickRay(sceneCam);
        aabb ab;
        
        ab.center = (vec3){0,0,0};
        ab.extent = (vec3){100,0.1,100};
        ray_hitinfo info = ray_aabb_test(r,ab);
        
        //If LCTRL is pressed then snap the positions to the grid size
        if(get_key_state(SDL_SCANCODE_LCTRL) == KEY_PRESSED)
        {    
            info.point.x = (f32)(((i32)info.point.x / (i32)grid_size) * (i32)grid_size);
            info.point.z = (f32)(((i32)info.point.z / (i32)grid_size) * (i32)grid_size);
        }
        
        update_terrain_edit((vec2){info.point.x, info.point.z});   
        if(info.hit)
        {
            if(get_mouse_state(0) == MOUSE_PRESSED)
            {
                vec2 local = (vec2){
                    info.point.x,
                    info.point.z
                };
                
                vec2 d = vec2_sub(last_local, local);
                if(vec2_length(d) < 2)
                {
                    terrain_edit(last_local, local);
                }
                last_local = local;
            }
        }
    }
}

void destroy_scene()
{
    cwsDeleteMesh(&gridMesh);
    cwsDeleteShader(&gridMaterial.shader);
    cwsDeleteMaterial(&gridMaterial);
    cwsDeleteDirLight(sceneLight);
    destroy_terrain_edit();
}

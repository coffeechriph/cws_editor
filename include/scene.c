#include "scene.h"
//Grid
cwsRenderer *gridRenderer;
cwsMesh gridMesh;
cwsMaterial gridMaterial;
cwsTexture2D gridTexture;

//Cube cursor
cwsRenderer *cursorRenderer;
cwsMesh cursorMesh;
cwsMaterial cursorMaterial;

cwsCamera *sceneCam;

void init_scene()
{
    sceneCam = cwsNewCamera();
    cwsSetActiveCamera(sceneCam);
    sceneCam->position = (vec3){.x = 0, .y = 4, .z = 0};
    sceneCam->rot = (vec3){.x = -45, .y = 0, .z = 0};
    
    cwsPlaneMesh(&gridMesh, 50);
    cwsMaterialInit(gridMaterial);
    cwsShaderFromfile(&gridMaterial.shader, "./data/shaders/grid_v", "./data/shaders/grid_f");
    cwsTextureFromfile(&gridTexture, "./data/gfx/grid.png", IF_LINEAR_MIP_LINEAR);
    cwsMaterialAddTexture(&gridMaterial, gridTexture);
    gridRenderer = cwsNewRenderer(&gridMaterial, &gridMesh);
    gridRenderer->position = (vec3){.x = 0, .y = 0.0f, .z = 0};
    gridRenderer->scale = (vec3){.x = 100, .y = 0, .z = 100};

    cwsMaterialInit(cursorMaterial);
    cwsShaderFromfile(&cursorMaterial.shader, "./data/shaders/single_v", "./data/shaders/single_f");
    cwsCubeMesh(&cursorMesh);
    cursorRenderer = cwsNewRenderer(&cursorMaterial, &cursorMesh);
    cursorRenderer->position = (vec3){.x = 0, .y = 0, .z = 0};
    cursorRenderer->scale = (vec3){.x = 1, .y = 1, .z = 1};
    cwsUpdateBounds(cursorRenderer);
    
    init_terrainEdit();
}

void update_scene()
{
    if(selected_tool == TOOL_TERRAIN_EDIT)
    {
        update_terrain_edit();
        ray r = cwsBuildPickRay(sceneCam);
        aabb ab;
        
        ab.center = (vec3){0,0,0};
        ab.extent = (vec3){50,0.1,50};
        ray_hitinfo info = ray_aabb_test(r,ab);
        
        if(info.hit)
        {
            cursorRenderer->position = (vec3){.x = info.point.x, .y = 0.0f, .z = info.point.z};
            
            if(get_mouse_state(0) == MOUSE_PRESSED)
            {
                vec2 local = (vec2){
                    info.point.x - dummy_terrain->renderer->position.x,
                    info.point.z - dummy_terrain->renderer->position.z
                };
                
                terrain_raise(dummy_terrain, local);
            }
        }
    }
}

void destroy_scene()
{
    cwsDeleteMesh(&cursorMesh);
    cwsDeleteMesh(&gridMesh);
    cwsDeleteShader(&gridMaterial.shader);
    cwsDeleteMaterial(&gridMaterial);
}

#include "scene.h"
//Grid
cwsRenderer *gridRenderer;
cwsMesh gridMesh;
cwsMaterial gridMaterial;
cwsTexture2D gridTexture;

cwsDirLight *sceneLight;
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
        
        update_terrain_edit((vec2){info.point.x, info.point.z});   
        if(info.hit)
        {
            if(get_mouse_state(0) == MOUSE_PRESSED)
            {
                vec2 local = (vec2){
                    info.point.x - dummy_terrain->renderer->position.x,
                    info.point.z - dummy_terrain->renderer->position.z
                };
                
                vec2 d = vec2_sub(last_local, local);
                if(vec2_length(d) < 2)
                {
                    terrain_edit(dummy_terrain, last_local, local);
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

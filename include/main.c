#include "cws.h"
#include "util/input.h"
#include "toolbar.h"
#include "scene.h"

int main(int args, char *argv[])
{
    cws_create(1280,720,"RPG", true);
    
    create_toolbar();
    init_scene();
    
    while(cws_running)
    {
        cws_run();
        
        cwsCamera *cam = cwsActiveCamera();
        if(get_key_state(SDL_SCANCODE_W) == KEY_PRESSED)
        {
             cam->position = vec3_add(cam->position, cam->dir);
        }
        else if(get_key_state(SDL_SCANCODE_S) == KEY_PRESSED)
        {
            cam->position = vec3_sub(cam->position, cam->dir);        
        }
        
        if(get_key_state(SDL_SCANCODE_LALT) == KEY_PRESSED)
        {
            vec2 md = get_mouse_delta();
            cam->rot.x -= md.y * 0.5f;
            cam->rot.y -= md.x * 0.5f;
            capture_mouse(true);
        }
        else if(get_key_state(SDL_SCANCODE_LALT) == KEY_RELEASED)
        {
            capture_mouse(false);
        }
        
        update_scene();
        update_toolbar();
        cwsClear();
        cwsSceneDraw();
        cwsGuiDraw();
        cwsSwapBuffers();
        
        if(get_key_state(SDL_SCANCODE_ESCAPE) == KEY_PRESSED)
        {
            cws_running = false;
        }
    }
    
    destroy_scene();
    destroy_toolbar();
    cws_close();
    return 0;
}
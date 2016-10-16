#include "toolbar.h"

cwsGuiSurface *toolbar;
cwsGuiToggleButton *newProjectBtn, *saveProjectBtn, *loadProjectBtn;
cwsGuiToggleButton *terrainEditBtn, *materialEditBtn;

i32 selected_tool = -1;

void create_toolbar()
{
    vec2 scrsz = cwsScreenSize();
    surface_skin.fill_color = cwsPackRgb((ivec3){.x = 31, .y = 34, .z = 49});
    surface_skin.outline_color = cwsPackRgb((ivec3){.x = 21, .y = 24, .z = 39});
    surface_skin.outline_size = 1;
    
    slider_skin.mark_scale = (vec2){.x = 0.025f, .y = 1.2f};
    
    toolbar = cwsNewSurface(NULL);
    toolbar->transform->size = (vec2){.x = scrsz.x - 400, .y = 40};
    toolbar->transform->pos = (vec3){.x = 200, scrsz.y - 40, 0};
    toolbar->renderer->fill = true;
    
    newProjectBtn = cwsSurfaceAddToggleButton(toolbar);
    newProjectBtn->pos = (vec2){.x = 2, .y = 2};
    newProjectBtn->size = (vec2){.x = 36, .y = 36};
    cwsRebuildText(newProjectBtn->text->context, newProjectBtn->text, "NP");
    
    saveProjectBtn = cwsSurfaceAddToggleButton(toolbar);
    saveProjectBtn->pos = (vec2){.x = 40, .y = 2};
    saveProjectBtn->size = (vec2){.x = 36, .y = 36};
    cwsRebuildText(saveProjectBtn->text->context, saveProjectBtn->text, "SP");
    
    loadProjectBtn = cwsSurfaceAddToggleButton(toolbar);
    loadProjectBtn->pos = (vec2){.x = 78, .y = 2};
    loadProjectBtn->size = (vec2){.x = 36, .y = 36};
    cwsRebuildText(loadProjectBtn->text->context, loadProjectBtn->text, "LP");
    
    terrainEditBtn = cwsSurfaceAddToggleButton(toolbar);
    terrainEditBtn->pos = (vec2){.x = 116, .y = 2};
    terrainEditBtn->size = (vec2){.x = 36, .y = 36};
    cwsRebuildText(terrainEditBtn->text->context, terrainEditBtn->text, "TE");

    materialEditBtn = cwsSurfaceAddToggleButton(toolbar);
    materialEditBtn->pos = (vec2){.x = 154, .y = 2};
    materialEditBtn->size = (vec2){.x = 36, .y = 36};
    cwsRebuildText(materialEditBtn->text->context, materialEditBtn->text, "ME");
    
    cwsRefreshSurface(toolbar);
    create_material_edit();
}

void enable_tool(i32 tooltype)
{
    if(tooltype == TOOL_NEW_PROJECT)
    {
        saveProjectBtn->toggled = false;
        loadProjectBtn->toggled = false;
        terrainEditBtn->toggled = false;
        materialEditBtn->toggled = false;
        newProjectBtn->toggled = !newProjectBtn->toggled;
        cwsShowSurface(materialEditSurface, false);
        cwsShowSurface(terrainEditSurface, false);
    }
    else if(tooltype == TOOL_SAVE_PROJECT)
    {
        newProjectBtn->toggled = false;
        loadProjectBtn->toggled = false;
        terrainEditBtn->toggled = false;
        materialEditBtn->toggled = false;
        saveProjectBtn->toggled = true;
        cwsShowSurface(terrainEditSurface, false);
        cwsShowSurface(materialEditSurface, false);
    }
    else if(tooltype == TOOL_LOAD_PROJECT)
    {
        newProjectBtn->toggled = false;
        saveProjectBtn->toggled = false;
        terrainEditBtn->toggled = false;
        materialEditBtn->toggled = false;
        loadProjectBtn->toggled = !loadProjectBtn->toggled;
        cwsShowSurface(terrainEditSurface, false);
        cwsShowSurface(materialEditSurface, false); 
    }
    else if(tooltype == TOOL_TERRAIN_EDIT)
    {
        newProjectBtn->toggled = false;
        saveProjectBtn->toggled = false;
        loadProjectBtn->toggled = false;
        materialEditBtn->toggled = false;
        terrainEditBtn->toggled = !terrainEditBtn->toggled;
        cwsShowSurface(materialEditSurface, false);
        cwsShowSurface(terrainEditSurface, terrainEditBtn->toggled);
    }
    else if(tooltype == TOOL_MATERIAL_EDIT)
    {
        newProjectBtn->toggled = false;
        saveProjectBtn->toggled = false;
        loadProjectBtn->toggled = false;
        terrainEditBtn->toggled = false;
        materialEditBtn->toggled = !materialEditBtn->toggled;
        cwsShowSurface(materialEditSurface, materialEditBtn->toggled);
        cwsShowSurface(terrainEditSurface, false);
    }
    
    selected_tool = tooltype;
}

//Key shortcuts for the toolbar
bool wait_key_release = false;
void shortcut_toolbar()
{
    if(get_key_state(SDL_SCANCODE_LCTRL) == KEY_PRESSED)
    {
        if(get_key_state(SDL_SCANCODE_1) == KEY_PRESSED)
        {
            if(wait_key_release == false)
            {
                wait_key_release = true;
                enable_tool(TOOL_NEW_PROJECT);
            }
        }
        else if(get_key_state(SDL_SCANCODE_2) == KEY_PRESSED)
        {
            if(wait_key_release == false)
            {
                wait_key_release = true;
                enable_tool(TOOL_SAVE_PROJECT);
            }
        }
        else if(get_key_state(SDL_SCANCODE_3) == KEY_PRESSED)
        {
            if(wait_key_release == false)
            {
                wait_key_release = true;
                enable_tool(TOOL_LOAD_PROJECT);
            }
        }
        else if(get_key_state(SDL_SCANCODE_4) == KEY_PRESSED)
        {
            if(wait_key_release == false)
            {
                wait_key_release = true;
                enable_tool(TOOL_TERRAIN_EDIT);
            }
        }
        else if(get_key_state(SDL_SCANCODE_5) == KEY_PRESSED)
        {
            if(wait_key_release == false)
            {
                wait_key_release = true;
                enable_tool(TOOL_MATERIAL_EDIT);
            }
        }
        else
        {
            wait_key_release = false;
        }
    }
}

void update_toolbar()
{
    shortcut_toolbar();
    if(newProjectBtn->event_flags & EVENT_CLICKED)
    {
        enable_tool(TOOL_NEW_PROJECT);
    }
    else if(saveProjectBtn->event_flags & EVENT_CLICKED)
    {
        enable_tool(TOOL_SAVE_PROJECT);
    }
    else if(loadProjectBtn->event_flags & EVENT_CLICKED)
    {
        enable_tool(TOOL_LOAD_PROJECT);
    }
    else if(terrainEditBtn->event_flags & EVENT_CLICKED)
    {
        enable_tool(TOOL_TERRAIN_EDIT);
    }
    else if(materialEditBtn->event_flags & EVENT_CLICKED)
    {
        enable_tool(TOOL_MATERIAL_EDIT);
    }
    
    update_material_edit();
}

void destroy_toolbar()
{
    destroy_material_edit();
}
#ifndef TOOLBAR_H
#define TOOLBAR_H
#include "./gui/gui.h"
#include "materialEdit.h"
#include "terrainEdit.h"

typedef enum
{
    TOOL_NONE,
    TOOL_NEW_PROJECT,
    TOOL_SAVE_PROJECT,
    TOOL_LOAD_PROJECT,
    TOOL_TERRAIN_EDIT,
    TOOL_MATERIAL_EDIT,
    TOOL_ENTITY_EDIT
} SELECTED_TOOL;

extern i32 selected_tool;

void create_toolbar();
void update_toolbar();
void destroy_toolbar();
#endif
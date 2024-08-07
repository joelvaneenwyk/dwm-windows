#ifndef DWM_MOD_DWM
#define DWM_MOD_DWM

#include "dwm-win32.h"

typedef struct {
    HINSTANCE hInstance;
} DwmState;

int luaopen_dwm(lua_State *L);

void dwm_setstate(lua_State *L, DwmState *state);
DwmState *dwm_getstate(lua_State *L);

#endif

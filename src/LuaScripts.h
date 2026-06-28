#pragma once 
#include "Registry.h"
#include "Components.h"
extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

extern "C" int lua_get_position(lua_State* L) {
    Registry* registry = static_cast<Registry*>(lua_touserdata(L, lua_upvalueindex(1)));
    Entity e = static_cast<Entity>(lua_tointeger(L, lua_upvalueindex(2)));
    
    if (!registry) {
        return luaL_error(L, "Registry not bound");
    }
    
    Transform* tr = registry->get<Transform>(e);
    
    if (!tr) {
        return luaL_error(L, "Entity has no Transform");
    }
    
    lua_pushnumber(L, tr->x);
    lua_pushnumber(L, tr->y);
    lua_pushnumber(L, tr->z);
    
    return 3;
}

extern "C" int lua_set_position(lua_State* L) {
    Registry* registry = static_cast<Registry*>(lua_touserdata(L, lua_upvalueindex(1)));
    Entity entity = static_cast<Entity>(lua_tointeger(L, lua_upvalueindex(2)));
    
    if (!registry) {
        return luaL_error(L, "Registry not bound");
    }
    
    lua_Number x = luaL_checknumber(L, 1);
    lua_Number y = luaL_checknumber(L, 2);
    lua_Number z = luaL_checknumber(L, 3);

    Transform* tr = registry->get<Transform>(entity);
    if (!tr) {
        return luaL_error(L, "Entity has no Transform");
    }
    
    tr->x = static_cast<float>(x);
    tr->y = static_cast<float>(y);
    tr->z = static_cast<float>(z);
    return 0;
}


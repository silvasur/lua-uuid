/***
Utility for generating UUIDs by wrapping libuuid's generate().

@license MIT
@module lua_uuid
*/
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <uuid/uuid.h>

#define UUID_LEN 16

/// Generate a UUID. As a shortcut, you can also call the module directly to generate a uuid.
// @return uuid_str
// @function generate()
static int generate(lua_State *L) {
    uuid_t uuid;
    char uuid_str[37];

    uuid_generate(uuid);
    uuid_unparse_lower(uuid, uuid_str);

    lua_pushstring(L, uuid_str);
    return 1;
}

/// Generate a binary UUID
// @return uuid
// @function generate_binary()
static int generate_binary(lua_State *L) {
    uuid_t uuid;
    uuid_generate(uuid);

    lua_pushlstring(L, (char *)&uuid, UUID_LEN);
    return 1;
}

/// Unparse UUID (create a hex string representation from a binary string representation)
// @tparam string binary_uuid
// @treturn string uuid_str
// @error if the binary_uuid could not be unparsed
// @function unparse
static int unparse(lua_State *L) {
    const char *binary_uuid;
    size_t binary_uuid_len;
    uuid_t uuid;
    char uuid_str[37];

    binary_uuid = luaL_checklstring(L, 1, &binary_uuid_len);

    if (binary_uuid_len != UUID_LEN) {
        lua_pushnil(L);
        lua_pushfstring(L, "could not unparse uuid: string length was != %d", UUID_LEN);
        return 2;
    }

    memcpy(&uuid[0], binary_uuid, UUID_LEN);

    uuid_unparse_lower(uuid, uuid_str);

    lua_pushstring(L, uuid_str);
    return 1;
}

/// Parse a hexadecimal UUID into a binary representation
// @tparam string uuid_str
// @treturn string uuid
// @error if the uuid_str could not be parsed
// @function parse
static int parse(lua_State *L) {
    const char *uuid_str;
    uuid_t uuid;

    uuid_str = luaL_checkstring(L, 1);

    if (uuid_parse(uuid_str, uuid) != 0) {
        lua_pushnil(L);
        lua_pushstring(L, "could not parse uuid");
        return 2;
    }

    lua_pushlstring(L, (char *)uuid, UUID_LEN);
    return 1;
}

static int mt_call(lua_State *L) {
    lua_pop(L, 1);
    return generate(L);
}

static const struct luaL_Reg mylib [] = {
    {"generate", generate},
    {"generate_binary", generate_binary},
    {"unparse", unparse},
    {"parse", parse},
    {NULL, NULL}
};

int luaopen_lua_uuid(lua_State *L) {
    // Set up library table
    lua_newtable(L);
    const luaL_Reg *fundef = mylib;
    for(fundef = mylib; fundef->name != NULL; fundef++) {
        lua_pushcfunction(L, fundef->func);
        lua_setfield(L, -2, fundef->name);
    }

    // Set up metatable
    lua_newtable(L);
    lua_pushcfunction(L, mt_call);
    lua_setfield(L, -2, "__call");

    lua_setmetatable(L, -2);
    return 1;
}

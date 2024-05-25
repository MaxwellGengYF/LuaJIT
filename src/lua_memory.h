#pragma once
#ifndef LUA_MEMORY_INCLUDE
#define LUA_MEMORY_INCLUDE
#ifdef __cplusplus
extern "C" {
#endif
#include "luaconf.h"
LUA_API void *lua_realloc(void *ptr, size_t size);
LUA_API void lua_free(void *ptr);
LUA_API void *lua_allocate(size_t size);
LUA_API void *lua_aligned_allocate(size_t align, size_t size);

#ifdef __cplusplus
}
#endif
#endif
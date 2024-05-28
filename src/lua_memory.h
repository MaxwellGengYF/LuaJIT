#pragma once
#ifndef LUA_MEMORY_INCLUDE
#define LUA_MEMORY_INCLUDE
#ifdef __cplusplus
extern "C" {
#endif
#include "luaconf.h"
LUA_API void
lua_set_allocator(void *(*lua_realloc)(void *ptr, size_t size),
                  void (*lua_free)(void *ptr), void *(*lua_allocate)(size_t size),
                  void *(*lua_aligned_allocate)(size_t align, size_t size));
LUA_API void lua_setenv(char const* env);
#ifdef __cplusplus
}
#endif
#endif
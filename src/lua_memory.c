#include "lua_memory.h"
#include <stdlib.h>
#ifdef LUA_USE_CUSTOM_ALLOCATOR
void *(*_lua_realloc)(void *ptr, size_t size) = NULL;
void (*_lua_free)(void *ptr) = NULL;
void *(*_lua_allocate)(size_t size) = NULL;
void *(*_lua_aligned_allocate)(size_t align, size_t size) = NULL;
LUA_API void *lua_realloc(void *ptr, size_t size) {
  return _lua_realloc(ptr, size);
}
LUA_API void lua_free(void *ptr) { _lua_free(ptr); }
LUA_API void *lua_allocate(size_t size) { return _lua_allocate(size); }
LUA_API void *lua_aligned_allocate(size_t align, size_t size) {
  return _lua_aligned_allocate(align, size);
}
LUA_API void lua_set_allocator(void *(*lua_realloc)(void *ptr, size_t size),
                               void (*lua_free)(void *ptr),
                               void *(*lua_allocate)(size_t size),
                               void *(*lua_aligned_allocate)(size_t align,
                                                             size_t size)) {
  _lua_realloc = lua_realloc;
  _lua_free = lua_free;
  _lua_allocate = lua_allocate;
  _lua_aligned_allocate = lua_aligned_allocate;
}
#else
#include "mimalloc.h"
LUA_API void *lua_realloc(void *ptr, size_t size) {
  return mi_realloc(ptr, size);
}
LUA_API void lua_free(void *ptr) { mi_free(ptr); }
LUA_API void *lua_allocate(size_t size) { return mi_malloc(size); }
LUA_API void *lua_aligned_allocate(size_t align, size_t size) {
  return mi_aligned_alloc(align, size);
}
LUA_API void lua_set_allocator(void *(*lua_realloc)(void *ptr, size_t size),
                               void (*lua_free)(void *ptr),
                               void *(*lua_allocate)(size_t size),
                               void *(*lua_aligned_allocate)(size_t align,
                                                             size_t size)) {
  exit(1);
}
#endif
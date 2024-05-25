#include "lua_memory.h"
#include "mimalloc.h"
LUA_API void *lua_realloc(void *ptr, size_t size) {
  return mi_realloc(ptr, size);
}
LUA_API void lua_free(void *ptr) { mi_free(ptr); }
LUA_API void *lua_allocate(size_t size) { return mi_malloc(size); }
LUA_API void *lua_aligned_allocate(size_t align, size_t size) {
  return mi_aligned_alloc(align, size);
}
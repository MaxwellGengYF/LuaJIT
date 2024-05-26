#include "config.h"
#include <iostream>
#include <lua_memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string_view>
#ifdef _WIN32
#define LUA_FSEEK _fseeki64
#define LUA_FTELL _ftelli64
#else
#define LUA_FSEEK fseeko
#define LUA_FTELL ftello
#endif
LUA_EXPORT_API string_view _getenv(char const *ptr) {
  auto r = std::getenv(ptr);
  return {r, strlen(r)};
}
LUA_EXPORT_API void *_malloc(size_t m) { return lua_allocate(m); }
LUA_EXPORT_API void _free(void *ptr) { lua_free(ptr); }
LUA_EXPORT_API void *_realloc(void *ptr, size_t size) { lua_realloc(ptr, size); }
LUA_EXPORT_API void _memset(void *ptr, int value, size_t size) {
  memset(ptr, value, size);
}
LUA_EXPORT_API int _memcmp(void const *a, void const *b, size_t size) {
  return memcmp(a, b, size);
}
LUA_EXPORT_API char _char_at(char const *ptr, size_t idx) { return ptr[idx - 1]; }

LUA_EXPORT_API string string_make(size_t capacity) {
  if (capacity == 0) {
    return string{nullptr, 0, 0};
  }
  return string{(char *)lua_allocate(capacity), 0, capacity};
}
LUA_EXPORT_API void string_dispose(string *s) {
  if (s->capacity) {
    lua_free(s->data);
    s->size = 0;
    s->capacity = 0;
  }
}
LUA_EXPORT_API void string_reserve(string *s, size_t new_capacity) {
  if (s->capacity >= new_capacity)
    return;
  s->data = (char *)lua_realloc(s->data, new_capacity);
  s->capacity = new_capacity;
}
LUA_EXPORT_API void string_add_str(string *s, char const *ptr, size_t size) {
  string_reserve(s, size + s->size);
  memcpy(s->data + s->size, ptr, size);
  s->size += size;
}
LUA_EXPORT_API void string_add_chr(string *s, char const *ptr) {
  string_add_str(s, ptr, strlen(ptr));
}
LUA_EXPORT_API string_view string_subview(string *s, size_t beg, size_t end) {
  return string_view{s->data + beg, end - beg};
}
LUA_EXPORT_API string string_from_file(char const *name) {
  auto f = fopen(name, "rb");
  if (!f) {
    return string{nullptr, 0, 0};
  }
  LUA_FSEEK(f, 0, SEEK_END);
  auto length = LUA_FTELL(f);
  LUA_FSEEK(f, 0, SEEK_SET);
  auto str = string_make(length + 1);
  fread(str.data, length, 1, f);
  str.size = length;
  fclose(f);
  return str;
}

LUA_EXPORT_API bool string_to_file(string *s, char const *name) {
  if (s->size == 0)
    return true;
  auto f = fopen(name, "wb");
  if (f) {
    fwrite(s->data, s->size, 1, f);
    fclose(f);
    return true;
  }
  return false;
}
LUA_EXPORT_API string string_from_file_str(string *v) {
  string_reserve(v, v->size + 1);
  v->data[v->size] = 0;
  return string_from_file(v->data);
}
LUA_EXPORT_API bool string_to_file_str(string *s, string *v) {
  string_reserve(v, v->size + 1);
  v->data[v->size] = 0;
  return string_to_file(s, v->data);
}
LUA_EXPORT_API string string_from_file_strview(string_view v) {
  auto tmp = (char *)lua_allocate(v.size + 1);
  memcpy(tmp, v.data, v.size);
  tmp[v.size] = 0;
  auto r = string_from_file(tmp);
  lua_free(tmp);
  return r;
}
LUA_EXPORT_API bool string_to_file_strview(string *s, string_view v) {
  auto tmp = (char *)lua_allocate(v.size + 1);
  memcpy(tmp, v.data, v.size);
  tmp[v.size] = 0;
  auto r = string_to_file(s, tmp);
  lua_free(tmp);
  return r;
}
LUA_EXPORT_API void string_set(string *s, size_t idx, char c) {
  s->data[idx - 1] = c;
}

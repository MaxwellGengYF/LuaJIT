#pragma once
#include <stdint.h>
#ifdef __cplusplus
#define LUA_EXTERN_C extern "C"
#define LUA_NOEXCEPT noexcept
#else
#define LUA_EXTERN_C
#define LUA_NOEXCEPT
#endif

#ifdef _MSC_VER
#define LUA_FORCE_INLINE inline
#define LUA_NEVER_INLINE __declspec(noinline)
#define LUA_EXPORT_API LUA_EXTERN_C __declspec(dllexport)
#define LUA_IMPORT_API LUA_EXTERN_C __declspec(dllimport)
#else
#define LUA_FORCE_INLINE __attribute__((always_inline, hot)) inline
#define LUA_NEVER_INLINE __attribute__((noinline))
#define LUA_EXPORT_API LUA_EXTERN_C __attribute__((visibility("default")))
#define LUA_IMPORT_API LUA_EXTERN_C
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "interface.inl"
#ifdef __cplusplus
}
#endif
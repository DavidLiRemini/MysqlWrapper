#pragma once

#if defined(WIN32) && defined(_MSC_VER)

#ifdef LIBSQLSTRING_EXPORTS
#define LIBSQLSTRING_API __declspec(dllexport)
#else
#ifdef CC_LIB_BUILD
#define LIBSQLSTRING_API
#else
#define LIBSQLSTRING_API __declspec(dllimport)
#endif
#endif
//
#else
#define LIBSQLSTRING_API __attribute__((visibility("default")))
#endif

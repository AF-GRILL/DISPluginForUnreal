
#ifndef OPENDIS6_EXPORT_H
#define OPENDIS6_EXPORT_H

#if defined(_MSC_VER)
#  define OPENDIS6_DECL_EXPORT __declspec(dllexport)
#  define OPENDIS6_DECL_IMPORT __declspec(dllimport)
#  define OPENDIS6_DECL_DEPRECATED __declspec(deprecated)
#elif defined(__GNUC__) || defined(__clang__)
#  define OPENDIS6_DECL_EXPORT __attribute__((visibility("default")))
#  define OPENDIS6_DECL_IMPORT __attribute__((visibility("default")))
#  define OPENDIS6_DECL_DEPRECATED __attribute__((deprecated))
#else
#  define OPENDIS6_DECL_EXPORT
#  define OPENDIS6_DECL_IMPORT
#  define OPENDIS6_DECL_DEPRECATED
#endif

#ifdef OPENDIS6_STATIC_DEFINE
#  define OPENDIS6_EXPORT
#  define OPENDIS6_NO_EXPORT
#else
#  ifndef OPENDIS6_EXPORT
#    ifdef OpenDIS6_EXPORTS
        /* We are building this library */
#      define OPENDIS6_EXPORT OPENDIS6_DECL_EXPORT
#    else
        /* We are using this library */
#      define OPENDIS6_EXPORT OPENDIS6_DECL_IMPORT
#    endif
#  endif

#  ifndef OPENDIS6_NO_EXPORT
#    define OPENDIS6_NO_EXPORT 
#  endif
#endif

#ifndef OPENDIS6_DEPRECATED
#  define OPENDIS6_DEPRECATED OPENDIS6_DECL_DEPRECATED
#endif

#ifndef OPENDIS6_DEPRECATED_EXPORT
#  define OPENDIS6_DEPRECATED_EXPORT OPENDIS6_EXPORT OPENDIS6_DEPRECATED
#endif

#ifndef OPENDIS6_DEPRECATED_NO_EXPORT
#  define OPENDIS6_DEPRECATED_NO_EXPORT OPENDIS6_NO_EXPORT OPENDIS6_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef OPENDIS6_NO_DEPRECATED
#    define OPENDIS6_NO_DEPRECATED
#  endif
#endif

#endif /* OPENDIS6_EXPORT_H */

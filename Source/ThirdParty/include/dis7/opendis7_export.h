
#ifndef OPENDIS7_EXPORT_H
#define OPENDIS7_EXPORT_H

#if defined(_MSC_VER)
#  define OPENDIS7_DECL_EXPORT __declspec(dllexport)
#  define OPENDIS7_DECL_IMPORT __declspec(dllimport)
#  define OPENDIS7_DECL_DEPRECATED __declspec(deprecated)
#elif defined(__GNUC__) || defined(__clang__)
#  define OPENDIS7_DECL_EXPORT __attribute__((visibility("default")))
#  define OPENDIS7_DECL_IMPORT __attribute__((visibility("default")))
#  define OPENDIS7_DECL_DEPRECATED __attribute__((deprecated))
#else
#  define OPENDIS7_DECL_EXPORT
#  define OPENDIS7_DECL_IMPORT
#  define OPENDIS7_DECL_DEPRECATED
#endif

#ifdef OPENDIS7_STATIC_DEFINE
#  define OPENDIS7_EXPORT
#  define OPENDIS7_NO_EXPORT
#else
#  ifndef OPENDIS7_EXPORT
#    ifdef OpenDIS7_EXPORTS
        /* We are building this library */
#      define OPENDIS7_EXPORT OPENDIS7_DECL_EXPORT
#    else
        /* We are using this library */
#      define OPENDIS7_EXPORT OPENDIS7_DECL_IMPORT
#    endif
#  endif

#  ifndef OPENDIS7_NO_EXPORT
#    define OPENDIS7_NO_EXPORT 
#  endif
#endif

#ifndef OPENDIS7_DEPRECATED
#  define OPENDIS7_DEPRECATED OPENDIS7_DECL_DEPRECATED
#endif

#ifndef OPENDIS7_DEPRECATED_EXPORT
#  define OPENDIS7_DEPRECATED_EXPORT OPENDIS7_EXPORT OPENDIS7_DEPRECATED
#endif

#ifndef OPENDIS7_DEPRECATED_NO_EXPORT
#  define OPENDIS7_DEPRECATED_NO_EXPORT OPENDIS7_NO_EXPORT OPENDIS7_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef OPENDIS7_NO_DEPRECATED
#    define OPENDIS7_NO_DEPRECATED
#  endif
#endif

#endif /* OPENDIS7_EXPORT_H */

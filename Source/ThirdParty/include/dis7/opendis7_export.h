
#ifndef OPENDIS7_EXPORT_H
#define OPENDIS7_EXPORT_H

#ifdef OPENDIS7_STATIC_DEFINE
#  define OPENDIS7_EXPORT
#  define OPENDIS7_NO_EXPORT
#else
#  ifndef OPENDIS7_EXPORT
#    ifdef OpenDIS7_EXPORTS
        /* We are building this library */
#      define OPENDIS7_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define OPENDIS7_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef OPENDIS7_NO_EXPORT
#    define OPENDIS7_NO_EXPORT 
#  endif
#endif

#ifndef OPENDIS7_DEPRECATED
#  define OPENDIS7_DEPRECATED __declspec(deprecated)
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

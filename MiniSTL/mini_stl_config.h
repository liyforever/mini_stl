#ifndef MINI_STL_CONFIG_H
#define MINI_STL_CONFIG_H

#define MINI_STL_BEGIN  namespace Mini_STL{
#define MINI_STL_END    }
#define _MY_STL         Mini_STL

#define MINI_STL_USE_THREAD

#ifndef NDEBUG
#   define MINI_STL_DEBUG
#endif

#if defined (WIN32) || defined (WIN64)
#     define OS_WIN
#endif

#if defined (linux)
#     define OS_LINUX
#endif

// Microsoft compiler.
#ifdef _MSC_VER
#   if _MSC_VER >= 1500
#       define MINI_STL_CLASS_PARTIAL_SPECIALIZATION
#       define MINI_STL_HAS_WCHAR_T
#       define MINI_STL_RVALUE_REFS
#   endif
#endif


# ifdef MINI_STL_USE_EXCEPTIONS
#   define MINI_STL_TRY try
#   define MINI_STL_CATCH_ALL catch(...)
#   define MINI_STL_THROW(x) throw x
#   define MINI_STL_RETHROW throw
#   define MINI_STL_NOTHROW throw()
#   define MINI_STL_UNWIND(action) catch(...) { action; throw; }
# else
#   define MINI_STL_TRY
#   define MINI_STL_CATCH_ALL if (false)
#   define MINI_STL_THROW(x)
#   define MINI_STL_RETHROW
#   define MINI_STL_NOTHROW
#   define MINI_STL_UNWIND(action)
# endif

#endif// MINI_STL_CONFIG_H

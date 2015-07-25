#ifndef MINI_STL_CONFIG_H
#define MINI_STL_CONFIG_H

#define MINI_STL_BEGIN  namespace Mini_STL{
#define MINI_STL_END    }
#define _MY_STL         Mini_STL
//#define MINI_STL_THROW_RANGE_ERROR(msg) (throw std::out_of_range(msg))

#define MINI_STL_THROW_RANGE_ERROR(msg) (throw std::range_error(msg))
#define MINI_STL_THROW_INVALID_ERROR(msg) (throw std::invalid_argument(msg))
#define MINI_STL_THROW_OVERFLOW_ERROR(msg)  (throw std::overflow_error(msg))
#define MINI_STL_THROW_RANGE_OUT_OF_RANGE_ERROR (throw std::out_of_range(msg))
#define MINI_STL_THROW_LOGIC_ERROR(msg) (throw std::logic_error(msg))
// Microsoft compiler.
#ifdef _MSC_VER
#   ifdef _DEBUG
#       define MINI_STL_DEBUG
#   endif
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

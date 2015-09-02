#ifndef MINI_STL_SHARED_PTR_H
#define MINI_STL_SHARED_PTR_H
#include "mini_stl_config.h"
#include "mini_stl_algobase.h"
#include <stddef.h>
MINI_STL_BEGIN

template<class Type>
class shared_ptr
{
private:
  Type* Myptr_;
  size_t *Myuse_;
  template<class Other>
  friend class shared_ptr;
public:
  typedef Type  element_type;
public:
  shared_ptr()
    : Myptr_(0), Myuse_(0)
  {}

  shared_ptr(const shared_ptr&  _Right)
  {
    _decr();
    Myptr_ = _Right.Myptr_;
    Myuse_ = _Right.Myuse_;
    _incr();
  }

  template<class Other>
  explicit shared_ptr(Other *_Ptr)
  {
    _decr();
    Myptr_ = _Ptr;
    Myuse_ = new size_t(1);
  }

  template<class Other>
  shared_ptr(const shared_ptr<Other>& _Right)
  {
    _decr();
    Myptr_ = _Right.Myptr_;
    Myuse_ = _Right.Myuse_;
    _incr();
  }

  shared_ptr(shared_ptr&& _Right)
  {
    _MY_STL::swap(Myptr_, _Right.Myptr_);
    _MY_STL::swap(Myuse_, _Right.Myuse_);
  }

  template<class Other>
  shared_ptr(shared_ptr<Other>&& _Right)
  {
    _MY_STL::swap(Myptr_, _Right.Myptr_);
    _MY_STL::swap(Myuse_, _Right.Myuse_);
  }

  shared_ptr& operator=(shared_ptr&& _Right)
  {
    Myptr_ = _Right.Myptr_;
    Myuse_ = _Right.Myuse_;
    return *this;
  }

  shared_ptr& operator=(const shared_ptr& _Right)
  {
    _decr();
    Myptr_ = _Right.Myptr_;
    Myuse_ = _Right.Myuse_;
    _incr();
    return *this;
  }

  template<class Other>
  shared_ptr& operator=(const shared_ptr<Other>& _Right)
  {
    _decr();
    Myptr_ = const_cast<shared_ptr<Other>&>(_Right).Myptr_;
    Myuse_ = _Right.Myuse_;
    _incr();
    return *this;
  }

  ~shared_ptr()
  {
    _incr();
  }
public:
  Type* get() const
  {
    return Myptr_;
  }

  operator bool() const
  {
    return Myptr_ != 0;
  }

  Type& operator*() const
  {
    MINI_STL_DEBUG_POINTER(Myptr_, "shared_ptr operator*()");
    return *Myptr_;
  }

  Type* operator->() const
  {
    return Myptr_;
  }

  bool unique() const
  {
    return *Myuse_ == 1;
  }

  size_t use_count() const
  {
    return *Myuse_;
  }

  void swap(shared_ptr& _Right)
  {
    _MY_STL::swap(Myptr_, _Right.Myptr_);
    _MY_STL::swap(Myuse_, _Right.Myuse_);
  }

private:
  void _incr()
  {
    ++*Myuse_;
  }

  void _decr()
  {
    if (Myuse_ == 0)
      return;

    if (--*Myuse_ == 0)
      delete Myptr_;
  }
};

MINI_STL_END
#endif // MINI_STL_SHARED_PTR_H

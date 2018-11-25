#pragma once

namespace	arcade {
  template<typename T>
  class		argPtr {
  public:
    argPtr(T const *ptr);
    ~argPtr();

    T		**get() const;
  private:
    T		**_ptr;
  };

  template<typename T>
  argPtr<T>::argPtr(T const *ptr) {
    _ptr = new T*[2];
    _ptr[0] = const_cast<T *>(ptr);
    _ptr[1] = 0;
  }

  template<typename T>
  T	**argPtr<T>::get() const {
    return _ptr;
  }

  template<typename T>
  argPtr<T>::~argPtr() {
    delete[] _ptr;
  }
}

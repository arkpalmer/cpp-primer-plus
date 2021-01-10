#ifndef __BLOB_PTR_H__ 
#define __BLOB_PTR_H__

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>

#include "types.h"

// StrBlobPtr: 474, 475, 566-569
template <typename T>
class Blob_ptr
{
public:
    Blob_ptr() : curr_(0) { }
    // note that curr_ is intialized to off the end
    Blob_ptr(Blob<T>& blob, size_t sz = 0) : wptr_(blob.data_), curr_(sz) { }

    T& operator*() const;
    T* operator->() const;

    // prefix versions
    Blob_ptr& operator++();
    Blob_ptr& operator--();
    // postfix versions
    Blob_ptr  operator++(int);
    Blob_ptr  operator--(int);

private:
    std::shared_ptr<std::vector<T>> check(size_t i, const std::string& msg) const;

    std::weak_ptr<std::vector<T>> wptr_;

    std::size_t curr_;
};

template <typename T>
std::shared_ptr<std::vector<T>> Blob_ptr<T>::check(size_t i, const std::string& msg) const
{
    auto ret = wptr_.lock();

    if (ret == nullptr)
    {
        throw std::runtime_error("unbound Blob_ptr/nullptr");
    }

    if (i >= ret->size())
    {
        std::stringstream ss;
        ss << msg << " size:" << ret->size();

        throw std::out_of_range(ss.str());
    }

    return ret;
}

template <typename T>
T& Blob_ptr<T>::operator*() const
{
    std::stringstream ss;
    ss << "deref past end, curr_:" << curr_;

    auto ref = check(curr_, ss.str());

    return (*ref)[curr_]; // (*ref) is the vector to which this object points
}

template <typename T>
T* Blob_ptr<T>::operator->() const
{
    return &this->operator*(); 
}

template <typename T>
Blob_ptr<T>& Blob_ptr<T>::operator++()
{
    std::stringstream ss;
    ss << "can't incr, already past end, curr_:" << curr_;

    auto ref = check(curr_, ss.str());

    // this is prefix operator++, so inc first
    ++curr_;

    return *this; // ugh, had to look this up
}

template <typename T>
Blob_ptr<T>& Blob_ptr<T>::operator--()
{
    if (curr_ == 0)
    {
        std::stringstream ss;
        ss << "can't dec, already at begin" << curr_;

        throw std::out_of_range(ss.str());
    }

    --curr_;

    return *this;
}

// we can't return a ref since we need to increment curr_ after we have
// the value to be returned (we can't return a local copy of a saved ref)
// I had tried this before having to look at the book:
// blob-ptr.h:107:10: warning: reference to local variable ‘ret’ returned [-Wreturn-local-addr]
//     auto ret = *this; //nope
//
template <typename T>
Blob_ptr<T> Blob_ptr<T>::operator++(int)
{
    auto ret = *this;

    ++*this; // prefix ++ will do check

    return ret;
}

template <typename T>
Blob_ptr<T> Blob_ptr<T>::operator--(int)
{
    auto ret = *this;

    --*this; // prefix -- will do check

    return ret;
}

#endif
#ifndef __BLOB_PTR_H__ 
#define __BLOB_PTR_H__

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>

// StrBlobPtr: 474, 475, 566-569
template <typename T>
class Blob_ptr
{
public:
    Blob_ptr() : curr_(0) { }
    // note that curr_ is intialized to off the end
    Blob_ptr(Blob<T>& blob, size_t sz) : wptr_(blob.data_), curr_(sz) { }

    T& deref() const;

// HERE use operator++ and operator-- instead
    Blob_ptr& incr();

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

    if (i <= ret->size())
    {
        std::stringstream ss;
        ss << msg << " size:" << ret->size();

        throw std::out_of_range(ss.str());
    }

    return ret;
}

template <typename T>
T& Blob_ptr<T>::deref() const
{
    std::stringstream ss;
    ss << "deref past end, curr_:" << curr_;

    auto ref = check(curr_, ss.str());

    return (*ref)[curr_];
}

template <typename T>
Blob_ptr<T>& Blob_ptr<T>::incr()
{
    std::stringstream ss;
    ss << "can't incr, already past end, curr_:" << curr_;

    auto ref = check(curr_, ss.str());

    ++curr_;

    return *this; // ugh, had to look this up
}

#endif
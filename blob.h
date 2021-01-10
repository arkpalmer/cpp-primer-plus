#ifndef __BLOB_H__ 
#define __BLOB_H__

#include <iostream>
#include <sstream>
#include <sstream>
#include <vector>
#include <memory> 
#include <utility>

//? do I need this if a user wants Blob<string> ??
// (but I need it since it's used in check, but wondering)
#include <string>

#include "types.h"

// p659, also see StrBlob:
// § 12.1.1 (p. 456) and updated in § 12.1.6 (p. 475) and in Chapters 13 and 14

template <typename> class Blob_ptr;
template <typename> class Blob; // for parameter declaration in operator== forward declaration
template <typename T> bool operator==(const Blob<T>&, const Blob<T>&);
template <typename T> bool operator!=(const Blob<T>&, const Blob<T>&);
template <typename T> std::ostream& operator<<(std::ostream&, const Blob<T>&);
template <typename T> void swap(Blob<T>&, Blob<T>&);

template <typename T>
class Blob
{
public:
    friend class Blob_ptr<T>; // Blob_ptr references data_
    // TBD exactly why we need the first <T> here but not in the definition
    friend bool operator==<T>(const Blob<T>&, const Blob<T>&);
    friend bool operator!=<T>(const Blob<T>&, const Blob<T>&);
    friend std::ostream& operator<< <T>(std::ostream&, const Blob<T>&);
    friend void swap<T>(Blob<T>&, Blob<T>&);

    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    // constructors
    Blob();
    Blob(std::initializer_list<T> ilist);

    Blob(const Blob& rhs); // copy ctor
    Blob<T>& operator=(const Blob<T>& rhs);

    size_t size() const { return data_->size(); }
    bool empty() const { return data_->empty(); }

    Blob_ptr<T> begin(); // const? or is there a cbegin?
    Blob_ptr<T> end(); // const? or is there a cbegin?
    // HERE
    // end() (and follow-up to cbegin question above)

    void push_back(const T& el) { data_->push_back(el); }
    // move version; see § 13.6.3 (p. 548)
    void push_back(const T&& el) { data_->push_back(std::move(el)); }

    void pop_back();

    T& back();

    T& operator[](size_t i);

// HERE2 - front, back and begin end in Blob

private:
    // shared_ptr that points to a vector of T
    std::shared_ptr<std::vector<T>> data_;
    void check(size_t i, const std::string& msg);
};

template <typename T>
Blob<T>::Blob() : 
    data_(std::make_shared<std::vector<T>>()) 
{ 
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template <typename T>
Blob<T>::Blob(std::initializer_list<T> ilist)  :
    data_(std::make_shared<std::vector<T>>(ilist))
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template <typename T>
Blob<T>::Blob(const Blob& rhs)
{
    data_ = std::make_shared<std::vector<T>>(*rhs.data_);
}

template <typename T>
void swap(Blob<T>& lhs, Blob<T>& rhs)
{
    using std::swap;

    // TBD data_ isn't a raw pointer, so would need more than just swap
}

// maybe could use copy/swap? (but see TBD above)
template <typename T>
Blob<T>& Blob<T>::operator=(const Blob<T>& rhs)
{
    if (this == &rhs)
    {
        return *this;
    }

    // how does book do it, github uses make_shared here (I guess more efficient that way)
    auto temp = Blob(rhs);
    // std::cout << __PRETTY_FUNCTION__ << " temp:" << temp << std::endl;
    // github didn't do this, does writing over the shared_ptr free the mem
    data_ = nullptr;

    data_ = temp.data_;

    return *this;
}

template <typename T>
Blob_ptr<T> Blob<T>::begin()
{
    auto bp = Blob_ptr<T>(*this);

    return bp;
}

template <typename T>
Blob_ptr<T> Blob<T>::end()
{
    auto bp = Blob_ptr<T>(*this, size());

    return bp;
}

template <typename T>
void Blob<T>::check(size_t i, const std::string& msg)
{
    //std::cout << "check i:" << i << " size:" << size() << std::endl;

    if (i >= data_->size())
    {
        std::cout << "check failed" << std::endl;
        throw std::out_of_range(msg);
    }
}

template <typename T>
void Blob<T>::pop_back()
{
    check(0, "pop_back() on empty blob");
    data_->pop_back();
}

template <typename T>
T& Blob<T>::back()
{
    check(0, "back() on empty blob");
    return data_->back();
}

template <typename T>
T& Blob<T>::operator[](size_t i)
{
    std::stringstream ss;
    ss << "index out of range rqstd index:" << i << " Blob size:" << size();
    check(i, ss.str());

    return (*data_)[i];
}

template <typename T> 
bool operator==(const Blob<T>& lhs, const Blob<T>& rhs)
{
    if (lhs.data_ == nullptr || rhs.data_ == nullptr)
    {
        throw std::out_of_range("nullptr");
    }

    return *lhs.data_ == *rhs.data_;
}

template <typename T> 
bool operator!=(const Blob<T>& lhs, const Blob<T>& rhs)
{
    return !(lhs == rhs);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Blob<T>& blob)
{
    if (blob.data_ == nullptr)
    {
        throw std::out_of_range("nullptr");
    }

    std::stringstream ss;

    for (const auto& i : *blob.data_)
    {
        ss << i << ", ";
    }

    auto s = ss.str();

    if (s.empty())
    {
        os << "blob is empty";
        return os;
    }

    // remove ", "
    s.pop_back();
    s.pop_back();
    
    os << s;

    return os;
}

#endif

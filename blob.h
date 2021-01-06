#ifndef __BLOB_H__ 
#define __BLOB_H__

#include <iostream>
#include <sstream>
#include <vector>
#include <memory> 
#include <utility>

//? do I need this if a user wants Blob<string> ??
// (but I need it since it's used in check, but wondering)
#include <string>

// p659, also see StrBlob:
// § 12.1.1 (p. 456) and updated in § 12.1.6 (p. 475) and in Chapters 13 and 14

template <typename> class Blob_ptr;

template <typename T>
class Blob
{
public:
    friend class Blob_ptr<T>;

    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    // constructors
    Blob();
    Blob(std::initializer_list<T> ilist);

    size_t size() const { return data_->size(); }
    bool empty() const { return data_->empty(); }

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

#endif
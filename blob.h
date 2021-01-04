#ifndef __BLOB_H__ 
#define __BLOB_H__

#include <iostream>
#include <vector>
#include <memory> 
#include <utility>

//? do I need this if a user wants Blob<string> ??
// (but I need it since it's used in check, but wondering)
#include <string>

// p659, also see StrBlob:
// § 12.1.1 (p. 456) and updated in § 12.1.6 (p. 475) and in Chapters 13 and 14

template <typename T>
class Blob
{
public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    // constructors
    Blob();
    Blob(std::initializer_list<T> ilist);

    size_t size() const { return data_->size(); }

private:
    // shared_ptr that points to a vector of T
    std::shared_ptr<std::vector<T>> data_;
    void check(size_t i, std::string& msg);
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
void Blob<T>::check(size_t i, std::string& msg)
{
    if (i >= data_->size())
    {
        std::cout << "check failed" << std::endl;
        throw std::out_of_range(msg);
    }
}

#endif
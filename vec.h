#ifndef __VEC_H__ 
#define __VEC_H__

#include <iostream>
#include <memory>

// modify so as not to use construct or destroy (deprecated after C++ 11)

template <typename T>
class Vec
{
public:
    Vec() : elements_(nullptr), first_free_(nullptr), cap_(nullptr) {}

    size_t size() const { return first_free_ - elements_; }
    size_t capacity() const { return cap_ - elements_; }

    T* begin() const { return elements_; }
    T* end() const { return first_free_; }

    // NEXT whatever move routines you need, 
    // NEXT2 dtor?

    void push_back(const T& el);

private:
    // will ensure there is room to add at least one element to Vec
    // If there isn't room, call reallocate to get more space
    void check_n_alloc();

    // allocate space and copy a range of elements
    void alloc_n_copy(const T* first_el, const T* last_el);
    // reallocate Vec when it runs out of space
    void reallocate();

    std::allocator<T> allocator_;

    T* elements_;    // point to first element in allocated memory of Vec
    T* first_free_;  // just after the last actual element
    T* cap_;         // just past the end of allocated memory
};

template <typename T>
void Vec<T>::check_n_alloc()
{
    if (size() == capacity()) 
    {
        reallocate();
    }
}

// return pair<T*,T*> not sure why yet?
template <typename T>
void Vec<T>::alloc_n_copy(const T* first_el, const T* last_el)
{
    size_t required = last_el - first_el + 1;

    size_t available = capacity() - size();

    std::cout << "req:" << required << " avail:" << available << std::endl;

}

template <typename T>
void Vec<T>::reallocate()
{
    if (capacity() == 0)
    {
        std::cout << "reallocate: no space" << std::endl;
    }

    auto cap = (capacity() == 0 ? 1 : capacity() * 2);

    T* tempel = allocator_.allocate(cap);
    T* tempff = tempel;

    // now copy the existing data into the new space (move?)
    for (auto it = begin(); it != end(); ++it)
    {
        std::cout << *it << std::endl;
        // TODO next, somehow use move 
        // would the std::move version take T&& and use tempff++ = it ?
        // what do you need? move ctor, move cctor, move assign?? 
        *(tempff)++ = std::move(*it);
        // TODO try both of these on a class with and w/o move ctor
        // e.g. Screen
        //*(tempff)++ = *it;
    }

    elements_   = tempel;
    first_free_ = tempff;

    cap_ = elements_ + cap;
}

template <typename T>
void Vec<T>::push_back(const T& el)
{
    reallocate();
    std::cout << "push_back" << std::endl;

// this is actual version, check_n_alloc calls reallocate()
// check_n_alloc();
    *first_free_++ = el;
}

#endif
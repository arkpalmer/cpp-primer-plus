#ifndef __VEC_H__ 
#define __VEC_H__

#include <iostream>
#include <memory>
#include <sstream>

// modify so as not to use construct or destroy (deprecated after C++ 11)

template <typename> class Vec; // needed for parameters in operator<< etc

template <typename T> std::ostream& operator<<(std::ostream& os, Vec<T>& v);

template <typename T>
class Vec
{
public:
    Vec() : elements_(nullptr), first_free_(nullptr), cap_(nullptr) 
    { std::cout << __PRETTY_FUNCTION__ << std::endl;}
    Vec(const Vec& other);

    ~Vec() { free(); }

    Vec& operator=(const Vec& rhs);

    size_t size() const { return first_free_ - elements_; }
    size_t capacity() const { return cap_ - elements_; }

    T* begin() const { return elements_; }
    T* end() const { return first_free_; }

    void push_back(const T& el);

    friend std::ostream& operator<< <T>(std::ostream& os, Vec& v);

private:
    // will ensure there is room to add at least one element to Vec
    // If there isn't room, call reallocate to get more space
    void check_n_alloc();
    // reallocate Vec when it runs out of space
    void reallocate();

    // allocate space and copy a range of elements
    std::pair<T*, T*> alloc_n_copy(const T* first_el, const T* last_el);

    void free();

    std::allocator<T> allocator_;

    T* elements_;    // point to first element in allocated memory of Vec
    T* first_free_;  // just after the last actual element
    T* cap_;         // just past the end of allocated memory
};

template <typename T>
Vec<T>::Vec(const Vec<T>& other)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    auto res = alloc_n_copy(other.begin(), other.end());

    elements_   = res.first;
    first_free_ = cap_ = res.second;
}

template <typename T>
void Vec<T>::free()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    //for (T* p = elements_; p != first_free_; ++p)

    if (capacity() == 0) return;

    for (T* data = (first_free_ - 1); data != elements_; --data)
    {
        std::cout << " p:" << data << std::endl;
        //delete data;  //crashes
        allocator_.destroy(data);
    }

    allocator_.deallocate(elements_, capacity());
}

template <typename T>
Vec<T>& Vec<T>::operator=(const Vec<T>& rhs)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    if (&rhs == this)
    {
        std::cout << "operator= they are the same" << std::endl;
    }

    //auto res = alloc_n_copy(rhs.begin(), rhs.end());
    std::pair<T*,T*> res = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements_ = res.first;
    first_free_ = cap_ = res.second;

    std::cout << "dbg3 " << *(elements_) << " " << *(first_free_ - 1) << std::endl;

    return *this;
}

template <typename T>
void Vec<T>::check_n_alloc()
{
    if (size() == capacity()) 
    {
        reallocate();
    }
}

// returns a pair of pointer, one at the start of the new space, the 
// second one past the last element copied into the new space
template <typename T>
std::pair<T*, T*> Vec<T>::alloc_n_copy(const T* first_el, const T* last_el)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    size_t required = last_el - first_el;

    std::cout << "req:" << required << std::endl;

    auto data = allocator_.allocate(required);
    auto temp = data;

    for (int i=0; i<required; ++i)
    {
        std::cout << *(first_el + i) << " data:" << data << std::endl;
        *data++ = *(first_el + i);
    }

    // data has been advanced, first is the first element
    std::cout << "dbg2 " << *temp << " " << *(data-1) << std::endl;
    return {temp, data};
    //return {data - required, data};
}

template <typename T>
void Vec<T>::reallocate()
{
    if (capacity() == 0)
    {
        std::cout << "reallocate: no space" << std::endl;
    }

    auto cap = (capacity() == 0 ? 1 : capacity() * 2);

    std::cout << "reallocate: cap:" << cap << std::endl;

    T* tempel = allocator_.allocate(cap);
    T* tempff = tempel;

    // now copy the existing data into the new space (move?)
    for (auto it = begin(); it != end(); ++it)
    {
        // TODO next, somehow use move 
        // would the std::move version take T&& and use tempff++ = it ?
        // what do you need? move ctor, move cctor, move assign?? 
        std::cout << *it << " tempff:" << tempff << std::endl;
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
    std::cout << "push_back" << std::endl;

    check_n_alloc();
    std::cout << "push_back first_free:" << first_free_ << std::endl;
    *first_free_++ = el;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, Vec<T>& v)
{
    std::cout << "operator<<" << std::endl;

    std::stringstream ss;

    for (auto it = v.begin(); it != v.end(); ++it)
    {
        ss << *it << ", ";
    }
    
    // TODO make this some utility for repeated use
    auto s = ss.str();

    if (s.empty())
    {
        os << "Vec is empty";
    }
    else
    {
        s.pop_back();
        s.pop_back();
        os << s;
    }

    return os;
}

#endif
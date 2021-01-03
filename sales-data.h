#ifndef __SALES_DATA_H__ 
#define __SALES_DATA_H__

#include <string>
#include <iostream>

class Sales_data
{
public:
    Sales_data() = default;
    Sales_data(const std::string& s, unsigned n, double p);
    Sales_data(const std::string& s);
    Sales_data(std::istream&);
    std::string isbn() const;
    Sales_data& combine(const Sales_data&);

    friend bool operator<(const Sales_data& s1, const Sales_data& s2);

private:
    double avg_price() const;

    std::string book_no_;
    unsigned    units_sold_ = 0;
    double      revenue_    = 0.0;
};

#endif
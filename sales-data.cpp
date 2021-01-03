#include "sales-data.h"

Sales_data::Sales_data(const std::string& s, unsigned n, double p) :
    book_no_(s), units_sold_(n), revenue_(p*n) {}

Sales_data::Sales_data(const std::string& s) :
    book_no_(s) {}

bool operator< (const Sales_data& s1, const Sales_data& s2)
{
    return s1.revenue_ < s2.revenue_;
}

// TODO:
// Sales_data::Sales_data(std::istream&);

std::string Sales_data::isbn() const
{
    return book_no_;
}

double Sales_data::avg_price() const
{
    return (units_sold_ ? revenue_/units_sold_ : 0);
}

Sales_data& Sales_data::combine(const Sales_data& other)
{
    units_sold_ += other.units_sold_;
    revenue_    += other.revenue_;

    return *this;
}

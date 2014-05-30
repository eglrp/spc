#include "VariantDataContainer.h"

#include <boost/lexical_cast.hpp>
#include <sstream>

namespace spc
{

template <typename T> void to_string_visitor::operator()(T const &item)
{
    str = boost::lexical_cast<std::string>(item);
}

void to_string_visitor::operator()(Eigen::Vector3f const &v)
{
    std::stringstream stream;
    stream << "[" << v(0) << " " << v(1) << " " << v(2) << "]";

    str = stream.str();
}

void to_string_visitor::operator()(std::string const &s)
{
    std::stringstream stream;
    stream << "\"" << s.c_str() << "\"";

    str = stream.str();
}

std::ostream &operator<<(std::ostream &os, const VariantDataContainer &obj)
{
    return os << obj.asString().c_str();
}

VariantDataContainer::VariantDataContainer(const VariantDataContainer::VarianT value)
{
    data_ = value;
}

VariantDataContainer::VarianT &VariantDataContainer::value()
{
    return data_;
}

std::string VariantDataContainer::asString() const
{
    to_string_visitor vis;
    boost::apply_visitor(vis, data_);
    return vis.str;
}

VariantDataContainer &VariantDataContainer::operator=(const double &data)
{
    data_ = VarianT(static_cast<float>(data));
    return *this;
}

bool VariantDataContainer::operator==(const VariantDataContainer &other) const
{
    return (other.data_ == data_);
}

} // end nspace
#include "Value/Value.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace FusioCore {

Scalar::Scalar(double value) : value_(value) {}

double Scalar::getValue() const {
    return value_;
}

void Scalar::setValue(double value) {
    value_ = value;
}

std::string Scalar::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << value_;
    return oss.str();
}

Scalar Scalar::operator+(const Scalar& other) const {
    return Scalar(value_ + other.value_);
}

Scalar Scalar::operator-(const Scalar& other) const {
    return Scalar(value_ - other.value_);
}

Scalar Scalar::operator*(const Scalar& other) const {
    return Scalar(value_ * other.value_);
}

Scalar Scalar::operator/(const Scalar& other) const {
    if (other.value_ == 0.0) {
        throw std::runtime_error("Division by zero");
    }
    return Scalar(value_ / other.value_);
}

} // namespace FusioCore 
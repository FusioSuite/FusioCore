#include "Value/Value.hpp"
#include <sstream>
#include <iomanip>

namespace FusioCore {

Vector::Vector(const Eigen::VectorXd& data) : data_(data) {}

Vector::Vector(size_t size, double defaultValue) : data_(Eigen::VectorXd::Constant(size, defaultValue)) {}

const Eigen::VectorXd& Vector::getData() const {
    return data_;
}

Eigen::VectorXd& Vector::getData() {
    return data_;
}

void Vector::setData(const Eigen::VectorXd& data) {
    data_ = data;
}

size_t Vector::size() const {
    return static_cast<size_t>(data_.size());
}

std::string Vector::toString() const {
    std::ostringstream oss;
    oss << "[";
    for (int i = 0; i < data_.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << std::fixed << std::setprecision(6) << data_(i);
    }
    oss << "]";
    return oss.str();
}

Vector Vector::operator+(const Vector& other) const {
    return Vector(data_ + other.data_);
}

Vector Vector::operator-(const Vector& other) const {
    return Vector(data_ - other.data_);
}

Scalar Vector::operator*(const Vector& other) const {
    return Scalar(data_.dot(other.data_));
}

Vector Vector::operator*(const Scalar& scalar) const {
    return Vector(data_ * scalar.getValue());
}

} // namespace FusioCore 
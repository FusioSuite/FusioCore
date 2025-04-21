#include "Value/Value.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace FusioCore {

Matrix::Matrix(const Eigen::MatrixXd& data) : data_(data) {}

Matrix::Matrix(size_t rows, size_t cols, double defaultValue) 
    : data_(Eigen::MatrixXd::Constant(rows, cols, defaultValue)) {}

const Eigen::MatrixXd& Matrix::getData() const {
    return data_;
}

Eigen::MatrixXd& Matrix::getData() {
    return data_;
}

void Matrix::setData(const Eigen::MatrixXd& data) {
    data_ = data;
}

size_t Matrix::rows() const {
    return static_cast<size_t>(data_.rows());
}

size_t Matrix::cols() const {
    return static_cast<size_t>(data_.cols());
}

double& Matrix::operator()(size_t i, size_t j) {
    return data_(static_cast<int>(i), static_cast<int>(j));
}

const double& Matrix::operator()(size_t i, size_t j) const {
    return data_(static_cast<int>(i), static_cast<int>(j));
}

std::string Matrix::toString() const {
    std::ostringstream oss;
    oss << "[";
    for (int i = 0; i < data_.rows(); ++i) {
        if (i > 0) oss << "; ";
        for (int j = 0; j < data_.cols(); ++j) {
            if (j > 0) oss << ", ";
            oss << std::fixed << std::setprecision(6) << data_(i, j);
        }
    }
    oss << "]";
    return oss.str();
}

Matrix Matrix::operator+(const Matrix& other) const {
    return Matrix(data_ + other.data_);
}

Matrix Matrix::operator-(const Matrix& other) const {
    return Matrix(data_ - other.data_);
}

Matrix Matrix::operator*(const Matrix& other) const {
    return Matrix(data_ * other.data_);
}

Matrix Matrix::operator*(const Scalar& scalar) const {
    return Matrix(data_ * scalar.getValue());
}

Vector Matrix::operator*(const Vector& vector) const {
    return Vector(data_ * vector.getData());
}

Matrix Matrix::transpose() const {
    return Matrix(data_.transpose());
}

Scalar Matrix::determinant() const {
    return Scalar(data_.determinant());
}

Matrix Matrix::inverse() const {
    if (data_.determinant() == 0.0) {
        throw std::runtime_error("Matrix is not invertible");
    }
    return Matrix(data_.inverse());
}

} // namespace FusioCore 
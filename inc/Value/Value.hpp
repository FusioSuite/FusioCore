#pragma once

#include <memory>
#include <string>
#include <Eigen/Dense>

namespace FusioCore {

// Interface de base pour toutes les valeurs
class IValue {
public:
    virtual ~IValue() = default;
    virtual std::string toString() const = 0;
    virtual bool isMatrix() const = 0;
    virtual bool isScalar() const = 0;
    virtual bool isVector() const = 0;
};

// Classe pour les valeurs scalaires
class Scalar : public IValue {
public:
    explicit Scalar(double value = 0.0);
    
    double getValue() const;
    void setValue(double value);
    
    std::string toString() const override;
    bool isMatrix() const override { return false; }
    bool isScalar() const override { return true; }
    bool isVector() const override { return false; }
    
    // Opérateurs arithmétiques
    Scalar operator+(const Scalar& other) const;
    Scalar operator-(const Scalar& other) const;
    Scalar operator*(const Scalar& other) const;
    Scalar operator/(const Scalar& other) const;
    
private:
    double value_;
};

// Classe pour les vecteurs
class Vector : public IValue {
public:
    explicit Vector(const Eigen::VectorXd& data = Eigen::VectorXd::Zero(0));
    Vector(size_t size, double defaultValue = 0.0);
    
    const Eigen::VectorXd& getData() const;
    Eigen::VectorXd& getData();
    void setData(const Eigen::VectorXd& data);
    size_t size() const;
    
    std::string toString() const override;
    bool isMatrix() const override { return false; }
    bool isScalar() const override { return false; }
    bool isVector() const override { return true; }
    
    // Opérateurs arithmétiques
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Scalar operator*(const Vector& other) const; // Produit scalaire
    Vector operator*(const Scalar& scalar) const;
    
private:
    Eigen::VectorXd data_;
};

// Classe pour les matrices
class Matrix : public IValue {
public:
    explicit Matrix(const Eigen::MatrixXd& data = Eigen::MatrixXd::Zero(0, 0));
    Matrix(size_t rows, size_t cols, double defaultValue = 0.0);
    
    const Eigen::MatrixXd& getData() const;
    Eigen::MatrixXd& getData();
    void setData(const Eigen::MatrixXd& data);
    size_t rows() const;
    size_t cols() const;
    
    std::string toString() const override;
    bool isMatrix() const override { return true; }
    bool isScalar() const override { return false; }
    bool isVector() const override { return false; }
    
    // Opérateurs arithmétiques
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(const Scalar& scalar) const;
    Vector operator*(const Vector& vector) const;
    
    // Opérations matricielles
    Matrix transpose() const;
    Scalar determinant() const;
    Matrix inverse() const;
    
    // Accès aux éléments
    double& operator()(size_t i, size_t j);
    const double& operator()(size_t i, size_t j) const;
    
private:
    Eigen::MatrixXd data_;
};

} // namespace FusioCore 
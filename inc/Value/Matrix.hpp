#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Value/IValue.hpp"
#include <Eigen/Dense>

namespace FusioCore {

class FusioMatrix final : public IFusioValue {
public:
    explicit FusioMatrix(const Eigen::MatrixXd& matrix);
    ~FusioMatrix() override = default;

    const Eigen::MatrixXd& getData() const;
    Eigen::MatrixXd& getData();

    FusioType getType() const override;
    std::string toString() const override;

private:
    Eigen::MatrixXd _matrix;
};

} // namespace FusioCore

#endif // MATRIX_HPP

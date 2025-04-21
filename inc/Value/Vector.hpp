#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Value/IValue.hpp"
#include <Eigen/Dense>

namespace FusioCore {

class FusioVector final : public IFusioValue {
public:
    explicit FusioVector(const Eigen::VectorXd& vector);
    ~FusioVector() override = default;

    const Eigen::VectorXd& getData() const;
    Eigen::VectorXd& getData();

    FusioType getType() const override;
    std::string toString() const override;

private:
    Eigen::VectorXd _vector;
};

} // namespace FusioCore

#endif // VECTOR_HPP

#ifndef SCALAR_HPP
#define SCALAR_HPP

#include "Value/IValue.hpp"

namespace FusioCore {

class FusioScalar final : public IFusioValue {
public:
    explicit FusioScalar(double value);
    ~FusioScalar() override = default;

    double getValue() const;

    FusioType getType() const override;
    std::string toString() const override;

private:
    double _value;
};

} // namespace FusioCore

#endif // SCALAR_HPP

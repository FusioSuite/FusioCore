#ifndef IVALUE_HPP
#define IVALUE_HPP

#include <string>

namespace FusioCore {

enum class FusioType {
    Scalar,
    Matrix,
    Vector,
    Unknown
};

class IFusioValue {
protected:
    IFusioValue() = default;
    virtual ~IFusioValue() = default;

    IFusioValue(const IFusioValue&) = delete;
    IFusioValue& operator=(const IFusioValue&) = delete;

public:
    virtual FusioType getType() const = 0;
    virtual std::string toString() const = 0;
};

} // namespace FusioCore

#endif // IVALUE_HPP

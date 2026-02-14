#ifndef ENGINE_MATERIALS_MATERIALPROPERTY_H_
#define ENGINE_MATERIALS_MATERIALPROPERTY_H_

#include <vector>

class IMaterialProperty {
public:
    virtual const void* getRawValue() = 0;
    virtual size_t getValueSize() = 0;
};

template <typename T>
class MaterialProperty : public IMaterialProperty {
    T value;
public:
    MaterialProperty(const T& _value) : value(_value) {}

    const void* getRawValue() override { return value; }
    size_t getValueSize() override { return sizeof(value); }

    const T& getValue() const { return T; }
    void setValue(const T& _value) { value = _value; }
};

#endif // ENGINE_MATERIALS_MATERIALPROPERTY_H_

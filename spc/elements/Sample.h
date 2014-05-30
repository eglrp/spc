#pragma once
#ifndef SAMPLE_H
#define SAMPLE_H

#include <spc/elements/MovableElement.h>
#include <spc/elements/VariantDataRecord.h>
#include <spc/elements/macros.h>

namespace spc
{

class Sample : public MovableElement
{

public:
    SPC_OBJECT(Sample)

    Sample()
    {
    }

    Sample(const float x, const float y, const float z)
        : MovableElement(x, y, z)
    {
    }

    Sample(const Eigen::Vector3f v) : MovableElement(v)
    {
    }

private:
    friend class cereal::access;

    template <class Archive> void serialize(Archive &ar)
    {
        ar(cereal::base_class<spc::MovableElement>(this));
    }
};

} // end nspace

#endif // SAMPLE_H
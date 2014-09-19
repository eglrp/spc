#pragma once
#ifndef ELEMENTSFACTORY_H
#define ELEMENTSFACTORY_H

#include <spc/elements/macros.h>
namespace spc
{
class ElementBase;
typedef spcSharedPtrMacro<ElementBase> ElementBasePtr;

class ElementsFactory
{
public:
    ElementBasePtr create(const std::string name);
};

}

#endif // ELEMENTSFACTORY_H
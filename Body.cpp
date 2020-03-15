#include "Body.h"

Body::Body()
{
    //ctor
}

Body::~Body()
{
    //dtor
}

Body::Body(const Body& other)
{
    //copy ctor
}

Body& Body::operator=(const Body& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

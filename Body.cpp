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
    blocks_properties = other.blocks_properties;
    bodies_of_blocks = other.bodies_of_blocks;
    bodies_of_edges = other.bodies_of_edges;
    bottom_elevations = other.bottom_elevations;
    connectors_properties = other.connectors_properties;
    properties = other.properties;
}

Body& Body::operator=(const Body& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    blocks_properties = rhs.blocks_properties;
    bodies_of_blocks = rhs.bodies_of_blocks;
    bodies_of_edges = rhs.bodies_of_edges;
    bottom_elevations = rhs.bottom_elevations;
    connectors_properties = rhs.connectors_properties;
    properties = rhs.properties;

    return *this;
}

Body& Body::operator+=(double &v)
{
	for (int i=0; i<bottom_elevations.size(); ++i)
        for (int j=0; j<bottom_elevations[i].size(); ++j)
            if (bottom_elevations[i][j]!=-9999)
                bottom_elevations[i][j] += v;
	return *this;
}

Body& Body::operator-=(double &v)
{
	for (int i=0; i<bottom_elevations.size(); ++i)
        for (int j=0; j<bottom_elevations[i].size(); ++j)
            if (bottom_elevations[i][j]!=-9999)
                bottom_elevations[i][j] -= v;
	return *this;
}

Body& Body::operator/=(double &v)
{
	for (int i=0; i<bottom_elevations.size(); ++i)
        for (int j=0; j<bottom_elevations[i].size(); ++j)
            if (bottom_elevations[i][j]!=-9999)
                bottom_elevations[i][j] /= v;
	return *this;
}

Body& Body::operator*=(double &v)
{
	for (int i=0; i<bottom_elevations.size(); ++i)
        for (int j=0; j<bottom_elevations[i].size(); ++j)
            if (bottom_elevations[i][j]!=-9999)
                bottom_elevations[i][j] *= v;
	return *this;
}

Body operator+(Body v1, double d)
{
	return v1 += d;
}

Body operator-(Body v1, double d)
{
	return v1 -= d;
}

Body operator/(Body v1, double d)
{
	return v1 /= d;
}

Body operator*(Body v1, double d)
{
	return v1 *= d;
}

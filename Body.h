#ifndef BODY_H
#define BODY_H

#include <string>
#include <vector>
#include <map>

using namespace std; 
struct _location
{
    int i,j;
    string name = "";
};

class Body
{
    public:
        Body();
        virtual ~Body();
        Body(const Body& other);
        Body& operator=(const Body& other);
        vector<_location> bodies_of_blocks;
		vector<string> bodies_of_edges;
		vector<vector<double>> bottom_elevations;
		map<string, vector<vector<double>>> properties;
        string connectors_properties;
        string blocks_properties;
    protected:

    private:
};

#endif // BODY_H

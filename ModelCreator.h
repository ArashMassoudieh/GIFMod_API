#ifndef MODELCREATOR_H
#define MODELCREATOR_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "MBBlock.h"
#include "Connection.h"
#include "Body.h"

using namespace std;



class CMedium;
class ModelCreator
{
    public:
        ModelCreator();
        ModelCreator(const string &filename, const string &blocksproperties, const string &connectorproperties);
		bool AddBody(const string& bodyname, const string& filename, const string& blocksproperties, const string& connectorproperties);
		virtual ~ModelCreator();
        ModelCreator(const ModelCreator& other);
        ModelCreator& operator=(const ModelCreator& other);
        bool AddLayer(const string &bodyname, CMedium *M, const string &type, double dx, double dy);
		map<string, string> connectors_properties;
		map<string, string> blocks_properties;
		bool getproperties(const string &bodyname, const string &prop, const string& filename);
		vector<_location> BBody(const string &bodyname)
		{
            if (bodies_of_blocks.count(bodyname)==0)
            {
                vector<_location> out;
                return out;
            }
            else
                return bodies_of_blocks[bodyname];
		}
		vector<string> CBody(const string &bodyname)
		{
            if (bodies_of_edges.count(bodyname)==0)
            {
                vector<string> out;
                return out;
            }
            else
                return bodies_of_edges[bodyname];
		}
		bool ConnectBodiesVertical(const string &newconnectorbodyname, CMedium *M, const string &sourcebody, const string &targetbody, const string &connector_properties, double sourcebodycoeffientinlenght=0.5, double targetbodycoefficientinlength=0.5);


    protected:

    private:

		bool file_not_found;
		bool error;
		string last_error;
		map<string, Body> Bodies;
		//map<string, vector<_location>> bodies_of_blocks;
		//map<string, vector<string>> bodies_of_edges;
		//map<string, vector<vector<double>>> bottom_elevations;
		//map<string, map<string, vector<vector<double>>>> properties;
};

#endif // MODELCREATOR_H

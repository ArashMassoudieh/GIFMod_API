#ifndef MODELCREATOR_H
#define MODELCREATOR_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "MBBlock.h"

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
    protected:

    private:
		
		bool file_not_found;
		bool error;
		string last_error; 
		vector<vector<CMBBlock*>> bodies;
		map<string, vector<vector<double>>> bottom_elevations;
		map<string, map<string, vector<vector<double>>>> properties; 
};

#endif // MODELCREATOR_H

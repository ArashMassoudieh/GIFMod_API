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
        ModelCreator(const string &filename);
        virtual ~ModelCreator();
        ModelCreator(const ModelCreator& other);
        ModelCreator& operator=(const ModelCreator& other);
        bool AddLayer(const string &bodyname, CMedium *M, const string &type, double dx, double dy);
    protected:

    private:
        bool file_not_found;
		bool error;
		vector<vector<CMBBlock*>> bodies;
		vector<vector<double>> vals;
};

#endif // MODELCREATOR_H

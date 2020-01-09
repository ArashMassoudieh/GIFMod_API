#ifndef MODELCREATOR_H
#define MODELCREATOR_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class ModelCreator
{
    public:
        ModelCreator();
        ModelCreator(const string &filename);
        virtual ~ModelCreator();
        ModelCreator(const ModelCreator& other);
        ModelCreator& operator=(const ModelCreator& other);

    protected:

    private:
        bool file_not_found;
		bool error;
		vector<vector<double>> vals;
};

#endif // MODELCREATOR_H

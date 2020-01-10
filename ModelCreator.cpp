#include "ModelCreator.h"
#include <fstream>
#include "StringOP.h"

ModelCreator::ModelCreator()
{
    //ctor
}



ModelCreator::~ModelCreator()
{
    //dtor
}

ModelCreator::ModelCreator(const ModelCreator& other)
{
    //copy ctor
}

ModelCreator& ModelCreator::operator=(const ModelCreator& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

ModelCreator::ModelCreator(const string &filename)
{
    ifstream file(filename);
	if (file.good() == false)
	{
		file_not_found = true;
		error = true;
		return;
	}


	vector<string> s;

	if (file.good())
	while (file.eof()== false)
	{
		s = getline(file);
		if (s.size() <= 1)
		{
			error = true;
//			file.close();
//			return;
		}
		if (s.size()>=2)
		if ((s[0].substr(0,2)!="//") && (tolower(s[0])!="names"))
		{
			vector<double> vals1;
			for (int i=0; i<s.size(); i++)
            {
                vals.push_back(ATOF(s));
            }
		}
	}

	file.close();
}


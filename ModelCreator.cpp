#include "ModelCreator.h"
#include <fstream>
#include "StringOP.h"
#include "Medium.h"

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
			vals.push_back(ATOF(s));
		}
	}

	file.close();
}

bool ModelCreator::AddLayer(const string &bodyname, CMedium *M, const string &type, double dx, double dy)
{
    vector<CMBBlock*> body;
    for (int i=0; i<vals.size(); i++)
        for (int j=0; j<vals[i].size(); j++)
            {
                if (vals[i][j]==-9999)
                    body.push_back(nullptr);
                else
                {
                    #ifdef Debug_API
                        cout << "Block:" << i << j << endl;
                    #endif // Debug_API
                    CMBBlock B1;
                    #ifdef Debug_API
                        cout << "Block:" << i << j <<"has been created" << endl;
                    #endif // Debug_API

                    B1 = CMBBlock("name=" + bodyname + "(" + numbertostring(i) +"."+numbertostring(j) + "), x= " + numbertostring(i*dx) + ", y=" + numbertostring(j*dy) + ", z=" + numbertostring(vals[i][j]) + ", a= " + numbertostring(dx*dy) + ", type="+type + ", z0=" + numbertostring(vals[i][j]));

                    #ifdef Debug_API
                        cout << "Block:" << i << j << "Assigned!" << endl;
                    #endif // Debug_API

                    M->AddBlock(B1);
                    body.push_back(M->Block(bodyname + "(" + numbertostring(i) +"."+numbertostring(j) + ")"));
                }
            }
    bodies.push_back(body);

     for (int i=0; i<vals.size(); i++)
        for (int j=0; j<vals[i].size()-1; j++)
            {
                if (M->Block(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")")!=nullptr && M->Block(bodyname+"(" + numbertostring(i)+"."+numbertostring(j+1)+")")!=nullptr)
                {
                    CConnection C("width=" + numbertostring(dx)  + " ,d= " + numbertostring(dy) + ",name="+type+"("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i)+"."+numbertostring(j+1)+")");
                    M->AddConnector(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")", bodyname+"(" + numbertostring(i)+"."+numbertostring(j+1)+")", C);
                }
            }

    for (int i=0; i<vals.size()-1; i++)
        for (int j=0; j<vals[i].size(); j++)
            {
                if (M->Block(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")")!=nullptr && M->Block(bodyname+"(" + numbertostring(i+1)+"."+numbertostring(j)+")")!=nullptr)
                {
                    CConnection C("width=" + numbertostring(dy) + " ,d= " + numbertostring(dx) + ",name=C("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i+1)+"."+numbertostring(j)+")");
                    M->AddConnector(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")", bodyname+"(" + numbertostring(i+1)+"."+numbertostring(j)+")", C);
                }
            }
    return true;
}

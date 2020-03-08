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
	connectors_properties = other.connectors_properties;
	blocks_properties = other.blocks_properties;
}

ModelCreator& ModelCreator::operator=(const ModelCreator& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
	connectors_properties = rhs.connectors_properties;
	blocks_properties = rhs.blocks_properties;
    return *this;
}

ModelCreator::ModelCreator(const string &filename, const string &blocks_props, const string &connector_props)
{

}

bool ModelCreator::AddBody(const string& bodyname, const string& filename, const string& blocks_props, const string& connector_props)
{
	ifstream file(filename);
	if (file.good() == false)
	{
		file_not_found = true;
		error = true;
		cout<<"File [" + filename + "] was not found!";
		return false;
	}

	vector<string> s;

	if (file.good())
		while (file.eof() == false)
		{
			s = getline(file);
			if (s.size() <= 1)
			{
				error = true;
				//			file.close();
				//			return;
			}
			if (s.size() >= 2)
				if ((s[0].substr(0, 2) != "//") && (tolower(s[0]) != "names"))
				{
					bottom_elevations[bodyname].push_back(ATOF(s));
				}
		}
	connectors_properties[bodyname] = connector_props;
	blocks_properties[bodyname] = blocks_props;
	file.close();
	return true;
}

bool ModelCreator::AddLayer(const string &bodyname, CMedium *M, const string &type, double dx, double dy)
{
    vector<string> block_body;
    vector<string> connector_body;
	if (bottom_elevations.count(bodyname) == 0)
	{
		this->last_error = "No body called [" + bodyname + "] has been defined!";
		cout << last_error << endl;
		return false;
	}
	for (int i=0; i<bottom_elevations[bodyname].size(); i++)
        for (int j=0; j<bottom_elevations[bodyname][i].size(); j++)
            {
                if (bottom_elevations[bodyname][i][j]==-9999)
                    block_body.push_back("");
                else
                {
                    #ifdef Debug_API
                        cout << "Block:" << i << j << endl;
                    #endif // Debug_API
                    CMBBlock B1;
                    #ifdef Debug_API
                        cout << "Block:" << i << j <<"has been created" << endl;
                    #endif // Debug_API

                    B1 = CMBBlock("name=" + bodyname + "(" + numbertostring(i) +"."+numbertostring(j) + "), x= " + numbertostring(i*dx) + ", y=" + numbertostring(j*dy) + ", z=" + numbertostring(bottom_elevations[bodyname][i][j]) + ", a= " + numbertostring(dx*dy) + ", type="+type + ", z0=" + numbertostring(bottom_elevations[bodyname][i][j]));
					B1.set_properties(blocks_properties[bodyname]);
                    #ifdef Debug_API
                        cout << "Block:" << i << j << "Assigned!" << endl;
                    #endif // Debug_API

                    M->AddBlock(B1);
                    block_body.push_back(bodyname + "(" + numbertostring(i) +"."+numbertostring(j) + ")");
                }
            }
    bodies_of_blocks[bodyname]=block_body;

     for (int i=0; i<bottom_elevations[bodyname].size(); i++)
        for (int j=0; j<bottom_elevations[bodyname][i].size()-1; j++)
            {
                if (M->Block(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")")!=nullptr && M->Block(bodyname+"(" + numbertostring(i)+"."+numbertostring(j+1)+")")!=nullptr)
                {
                    CConnection C("width=" + numbertostring(dx)  + " ,d= " + numbertostring(dy) + ",name="+bodyname+"("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i)+"."+numbertostring(j+1)+")");
					C.set_properties(connectors_properties[bodyname]);
					M->AddConnector(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")", bodyname+"(" + numbertostring(i)+"."+numbertostring(j+1)+")", C);
					connector_body.push_back(bodyname+"("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i)+"."+numbertostring(j+1)+")");
                }
            }

    for (int i=0; i<bottom_elevations[bodyname].size()-1; i++)
        for (int j=0; j<bottom_elevations[bodyname][i].size(); j++)
            {
                if (M->Block(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")")!=nullptr && M->Block(bodyname+"(" + numbertostring(i+1)+"."+numbertostring(j)+")")!=nullptr)
                {
                    CConnection C("width=" + numbertostring(dy) + " ,d= " + numbertostring(dx) + ",name="+bodyname+"("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i+1)+"."+numbertostring(j)+")");
					C.set_properties(connectors_properties[bodyname]);
					M->AddConnector(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")", bodyname+"(" + numbertostring(i+1)+"."+numbertostring(j)+")", C);
					connector_body.push_back(bodyname+"("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i+1)+"."+numbertostring(j)+")");
                }
            }
    bodies_of_edges[bodyname]=connector_body;
	for (map<string, vector<vector<double>>>::iterator prop = properties[bodyname].begin(); prop != properties[bodyname].end(); prop++)
	{
		for (int i = 0; i < prop->second.size(); i++)
			for (int j = 0; j < prop->second[i].size(); j++)
			{
				if (M->Block(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + ")") != nullptr)
				{
					M->Block(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + ")")->set_property(prop->first, prop->second[i][j]);
					M->Block(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + ")")->set_properties(blocks_properties[bodyname]);
				}

			}
	}

	{
		for (int i = 0; i < bottom_elevations[bodyname].size(); i++)
			for (int j = 0; j < bottom_elevations[bodyname][i].size() - 1; j++)
			{
				if (M->Connector(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + "-" + numbertostring(i) + "." + numbertostring(j + 1) + ")") && properties[bodyname].count("depth") != 0)
					M->Connector(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + "-" + numbertostring(i) + "." + numbertostring(j + 1) + ")")->set_property("area", 0.5*(properties[bodyname]["depth"][i][j]+ properties[bodyname]["depth"][i][j+1])*dx);

			}

		for (int i = 0; i < bottom_elevations[bodyname].size() - 1; i++)
			for (int j = 0; j < bottom_elevations[bodyname][i].size(); j++)
				if (M->Connector(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + "-" + numbertostring(i + 1) + "." + numbertostring(j) + ")") && properties[bodyname].count("depth")!=0)
					M->Connector(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + "-" + numbertostring(i + 1) + "." + numbertostring(j) + ")")->set_property("area", 0.5 * (properties[bodyname]["depth"][i][j] + properties[bodyname]["depth"][i+1][j]) * dy);

	}
    return true;
}

bool ModelCreator::getproperties(const string &bodyname, const string &prop, const string& filename)
{
	vector<vector<double>> vals;
	ifstream file(filename);
	if (file.good() == false)
	{
		file_not_found = true;
		error = true;
		return false;
	}


	vector<string> s;

	if (file.good())
		while (file.eof() == false)
		{
			s = getline(file);
			if (s.size() <= 1)
			{
				error = true;
				//			file.close();
				//			return;
			}
			if (s.size() >= 2)
				if ((s[0].substr(0, 2) != "//") && (tolower(s[0]) != "names"))
				{
					vals.push_back(ATOF(s));
				}
		}

	properties[bodyname][prop] = vals;
	file.close();

	return true;
}

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
	BBodies = other.BBodies; 
	CBodies = other.CBodies; 
	
}

ModelCreator& ModelCreator::operator=(const ModelCreator& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
	BBodies = rhs.BBodies;
	CBodies = rhs.CBodies;
	
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
					BBodies[bodyname].bottom_elevations.push_back(ATOF(s));
				}
		}
	CBodies[bodyname].connectors_properties = connector_props;
	BBodies[bodyname].blocks_properties = blocks_props;
	file.close();
	return true;
}

bool ModelCreator::AddLayer(const string &bodyname, CMedium *M, const string &type, double dx, double dy)
{
    vector<_location> block_body;
    vector<string> connector_body;
	if (BBodies.count(bodyname) == 0)
	{
		this->last_error = "No body called [" + bodyname + "] has been defined!";
		cout << last_error << endl;
		return false;
	}
	for (int i=0; i<BBodies[bodyname].bottom_elevations.size(); i++)
        for (int j=0; j<BBodies[bodyname].bottom_elevations[i].size(); j++)
            {
                if (BBodies[bodyname].bottom_elevations[i][j]==-9999)
                {
                    //_location loc;
                    //block_body.push_back(loc);
                }
                else
                {
                    #ifdef Debug_API
                        cout << "Block:" << i << j << endl;
                    #endif // Debug_API
                    CMBBlock B1;
                    #ifdef Debug_API
                        cout << "Block:" << i << j <<"has been created" << endl;
                    #endif // Debug_API

                    B1 = CMBBlock("name=" + bodyname + "(" + numbertostring(i) +"."+numbertostring(j) + "), x= " + numbertostring(i*dx) + ", y=" + numbertostring(j*dy) + ", z=" + numbertostring(BBodies[bodyname].bottom_elevations[i][j]) + ", a= " + numbertostring(dx*dy) + ", type="+type + ", z0=" + numbertostring(BBodies[bodyname].bottom_elevations[i][j]));
					B1.set_properties(BBodies[bodyname].blocks_properties);
                    #ifdef Debug_API
                        cout << "Block:" << i << j << "Assigned!" << endl;
                    #endif // Debug_API

                    M->AddBlock(B1);
                    _location loc;
                    loc.i = i;
                    loc.j = j;
                    loc.name = bodyname + "(" + numbertostring(i) +"."+numbertostring(j) + ")";
                    block_body.push_back(loc);

                }
            }
    BBodies[bodyname].bodies_of_blocks=block_body;

     for (int i=0; i< BBodies[bodyname].bottom_elevations.size(); i++)
        for (int j=0; j< BBodies[bodyname].bottom_elevations[i].size()-1; j++)
            {
                if (M->Block(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")")!=nullptr && M->Block(bodyname+"(" + numbertostring(i)+"."+numbertostring(j+1)+")")!=nullptr)
                {
                    CConnection C("width=" + numbertostring(dx)  + " ,d= " + numbertostring(dy) + ",name="+bodyname+"("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i)+"."+numbertostring(j+1)+")");
					C.set_properties(CBodies[bodyname].connectors_properties);
					M->AddConnector(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")", bodyname+"(" + numbertostring(i)+"."+numbertostring(j+1)+")", C);
					connector_body.push_back(bodyname+"("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i)+"."+numbertostring(j+1)+")");
                }
            }

    for (int i=0; i< BBodies[bodyname].bottom_elevations.size()-1; i++)
        for (int j=0; j< BBodies[bodyname].bottom_elevations[i].size(); j++)
            {
                if (M->Block(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")")!=nullptr && M->Block(bodyname+"(" + numbertostring(i+1)+"."+numbertostring(j)+")")!=nullptr)
                {
                    CConnection C("width=" + numbertostring(dy) + " ,d= " + numbertostring(dx) + ",name="+bodyname+"("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i+1)+"."+numbertostring(j)+")");
					C.set_properties(CBodies[bodyname].connectors_properties);
					M->AddConnector(bodyname+"(" + numbertostring(i)+"."+numbertostring(j)+")", bodyname+"(" + numbertostring(i+1)+"."+numbertostring(j)+")", C);
					connector_body.push_back(bodyname+"("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i+1)+"."+numbertostring(j)+")");
                }
            }
    CBodies[bodyname].bodies_of_edges=connector_body;
	for (map<string, vector<vector<double>>>::iterator prop = BBodies[bodyname].properties.begin(); prop != BBodies[bodyname].properties.end(); prop++)
	{
		for (int i = 0; i < prop->second.size(); i++)
			for (int j = 0; j < prop->second[i].size(); j++)
			{
				if (M->Block(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + ")") != nullptr)
				{
					M->Block(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + ")")->set_property(prop->first, prop->second[i][j]);
					M->Block(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + ")")->set_properties(BBodies[bodyname].blocks_properties);
				}

			}
	}

	{
		for (int i = 0; i < BBodies[bodyname].bottom_elevations.size(); i++)
			for (int j = 0; j < BBodies[bodyname].bottom_elevations[i].size() - 1; j++)
			{
				if (M->Connector(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + "-" + numbertostring(i) + "." + numbertostring(j + 1) + ")") && BBodies[bodyname].properties.count("depth") != 0)
					M->Connector(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + "-" + numbertostring(i) + "." + numbertostring(j + 1) + ")")->set_property("area", 0.5*(BBodies[bodyname].properties["depth"][i][j]+ BBodies[bodyname].properties["depth"][i][j+1])*dx);

			}

		for (int i = 0; i < BBodies[bodyname].bottom_elevations.size() - 1; i++)
			for (int j = 0; j < BBodies[bodyname].bottom_elevations[i].size(); j++)
				if (M->Connector(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + "-" + numbertostring(i + 1) + "." + numbertostring(j) + ")") && BBodies[bodyname].properties.count("depth")!=0)
					M->Connector(bodyname + "(" + numbertostring(i) + "." + numbertostring(j) + "-" + numbertostring(i + 1) + "." + numbertostring(j) + ")")->set_property("area", 0.5 * (BBodies[bodyname].properties["depth"][i][j] + BBodies[bodyname].properties["depth"][i+1][j]) * dy);

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

	BBodies[bodyname].properties[prop] = vals;
	file.close();

	return true;
}

bool ModelCreator::ConnectBodiesVertical(const string &newconnectorbodyname, CMedium *M, const string &sourcebody, const string &targetbody, const string &connector_properties, double sourcebodycoeffientinlenght, double targetbodycoefficientinlength)
{
    if (BBodies.count(sourcebody)==0)
    {
        cout << "No body called [" + sourcebody + "] exists!" << endl;
        return false;
    }

    if (BBodies.count(targetbody)==0)
    {
        cout << "No body called [" + targetbody + "] exists!" << endl;
        return false;
    }
    if (BBody(sourcebody).size() != BBody(targetbody).size())
    {
        cout<< "The size of bodies [" + sourcebody + "] and [" +targetbody + "] must be equal!" << endl;
        return false;
    }
    vector<string> connector_body;
    for (int i=0; i<BBody(sourcebody).size(); i++)
    {
        if (BBody(sourcebody)[i].name!="" && BBody(targetbody)[i].name!="")
        {
            CMBBlock* SourceBlock = M->Block(BBody(sourcebody)[i].name);
            CMBBlock* TargetBlock = M->Block(BBody(targetbody)[i].name);
            int ii = BBody(targetbody)[i].i;
            int jj = BBody(targetbody)[i].j;
            double area = 0.5*(SourceBlock->get_property("area") + TargetBlock->get_property("area"));
            double length = SourceBlock->get_property("depth")*sourcebodycoeffientinlenght + TargetBlock->get_property("depth")*targetbodycoefficientinlength;
            CConnection C("area=" + numbertostring(area) + " ,d= " + numbertostring(length) + ",name="+newconnectorbodyname+"("+numbertostring(ii)+"."+numbertostring(jj)+")");
            C.set_properties(connector_properties);
            M->AddConnector(SourceBlock->ID, TargetBlock->ID, C);
            connector_body.push_back(newconnectorbodyname+"("+numbertostring(ii)+"."+numbertostring(jj)+")");
        }
    }
	CBodies[newconnectorbodyname].bodies_of_edges = connector_body;
    return true;
}

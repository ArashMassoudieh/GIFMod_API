#include <iostream>
#include "MediumSet.h"


using namespace std;

int main()
{
    CMBBlock B1("name=myBlock1, a=10, type=pond, S=1, z0=0");
    CMBBlock B2("name=myBlock2, a=10, type=pond, S=2, z0=0");
    CMedium M(true);
    M.AddBlock(B1);
    M.AddBlock(B2);
    CConnection C("nmanning=0.00001, width=1, name=C, d=1");
    M.AddConnector("myBlock1","myBlock2",C);
    M.set_properties("tstart=0, tend=1, dt=0.1");
    M.set_default_params();
    cout<<M.Connector("C")->tostring();
    cout<<M.Block("myBlock1")->tostring();
    cout<<M.Block("myBlock2")->tostring();
    M.write_details() = true;
    M.solve();
    M.Results.ANS.writetofile("text.txt");
}

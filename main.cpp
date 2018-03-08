#include <iostream>
#include "MediumSet.h"


using namespace std;

int main()
{
    CMBBlock B1("Name=myBlock1, area = 10, type=pond, depth=2, s=1");
    CMBBlock B2("Name=myBlock2, area = 10, type=pond, depth=2, s=2");
    CConnection C("width=1, nmanning=0.00001");
    CMedium M(true);
    M.showmessages = true;
    M.set_properties("name=myModel, dt=0.01, tstart=0, tend=1");
    M.AddBlock(B1);
    M.AddBlock(B2);
    M.AddConnector("myBlock1","myBlock2",C);
    M.set_default_params();
    M.set_properties("dt=0.1");
    M.solve();
    M.Results.ANS.writetofile("output.txt");
}

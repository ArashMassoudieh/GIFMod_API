#include <iostream>
#include "MediumSet.h"


using namespace std;

int main()
{
    CMBBlock B("Name=myBlock, area = 10, type=soil, depth=2" );
    CMedium M(true);
    M.showmessages = true;
    M.set_properties("name=myModel, dt=0.01, tstart=0, tend=1");
    M.AddBlock(B);
    M.set_default_params();
    M.set_properties("dt=0.1");
    M.solve();
    M.Results.ANS.writetofile("output.txt");
}

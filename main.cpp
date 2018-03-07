#include <iostream>
#include "MediumSet.h"


using namespace std;

int main()
{
    CMBBlock B("Name=myBlock, area = 10, type=soil, depth=2" );
    CMedium M;
    M.showmessages = true;
    M.set_properties("name=myModel");
    M.AddBlock(B);
    M.set_default_params();
    M.set_properties("dt=0.1");
    M.solve();
}

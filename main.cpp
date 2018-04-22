#include <iostream>
#include "MediumSet.h"

using namespace std;

int main()
{
    //VTK_test V;
    //V.TDPoints();
    //V.Point();
    int nx = 5;
    int ny = 5;
    int nz = 5;
    double dx = 100;
    double dy = 100;
    double dz = 1;
    //CMBBlock Soil("name=Soil, a=10000, type=soil, theta_s=0.4, theta_r=0.1, S=4000, vg_n=3, vg_m=0.667, vg_alpha=1, lambda=0.5, z0= " + numbertostring(k*3) +", V=30000, ks=0.1");

    //cout<<Soil.tostring()<<endl;


    CMedium M(true);
    cout<<"Blocks..."<< endl;
    for (int i=0; i<nx; i++)
        for (int j=0; j<ny; j++)
            for (int k=0; k<nz; k++)
            {
                #ifdef Debug_API
                    cout << "Block:" << i << j << k << endl;
                #endif // Debug_API
                CMBBlock B1;
                #ifdef Debug_API
                    cout << "Block:" << i << j << k << "has been created" << endl;
                #endif // Debug_API
                if (k!=0)
                    B1 = CMBBlock("name=Soil(" + numbertostring(i) +"."+numbertostring(j) + "." + numbertostring(k) + "), x= " + numbertostring(i*100) + ", y=" + numbertostring(j*100) + ", z=" + numbertostring(k*dz+0.01*dx*i+0.02*dy*j) + ", a= " + numbertostring(dx*dy) + ", type=soil, theta_s=0.4, theta_r=0.1, S=" + numbertostring(dx*dy*dz*(0.4-k/nz*0.2) )+" , vg_n=3, vg_m=0.667, vg_alpha=1, lambda=0.5, z0=" + numbertostring(k*dz+0.01*dx*i+0.02*dy*j) +", V=" + numbertostring(dx*dy*dz) + ", ks=20");

                else
                    B1 = CMBBlock("name=Soil(" + numbertostring(i) +"."+numbertostring(j) + "." + numbertostring(k) + "), x= " + numbertostring(i*100) + ", y=" + numbertostring(j*100) + ", z=" + numbertostring(k*dz+0.01*dx*i+0.02*dy*j) + ",a= " + numbertostring(dx*dy) + ", type=soil, theta_s=0.4, theta_r=0.1, S=" + numbertostring(dx*dy*dz*0.4) + ", vg_n=3, vg_m=0.667, vg_alpha=1, lambda=0.5, z0=" + numbertostring(k*dz+0.01*dx*i+0.02*dy*j) +", V= " + numbertostring(dx*dy*dz) + ", ks=20");
                #ifdef Debug_API
                    cout << "Block:" << i << j << k << "Assigned!" << endl;
                #endif // Debug_API

                M.AddBlock(B1);
            }

    //CMBBlock B1("name=myBlock1, a=10, type=pond, S=1, z0=0");
    //CMBBlock B2("name=myBlock2, a=10, type=pond, S=2, z0=0");
    cout<<"Connectors_1"<< endl;
    for (int i=0; i<nx; i++)
        for (int j=0; j<ny; j++)
            for (int k=0; k<nz-1; k++)
            {
                CConnection C("a=" + numbertostring(dx*dy) + " ,d= " + numbertostring(dz) + ",name=C("+numbertostring(i)+"."+numbertostring(j)+"."+numbertostring(k)+"-"+numbertostring(i)+"."+numbertostring(j)+"."+numbertostring(k+1)+")");
                M.AddConnector("Soil(" + numbertostring(i)+"."+numbertostring(j)+"."+numbertostring(k)+")", "Soil(" + numbertostring(i)+"."+numbertostring(j)+"."+numbertostring(k+1)+")", C);
            }

    cout<<"Connectors_2"<< endl;
    for (int i=0; i<nx; i++)
        for (int j=0; j<ny-1; j++)
            for (int k=0; k<nz; k++)
            {
                CConnection C("a=" + numbertostring(dx*dz)  + " ,d= " + numbertostring(dy) + ",name=C("+numbertostring(i)+"."+numbertostring(j)+"."+numbertostring(k)+"-"+numbertostring(i)+"."+numbertostring(j+1)+"."+numbertostring(k)+")");
                M.AddConnector("Soil(" + numbertostring(i)+"."+numbertostring(j)+"."+numbertostring(k)+")", "Soil(" + numbertostring(i)+"."+numbertostring(j+1)+"."+numbertostring(k)+")", C);
            }

    cout<<"Connectors_3"<< endl;
    for (int i=0; i<nx-1; i++)
        for (int j=0; j<ny; j++)
            for (int k=0; k<nz; k++)
            {
                CConnection C("a=" + numbertostring(dy*dz) + " ,d= " + numbertostring(dx) + ",name=C("+numbertostring(i)+"."+numbertostring(j)+"."+numbertostring(k)+"-"+numbertostring(i+1)+"."+numbertostring(j)+"."+numbertostring(k)+")");
                M.AddConnector("Soil(" + numbertostring(i)+"."+numbertostring(j)+"."+numbertostring(k)+")", "Soil(" + numbertostring(i+1)+"."+numbertostring(j)+"."+numbertostring(k)+")", C);
            }

    //M.AddBlock(B2);
    //CConnection C("nmanning=0.00001, width=1, name=C, d=1");
    //M.AddConnector("myBlock1","myBlock2",C);
    M.set_properties("tstart=0, tend=10000, dt=10");
    M.set_default_params();
    M.max_dt() = 100;
    //cout<<M.Connector("C")->tostring();
    //cout<<M.Block("myBlock1")->tostring();
    //cout<<M.Block("myBlock2")->tostring();
    M.write_details() = true;
    #ifdef USE_VTK
    VTK_grid gr = M.VTK_get_snap_shot("z0",0,1);
    M.merge_to_snapshot(gr,"ks");
    M.write_grid_to_vtp(gr,"test_1.vtu");
    M.write_grid_to_text(gr, "test_1.txt");
    #endif // USE_VTK
    M.solve();


    #ifdef USE_VTK
    M.merge_to_snapshot(gr, "theta", 1);
    M.merge_to_snapshot(gr, "theta", 5);
    M.merge_to_snapshot(gr, "theta", 25);
    M.merge_to_snapshot(gr, "theta", 50);
    M.merge_to_snapshot(gr, "theta", 100);
    M.merge_to_snapshot(gr, "theta", 200);
    M.merge_to_snapshot(gr, "theta", 500);
    M.merge_to_snapshot(gr, "theta", 1000);
    M.merge_to_snapshot(gr, "theta", 2000);
    M.merge_to_snapshot(gr, "theta", 5000);
    M.merge_to_snapshot(gr, "theta", 10000);
    M.write_grid_to_text(gr, "grid_1.txt");
    M.write_grid_to_vtp(gr,"test.vtu");
    #endif // USE_VTK

    M.Results.ANS.writetofile(string("text.txt"));
}

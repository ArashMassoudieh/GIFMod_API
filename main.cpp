#include <iostream>
#include "MediumSet.h"
#include "ModelCreator.h"

using namespace std;

int main()
{
    //VTK_test V;
    //V.TDPoints();
    //V.Point();
    int nx = 10;
    int ny = 10;
    int nz = 3;
    double dx = 100;
    double dy = 100;
    double dz = 1;
    //CMBBlock Soil("name=Soil, a=10000, type=soil, theta_s=0.4, theta_r=0.1, S=4000, vg_n=3, vg_m=0.667, vg_alpha=1, lambda=0.5, z0= " + numbertostring(k*3) +", V=30000, ks=0.1");

    ModelCreator mCreate("/home/arash/Projects/GIFMOD_API_results/topo_sligo.txt");
    //cout<<Soil.tostring()<<endl;

    CMatrix Topo("/home/arash/Projects/GIFMOD_API_results/Topo_info/GSurf.txt");
    CMatrix top_layer("/home/arash/Projects/GIFMOD_API_results/Topo_info/Layer1_B.txt");
    CMatrix mid_layer("/home/arash/Projects/GIFMOD_API_results/Topo_info/Layer2_B.txt");
    CMatrix bed_rock("/home/arash/Projects/GIFMOD_API_results/Topo_info/BedRock.txt");
    top_layer.writetofile("/home/arash/Projects/GIFMOD_API_results/Topo_info/top_layer_out.txt");
    mid_layer.writetofile("/home/arash/Projects/GIFMOD_API_results/Topo_info/mid_layer_out.txt");
    bed_rock.writetofile("/home/arash/Projects/GIFMOD_API_results/Topo_info/bed_rock.txt");

    CMedium M(true);
    cout<<"Blocks..."<< endl;

    for (int i=0; i<nx; i++)
        for (int j=0; j<ny; j++)
            {
                #ifdef Debug_API
                    cout << "Block:" << i << j << endl;
                #endif // Debug_API
                CMBBlock B1;
                #ifdef Debug_API
                    cout << "Block:" << i << j <<"has been created" << endl;
                #endif // Debug_API

                B1 = CMBBlock("name=Catchment(" + numbertostring(i) +"."+numbertostring(j) + "), x= " + numbertostring(i*dx) + ", y=" + numbertostring(j*dy) + ", z=" + numbertostring(Topo[i][j]) + ", a= " + numbertostring(dx*dy) + ", type=catchment, z0=" + numbertostring(Topo[i][j]) + ",nmanning=1e-6, precipitation=yes");

                #ifdef Debug_API
                    cout << "Block:" << i << j << "Assigned!" << endl;
                #endif // Debug_API

                M.AddBlock(B1);
            }

    for (int i=0; i<nx; i++)
        for (int j=0; j<ny-1; j++)
            {
                CConnection C("width=" + numbertostring(dx)  + " ,d= " + numbertostring(dy) + ",name=C("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i)+"."+numbertostring(j+1)+"), nmanning=1e-6");
                M.AddConnector("Catchment(" + numbertostring(i)+"."+numbertostring(j)+")", "Catchment(" + numbertostring(i)+"."+numbertostring(j+1)+")", C);
            }

    for (int i=0; i<nx-1; i++)
        for (int j=0; j<ny; j++)
            {
                CConnection C("width=" + numbertostring(dy) + " ,d= " + numbertostring(dx) + ",name=C("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i+1)+"."+numbertostring(j)+"), nmanning=1e-6");
                M.AddConnector("Catchment(" + numbertostring(i)+"."+numbertostring(j)+")", "Catchment(" + numbertostring(i+1)+"."+numbertostring(j)+")", C);
            }

    //********************  top soil ***********************

    for (int i=0; i<nx; i++)
        for (int j=0; j<ny; j++)
            {
                CMBBlock B1 = CMBBlock("name=TopSoil(" + numbertostring(i) +"."+numbertostring(j) + "), x= " + numbertostring(i*dx) + ", y=" + numbertostring(j*dy) + ", z=" + numbertostring(0.5*(Topo[i][j]+top_layer[i][j])) + ", a= " + numbertostring(dx*dy) + ", type=soil, theta_s=0.4, theta_r=0.1, S=" + numbertostring(dx*dy*(Topo[i][j]-top_layer[i][j])*(0.4-double(1)/double(nz)*0.2) )+" , vg_n=3, vg_m=0.667, vg_alpha=1, lambda=0.5, z0=" + numbertostring(top_layer[i][j]) +", V=" + numbertostring(dx*dy*(Topo[i][j]-top_layer[i][j])) + ", ks=20");
                M.AddBlock(B1);
            }

    for (int i=0; i<nx; i++)
        for (int j=0; j<ny; j++)
            {
                CConnection C("a=" + numbertostring(dx*dy) + " ,d= " + numbertostring((Topo[i][j]-top_layer[i][j])/2) + ",name=TopC("+numbertostring(i)+"."+numbertostring(j)+")");
                M.AddConnector("Catchment(" + numbertostring(i)+"."+numbertostring(j)+")", "TopSoil(" + numbertostring(i)+"."+numbertostring(j)+")", C);
            }


    for (int i=0; i<nx; i++)
        for (int j=0; j<ny-1; j++)
            {
                CConnection C("a=" + numbertostring(dx*0.5*(Topo[i][j]-top_layer[i][j]+Topo[i][j+1]-top_layer[i][j+1]))  + " ,d= " + numbertostring(dy) + ",name=C_topsoil("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i)+"."+numbertostring(j+1)+")");
                M.AddConnector("TopSoil(" + numbertostring(i)+"."+numbertostring(j)+")", "TopSoil(" + numbertostring(i)+"."+numbertostring(j+1)+")", C);
            }


    for (int i=0; i<nx-1; i++)
        for (int j=0; j<ny; j++)
            {
                CConnection C("a=" + numbertostring(dy*0.5*(Topo[i][j]-top_layer[i][j]+Topo[i+1][j]-top_layer[i+1][j]))  + " ,d= " + numbertostring(dx) + ",name=C_topsoil("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i+1)+"."+numbertostring(j)+")");
                M.AddConnector("TopSoil(" + numbertostring(i)+"."+numbertostring(j)+")", "TopSoil(" + numbertostring(i+1)+"."+numbertostring(j)+")", C);
            }

    //********************  mid soil ***********************
    for (int i=0; i<nx; i++)
        for (int j=0; j<ny; j++)
            {
                CMBBlock B1 = CMBBlock("name=MidSoil(" + numbertostring(i) +"."+numbertostring(j) + "), x= " + numbertostring(i*dx) + ", y=" + numbertostring(j*dy) + ", z=" + numbertostring(0.5*(mid_layer[i][j]+top_layer[i][j])) + ", a= " + numbertostring(dx*dy) + ", type=soil, theta_s=0.4, theta_r=0.1, S=" + numbertostring(dx*dy*(top_layer[i][j]-mid_layer[i][j])*(0.4-double(2)/double(nz)*0.2) )+" , vg_n=3, vg_m=0.667, vg_alpha=1, lambda=0.5, z0=" + numbertostring(mid_layer[i][j]) +", V=" + numbertostring(dx*dy*(top_layer[i][j]-mid_layer[i][j])) + ", ks=20");
                M.AddBlock(B1);
            }

    for (int i=0; i<nx; i++)
        for (int j=0; j<ny; j++)
            {
                CConnection C("a=" + numbertostring(dx*dy) + " ,d= " + numbertostring((Topo[i][j]-mid_layer[i][j])/2) + ",name=MidC("+numbertostring(i)+"."+numbertostring(j)+")");
                M.AddConnector("TopSoil(" + numbertostring(i)+"."+numbertostring(j)+")", "MidSoil(" + numbertostring(i)+"."+numbertostring(j)+")", C);
            }


    for (int i=0; i<nx; i++)
        for (int j=0; j<ny-1; j++)
            {
                CConnection C("a=" + numbertostring(dx*0.5*(top_layer[i][j]-mid_layer[i][j]+top_layer[i][j+1]-mid_layer[i][j+1]))  + " ,d= " + numbertostring(dy) + ",name=C_midsoil("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i)+"."+numbertostring(j+1)+")");
                M.AddConnector("MidSoil(" + numbertostring(i)+"."+numbertostring(j)+")", "MidSoil(" + numbertostring(i)+"."+numbertostring(j+1)+")", C);
            }


    for (int i=0; i<nx-1; i++)
        for (int j=0; j<ny; j++)
            {
                CConnection C("a=" + numbertostring(dy*0.5*(top_layer[i][j]-mid_layer[i][j]+top_layer[i+1][j]-mid_layer[i+1][j]))  + " ,d= " + numbertostring(dx) + ",name=C_midsoil("+numbertostring(i)+"."+numbertostring(j)+"-"+numbertostring(i+1)+"."+numbertostring(j)+")");
                M.AddConnector("MidSoil(" + numbertostring(i)+"."+numbertostring(j)+")", "MidSoil(" + numbertostring(i+1)+"."+numbertostring(j)+")", C);
            }


/*

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

  */
    //M.AddBlock(B2);
    //CConnection C("nmanning=0.00001, width=1, name=C, d=1");
    //M.AddConnector("myBlock1","myBlock2",C);
    M.Precipitation_filename.push_back("/home/arash/Projects/GIFMOD_API_results/rain.txt");
    M.set_properties("tstart=0, tend=100, dt=0.1");
    M.f_load_inflows();
    M.set_default_params();
    M.max_dt() = 1;
    cout<<M.Connector("C("+numbertostring(1)+"."+numbertostring(1)+"-"+numbertostring(1)+"."+numbertostring(1+1)+")")->tostring();
    //cout<<M.Block("myBlock1")->tostring();
    //cout<<M.Block("myBlock2")->tostring();
    M.write_details() = true;
    #ifdef USE_VTK
    VTK_grid gr = M.VTK_get_snap_shot("z0",0,1);
    //M.merge_to_snapshot(gr,"ks");
    M.write_grid_to_vtp_surf(gr,"/home/arash/Projects/GIFMOD_API_results/surf.vtp");
    //M.write_grid_to_vtp_surf(gr,"/home/arash/Projects/GIFMOD_API_results/test_1.vtu");
    M.write_grid_to_text(gr, "/home/arash/Projects/GIFMOD_API_results/test_1.txt");
    #endif // USE_VTK
    M.write_details() = false;
    M.solution_method() = "Direct Solution";
    M.solve();
    #ifdef USE_VTK
    for (double t=0; t<100; t+=5)
    {
        VTK_grid moisture = M.VTK_get_snap_shot("theta",t,10,"theta");
        VTK_grid depth = M.VTK_get_snap_shot("depth",t,10,"depth");
        cout<<"writing depths ..."<<endl;
        M.write_grid_to_vtp_surf(depth,"/home/arash/Projects/GIFMOD_API_results/water_depth" + numbertostring(t)+ ".vtp");
        M.write_grid_to_text(depth,"/home/arash/Projects/GIFMOD_API_results/water_depth" + numbertostring(t)+ ".txt");
        cout<<"writing moistures ..."<<endl;
        M.write_grid_to_vtp(moisture,"/home/arash/Projects/GIFMOD_API_results/moisture" + numbertostring(t)+ ".vtu");
        M.write_grid_to_text(moisture,"/home/arash/Projects/GIFMOD_API_results/moisture" + numbertostring(t)+ ".txt");

    }

    #endif // USE_VTK

    M.Results.ANS.writetofile(string("text.txt"));
}

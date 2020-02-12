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
	
#ifndef Windows
    ModelCreator mCreate("/home/arash/Projects/GIFMOD_API_results/topo_sligo.csv");
#else
	ModelCreator mCreate("C:\\Projects\\GIFMod_API_Projects\\topo_sligo.csv","","nmanning=0.00001");
#endif
    //cout<<Soil.tostring()<<endl;

    //CMatrix Topo("/home/arash/Projects/GIFMOD_API_results/Topo_info/GSurf.txt");
    //CMatrix top_layer("/home/arash/Projects/GIFMOD_API_results/Topo_info/Layer1_B.txt");
    //CMatrix mid_layer("/home/arash/Projects/GIFMOD_API_results/Topo_info/Layer2_B.txt");
    //CMatrix bed_rock("/home/arash/Projects/GIFMOD_API_results/Topo_info/BedRock.txt");
    //top_layer.writetofile("/home/arash/Projects/GIFMOD_API_results/Topo_info/top_layer_out.txt");
    //mid_layer.writetofile("/home/arash/Projects/GIFMOD_API_results/Topo_info/mid_layer_out.txt");
    //bed_rock.writetofile("/home/arash/Projects/GIFMOD_API_results/Topo_info/bed_rock.txt");

    CMedium M(true);
    mCreate.AddLayer("surface",&M,"Catchment",331,508);

    cout<<"Blocks..."<< endl;
	
#ifdef Windows
	M.Precipitation_filename.push_back("C:\\Projects\\GIFMod_API_Projects\\rain.txt");
#else
    M.Precipitation_filename.push_back("/home/arash/Projects/GIFMOD_API_results/rain.txt");
#endif
    M.set_properties("tstart=0, tend=1, dt=0.1");
    M.f_load_inflows();
    M.set_default_params();
    M.max_dt() = 1;
    //cout<<M.Connector("C("+numbertostring(1)+"."+numbertostring(1)+"-"+numbertostring(1)+"."+numbertostring(1+1)+")")->tostring();
    //cout<<M.Block("myBlock1")->tostring();
    //cout<<M.Block("myBlock2")->tostring();
    M.write_details() = true;
    #ifdef USE_VTK
    VTK_grid gr = M.VTK_get_snap_shot("z0",0,1);
    //M.merge_to_snapshot(gr,"ks");
    M.write_grid_to_vtp_surf(gr,"C:\\Projects\\GIFMod_API_Projects\\surf.vtp");
    //M.write_grid_to_vtp_surf(gr,"/home/arash/Projects/GIFMOD_API_results/test_1.vtu");
    M.write_grid_to_text(gr, "C:\\Projects\\GIFMod_API_Projects\\test_1.txt");
    #endif // USE_VTK
    M.write_details() = false;
    M.solution_method() = "Direct Solution";
    M.solve();
    #ifdef USE_VTK
    for (double t=0; t<1; t+=1)
    {
        VTK_grid moisture = M.VTK_get_snap_shot("theta",t,1,"theta");
		VTK_edge_grid Flow = M.VTK_get_snap_shot_edges("Q", t, 1, "Q");
        VTK_grid depth = M.VTK_get_snap_shot("depth",t,1,"depth");
        cout<<"writing depths ..."<<endl;
        M.write_grid_to_vtp_surf(depth,"C:\\Projects\\GIFMod_API_Projects\\water_depth" + numbertostring(t)+ ".vtp");
        M.write_grid_to_text(depth,"C:\\Projects\\GIFMod_API_Projects\\water_depth" + numbertostring(t)+ ".txt");
		M.write_grid_to_vtp_surf(Flow, "C:\\Projects\\GIFMod_API_Projects\\flow" + numbertostring(t) + ".vtp", "Q");
        //cout<<"writing moistures ..."<<endl;
        //M.write_grid_to_vtp(moisture,"/home/arash/Projects/GIFMOD_API_results/moisture" + numbertostring(t)+ ".vtu");
        //M.write_grid_to_text(moisture,"/home/arash/Projects/GIFMOD_API_results/moisture" + numbertostring(t)+ ".txt");

    }

    #endif // USE_VTK

    M.Results.ANS.writetofile(string("C:\\Projects\\GIFMod_API_Projects\\text.txt"));
}

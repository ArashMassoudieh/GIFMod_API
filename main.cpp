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
#ifdef  Windows
	string inputpath = "C:\\Projects\\GIFMod_API_Projects\\";
	string outputpath = "C:\\Projects\\GIFMod_API_Projects\\outputs\\";
#else 
	string inputpath = "/home/arash/Projects/GIFMOD_API_results/";
	string outputpath = "/home/arash/Projects/GIFMOD_API_results/output/";
#endif //  Windows

	ModelCreator mCreate;

    mCreate.AddBody(string("surface"),string(inputpath + "topo_sligo.csv"),"","nmanning=0.00001");
    mCreate.AddBody("soil", inputpath + "bedrock.csv", "theta=0.35, porosity=0.4, ks=0.1, theta_s=0.4, theta_r=0.05, vg_alpha=3.6, vg_n=1.56, lambda=0.5, storativity=0.01","");
	mCreate.getproperties("soil", "depth", inputpath + "depth.csv");

    CMedium M(true);
    mCreate.AddLayer("surface",&M,"Catchment",331,508);
	mCreate.AddLayer("soil", &M, "Soil", 331, 508);
	mCreate.ConnectBodiesVertical("infiltration",&M, "surface", "soil","",0,0.5);
    //cout<<"Blocks..."<< endl;
	
	// Creating the downstream boundary condition
	CMBBlock DownstreamBC("name=DSBC, hs_relationship=16, area=100, z0=0, type=pond, depth=1"); 
	M.AddBlock(DownstreamBC);
	CConnection DownStreamBC_connect("name=DSBC_c, width = 331, d=250, nmanning=0.00001");
	M.AddConnector("surface(19.1)", "DSBC", DownStreamBC_connect);
	M.AddConnector("soil(19.1)", "DSBC", DownStreamBC_connect); 
	// Creating the downstream boundary condition done!

	M.Precipitation_filename.push_back(inputpath + "rain.txt");
    M.set_properties("tstart=0, tend=100, dt=0.1");

	M.outputpathname() = outputpath;
    M.f_load_inflows();
    M.set_default_params();
    M.max_dt() = 1;
    M.write_details() = true;
    #ifdef USE_VTK
    cout << "getting surface elevation ..." << endl;
    VTK_grid gr = M.VTK_get_snap_shot("surface",&mCreate ,"z0",0,1);
    
	cout << "writing surf to vtp ..." << endl;
    M.write_grid_to_vtp_surf(gr, outputpath + "surf.vtp");
    M.write_grid_to_text(gr, outputpath + "test_1.txt");
    
    cout << "writing surf to txt ..." << endl;
    M.write_grid_to_text(gr, outputpath + "surf.txt");

    cout << "Solving ..." << endl;

    #endif // USE_VTK
    M.write_details() = false;
    M.solution_method() = "Direct Solution";
    M.solve();
    #ifdef USE_VTK
    cout << "Creating Outputs" << endl;
    for (double t=0; t<100; t+=1)
    {
        VTK_grid moisture = M.VTK_get_snap_shot("theta",t,1,"theta");
		VTK_edge_grid Surface_Flow = M.VTK_get_snap_shot_edges("surface", &mCreate, "Q", t, 1, "Q");
		VTK_edge_grid Subsurface_Flow = M.VTK_get_snap_shot_edges("soil", &mCreate, "Q", t, 1, "Q");
		VTK_edge_grid Infiltration_Flow = M.VTK_get_snap_shot_edges("infiltration", &mCreate, "Q", t, 1, "Q");
        VTK_grid depth = M.VTK_get_snap_shot("depth",t,1,"depth");
        VTK_grid Infiltration_flow_surf = Infiltration_Flow.toVTKGtid();

        
        M.write_grid_to_vtp_surf(depth, outputpath + "water_depth" + numbertostring(t) + ".vtp");
		M.write_grid_to_text(depth, outputpath + "water_depth" + numbertostring(t) + ".txt");
		M.write_grid_to_vtp_surf(Surface_Flow, outputpath + "surface_flow" + numbertostring(t) + ".vtp", "Q");
		M.write_grid_to_vtp_surf(Subsurface_Flow, outputpath + "subsurface_flow" + numbertostring(t) + ".vtp", "Q");
		M.write_grid_to_vtp_surf(Infiltration_Flow, outputpath + "infiltration_flow" + numbertostring(t) + ".vtp", "Q");
		M.write_grid_to_vtp_surf(moisture, outputpath + "moisture" + numbertostring(t) + ".vtp");
		M.write_grid_to_vtp_surf(Infiltration_flow_surf, outputpath + "infiltration_surf" + numbertostring(t) + ".vtp");
		M.write_grid_to_text(moisture, outputpath + "moisture" + numbertostring(t) + ".txt");
        
    }

    #endif // USE_VTK
    
    M.Results.ANS.writetofile(string(outputpath + "output.txt"));
    
}

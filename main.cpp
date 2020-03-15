#include <iostream>
#include "MediumSet.h"
#include "ModelCreator.h"

using namespace std;

int main()
{

#ifdef  Windows
	string inputpath = "E:\\Projects\\GIFMod_API_Projects\\";
	string outputpath = "E:\\Projects\\GIFMod_API_Projects\\outputs_2\\";
#else
	string inputpath = "/home/arash/Projects/GIFMOD_API_results/";
	string outputpath = "/home/arash/Projects/GIFMOD_API_results/outputs/";
#endif //  Windows

	ModelCreator mCreate;

    mCreate.AddBody(string("surface"),string(inputpath + "topo_sligo.csv"),"","nmanning=0.00001");
    mCreate.AddBody("topsoil", inputpath + "topsoilbottom.csv", "theta=0.35, porosity=0.4, ks=0.1, theta_s=0.4, theta_r=0.05, vg_alpha=3.6, vg_n=1.56, lambda=0.5, storativity=0.001","");
    mCreate.AddBody("deepsoil", inputpath + "bedrock.csv", "theta=0.395, porosity=0.4, ks=0.003, theta_s=0.4, theta_r=0.05, vg_alpha=3.6, vg_n=1.56, lambda=0.5, storativity=0.001","");
	mCreate.getproperties("topsoil", "depth", inputpath + "topsoildepth.csv");
	mCreate.getproperties("deepsoil", "depth", inputpath + "deepsoildepth.csv");

    CMedium M(true);
    mCreate.AddLayer("surface",&M,"Catchment",331,508);
	mCreate.AddLayer("topsoil", &M, "Soil", 331, 508);
	mCreate.AddLayer("deepsoil", &M, "Soil", 331, 508);
	mCreate.ConnectBodiesVertical("infiltration",&M, "surface", "topsoil","",0,0.5);
	mCreate.ConnectBodiesVertical("percolation",&M, "topsoil", "deepsoil","",0.5,0.5);
    //cout<<"Blocks..."<< endl;

    cout<<"Downstream boundary condition"<<endl;
	// Creating the downstream boundary condition
	CMBBlock DownstreamBC("name=DSBC, hs_relationship=15.7315, area=10000000, z0=0, type=pond, s=10000000");
	DownstreamBC.set_property("Precipitation","no");
	M.AddBlock(DownstreamBC);
	CConnection DownStreamBC_connect("name=DSBC_c, width = 331, d=250, nmanning=0.00001");
	CConnection DownStreamBC_connect_ss("name=DSBC_c_soil, area = 100000, d=250, ks=0.5");
	M.AddConnector("surface(19.1)", "DSBC", DownStreamBC_connect);
	M.AddConnector("DSBC", "topsoil(19.1)", DownStreamBC_connect_ss);
	// Creating the downstream boundary condition done!

	M.Precipitation_filename.push_back(inputpath + "rain_real.txt");
    M.set_properties("tstart=40909, tend=41250, dt=0.1");
    cout << M.Block("topsoil(19.1)")->tostring()<<endl;
    //cout << M.Block("deepsoil(19.1)")->tostring()<<endl;
    cout<<"Observations"<<endl;
    /* Observation */
    measured_chrc outflow_surface;
    outflow_surface.loc_type = 1;
    outflow_surface.quan = "q";
    outflow_surface.id.push_back("DSBC_c");
    outflow_surface.name = "Outflow_Surface";
    M.measured_quan().push_back(outflow_surface);

    measured_chrc outflow_subsurface;
    outflow_subsurface.loc_type = 1;
    outflow_subsurface.quan = "q";
    outflow_subsurface.id.push_back("DSBC_c_soil");
    outflow_subsurface.name = "Outflow_Subsurface";
    M.measured_quan().push_back(outflow_subsurface);

    measured_chrc outflow_storage;
    outflow_storage.loc_type = 0;
    outflow_storage.quan = "s";
    outflow_storage.id.push_back("DSBC");
    outflow_storage.name = "Outflow_Storage";
    M.measured_quan().push_back(outflow_storage);

    measured_chrc outflow_bc_head;
    outflow_bc_head.loc_type = 0;
    outflow_bc_head.quan = "h";
    outflow_bc_head.id.push_back("DSBC");
    outflow_bc_head.name = "Outflow_BC_head";
    M.measured_quan().push_back(outflow_bc_head);

    measured_chrc outflow_head;
    outflow_head.loc_type = 0;
    outflow_head.quan = "h";
    outflow_head.id.push_back("topsoil(19.1)");
    outflow_head.name = "Outflow_Head";
    M.measured_quan().push_back(outflow_head);

    measured_chrc outflow_surface_head;
    outflow_surface_head.loc_type = 0;
    outflow_surface_head.quan = "h";
    outflow_surface_head.id.push_back("surface(19.1)");
    outflow_surface_head.name = "Outflow_Surface_Head";
    M.measured_quan().push_back(outflow_surface_head);

    measured_chrc outflow_soil_moisture;
    outflow_soil_moisture.loc_type = 0;
    outflow_soil_moisture.quan = "theta";
    outflow_soil_moisture.id.push_back("topsoil(19.1)");
    outflow_soil_moisture.name = "Outflow_Soil_Moisture";
    M.measured_quan().push_back(outflow_soil_moisture);
    /* Observation */
    cout << M.Connector("DSBC_c_soil")->tostring() << endl;
    cout<<"Solver Settings"<<endl;
    M.check_oscillation() = false;
	M.outputpathname() = outputpath;
    M.f_load_inflows();
    M.set_default_params();
    M.max_dt() = 1;
    M.write_details() = true;
    #ifdef USE_VTK
    cout << "getting surface elevation ..." << endl;
    VTK_grid gr = M.VTK_get_snap_shot("surface",&mCreate ,"z0",0,10);

	cout << "writing surf to vtp ..." << endl;
    M.write_grid_to_vtp_surf(gr, outputpath + "surf.vtp");
    M.write_grid_to_text(gr, outputpath + "surf.txt");

    cout << "writing surf to txt ..." << endl;
    M.write_grid_to_text(gr, outputpath + "surf.txt");

    cout << "Solving ..." << endl;

    #endif // USE_VTK
    //M.write_details() = false;
    M.solution_method() = "Direct Solution";
    M.solve();
    #ifdef USE_VTK
    cout << "Creating Outputs" << endl;
    for (double t=40909; t<41250; t+=1)
    {
        VTK_grid moisture_topsoil = M.VTK_get_snap_shot("topsoil", &mCreate, "theta",t,10,"theta");
        VTK_grid moisture_deepsoil = M.VTK_get_snap_shot("deepsoil", &mCreate, "theta",t,10,"theta");
        VTK_grid head_topsoil = M.VTK_get_snap_shot("topsoil", &mCreate, "h", t, 10, "h");
        VTK_grid head_deepsoil = M.VTK_get_snap_shot("deepsoil", &mCreate, "h", t, 10, "h");
		VTK_edge_grid Surface_Flow = M.VTK_get_snap_shot_edges("surface", &mCreate, "Q", t, 10, "Q");
		VTK_edge_grid Subsurface_Flow_Top = M.VTK_get_snap_shot_edges("topsoil", &mCreate, "Q", t, 10, "Q");
        VTK_edge_grid Subsurface_Flow_Deep = M.VTK_get_snap_shot_edges("deepsoil", &mCreate, "Q", t, 10, "Q");
		VTK_edge_grid Infiltration_Flow = M.VTK_get_snap_shot_edges("infiltration", &mCreate, "Q", t, 10, "Q");
		VTK_edge_grid Percolation_Flow = M.VTK_get_snap_shot_edges("percolation", &mCreate, "Q", t, 10, "Q");
        VTK_grid depth = M.VTK_get_snap_shot("surface", &mCreate, "depth",t,10,"depth");
        VTK_grid Infiltration_flow_surf = Infiltration_Flow.toVTKGtid();
        VTK_grid Percolation_flow_surf = Percolation_Flow.toVTKGtid();


        M.write_grid_to_vtp_surf(depth, outputpath + "water_depth" + numbertostring(t) + ".vtp");
		M.write_grid_to_text(depth, outputpath + "water_depth" + numbertostring(t) + ".txt");
		M.write_grid_to_vtp_surf(Surface_Flow, outputpath + "surface_flow" + numbertostring(t) + ".vtp", "Q");
		M.write_grid_to_vtp_surf(Subsurface_Flow_Top, outputpath + "subsurface_flow_top" + numbertostring(t) + ".vtp", "Q");
        M.write_grid_to_vtp_surf(Subsurface_Flow_Deep, outputpath + "subsurface_flow_deep" + numbertostring(t) + ".vtp", "Q");
		M.write_grid_to_vtp_surf(Infiltration_Flow, outputpath + "infiltration_flow" + numbertostring(t) + ".vtp", "Q");
		M.write_grid_to_vtp_surf(moisture_topsoil, outputpath + "moisture_topsoil" + numbertostring(t) + ".vtp");
		M.write_grid_to_vtp_surf(moisture_deepsoil, outputpath + "moisture_deepsoil" + numbertostring(t) + ".vtp");
		M.write_grid_to_vtp_surf(Infiltration_flow_surf, outputpath + "infiltration_surf" + numbertostring(t) + ".vtp");
		M.write_grid_to_vtp_surf(Percolation_flow_surf, outputpath + "percolation_surf" + numbertostring(t) + ".vtp");
		M.write_grid_to_text(moisture_topsoil, outputpath + "moisture_topsoil" + numbertostring(t) + ".txt");
        M.write_grid_to_vtp_surf(head_topsoil, outputpath + "head_topsoil" + numbertostring(t) + ".vtp");
        M.write_grid_to_vtp_surf(head_deepsoil, outputpath + "head_deepsoil" + numbertostring(t) + ".vtp");

    }

    #endif // USE_VTK

    M.Results.ANS.writetofile(string(outputpath + "output.txt"));
    M.Results.ANS_obs.writetofile(string(outputpath + "output_obs.txt"));

}

#include "VTK_test.h"

VTK_test::VTK_test()
{
    //ctor
}

VTK_test::~VTK_test()
{
    //dtor
}

void VTK_test::Point()
{

  vtkSmartPointer<vtkNamedColors> colors =
    vtkSmartPointer<vtkNamedColors>::New();

  // Create the geometry of a point (the coordinate)
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();

  // Create the topology of the point (a vertex)
    vtkSmartPointer<vtkCellArray> vertices =
        vtkSmartPointer<vtkCellArray>::New();

    vtkSmartPointer<vtkDoubleArray> weights =
    vtkSmartPointer<vtkDoubleArray>::New();

  weights->SetName("orig");


  for (int i=0;i<10; i++)
    for (int j=0; j<10; j++)
        for (int k=0; k<10; k++)
        {

            const float p[3] = {i, j, k};

            // We need an an array of point id's for InsertNextCell.

            vtkIdType pid[1];
            pid[0] = points->InsertNextPoint(p);
            vertices->InsertNextCell(1,pid);
            weights->InsertNextValue(i+j+k);
        }
    // Create a polydata object
    vtkSmartPointer<vtkPolyData> point =
    vtkSmartPointer<vtkPolyData>::New();

  // Set the points and vertices we created as the geometry and topology of the polydata
  point->SetPoints(points);
  point->SetVerts(vertices);
  point->GetPointData()->SetScalars(weights);
  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
  mapper->SetInput(point);
#else
  mapper->SetInputData(point);
#endif

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
  actor->GetProperty()->SetPointSize(20);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetWindowName("Point");
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(colors->GetColor3d("DarkOliveGreen").GetData());

  renderWindow->Render();
  renderWindowInteractor->Start();

  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
		vtkSmartPointer<vtkXMLPolyDataWriter>::New();
	writer->SetFileName("test.vtp");
	writer->SetInputData(mapper->GetInput());
	// This is set so we can see the data in a text editor.
	writer->SetDataModeToAscii();
	writer->Write();
  return;
}


void VTK_test::TDPoints()
{
  // Create a point cloud
  vtkSmartPointer<vtkPointSource> pointSource =
    vtkSmartPointer<vtkPointSource>::New();
  pointSource->SetCenter(0.0, 0.0, 0.0);
  pointSource->SetNumberOfPoints(50);
  pointSource->SetRadius(5.0);
  pointSource->Update();

  // Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(pointSource->GetOutputPort());

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  // Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Add the actor to the scene
  renderer->AddActor(actor);
  renderer->SetBackground(.3, .6, .3); // Background color green

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return;
}

#include "MedNUSMeshViewerCallback.h"
//Callback

MedNUSMeshViewerCallback::MedNUSMeshViewerCallback()
{
}


MedNUSMeshViewerCallback::~MedNUSMeshViewerCallback()
{
}


void MedNUSMeshViewerCallback::Execute( vtkObject *caller,
    unsigned long eventId, void *callData)
{
    vtkRenderWindowInteractor *interactor = viewer->getInteractor();
    int *pos = interactor->GetEventPosition();
    vtkRenderer *renderer = viewer->getRenderer();

    // Pick at the mouse location provided by the interactor
    vtkPropPicker *picker = vtkPropPicker::New();
    int found = picker->PickProp(pos[0], pos[1], renderer);

    if (found)
    {
        vtkProp *prop = picker->GetViewProp();
        viewer->highlight(vtkActor::SafeDownCast(prop));

        // hack
        picker->Pick(pos[0], pos[1], 0, viewer->getRenderer());
        double* worldpos = picker->GetPickPosition();
        AddAnnotation(worldpos[0], worldpos[1], worldpos[2]);
    }
    else
        viewer->highlight(NULL);
}

void MedNUSMeshViewerCallback::AddAnnotation(double x, double y, double z)
{
    MedNUSMeshAnnotation toAdd;

    toAdd.x = x;
    toAdd.y = y;
    toAdd.z = z;

    toAdd.source = vtkSphereSource::New();
    toAdd.source->SetCenter(x, y, z);
    toAdd.source->SetRadius(10);
    toAdd.source->Update();

    toAdd.normals = vtkPolyDataNormals::New();
    toAdd.normals->SetInputConnection(toAdd.source->GetOutputPort());

    toAdd.mapper = vtkPolyDataMapper::New();
    toAdd.mapper->SetInputConnection(toAdd.normals->GetOutputPort());
    toAdd.mapper->Update();

    toAdd.actor = vtkActor::New();
    toAdd.actor->SetMapper(toAdd.mapper);
    toAdd.actor->SetPosition(x, y, z);
    toAdd.actor->GetProperty()->SetColor(255, 0, 0);
    toAdd.actor->SetVisibility(true);

    viewer->getRenderer()->AddActor(toAdd.actor);

    CreateAnnotationText("point", x, y, z);
}

void MedNUSMeshViewerCallback::CreateAnnotationText(QString text, double x, double y, double z)
{
    vtkSmartPointer<vtkVectorText> annotationText = vtkSmartPointer<vtkVectorText>::New();
    annotationText->SetText( text.toStdString().c_str() );

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(annotationText->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->SetScale(3,3,3);
    actor->SetPosition(x,y,z);
    actor->GetProperty()->SetColor(255, 0, 0);

    viewer->getRenderer()->AddActor(actor);
}

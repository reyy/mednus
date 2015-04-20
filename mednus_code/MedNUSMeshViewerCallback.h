#ifndef MEDNUSMESHVIEWERCALLBACK_H
#define MEDNUSMESHVIEWERCALLBACK_H

#include <vtkCommand.h>
#include <vtkObjectBase.h>
#include <QDebug>
#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkVectorText.h>
#include <vtkSmartPointer.h>

#include "MedNUSMeshViewer.h"

class MedNUSMeshAnnotation
{
public:
    double x, y, z;
    vtkSphereSource* source;
    vtkPolyDataNormals* normals;
    vtkPolyDataMapper* mapper;
    vtkActor* actor;
};

class MedNUSMeshViewerCallback: public vtkCommand
{
public:
    MedNUSMeshViewer *viewer;

    static MedNUSMeshViewerCallback *New() { return new MedNUSMeshViewerCallback(); }
    void Execute(vtkObject *caller, unsigned long eventId, void *callData);
    void PrintSelf(ostream& os, vtkIndent indent) {}
    void PrintHeader(ostream& os, vtkIndent indent) {}
    void PrintTrailer(ostream& os, vtkIndent indent) {}

    void AddAnnotation(double x, double y, double z);

protected:
    MedNUSMeshViewerCallback();
    ~MedNUSMeshViewerCallback();

    void CreateAnnotationText(QString text, double x, double y, double z);
};


#endif // MEDNUSMESHVIEWERCALLBACK_H

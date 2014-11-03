#ifndef MedNUSMeshViewerCALLBACK_H
#define MedNUSMeshViewerCALLBACK_H
#include <vtkCommand.h>
#include <vtkObjectBase.h>
#include "MedNUSMeshViewer.h"

class MedNUSMeshViewerCallback: public vtkCommand
{
public:
    static MedNUSMeshViewerCallback *New() { return new MedNUSMeshViewerCallback(); }
    void Execute(vtkObject *caller, unsigned long eventId, void *callData);
    MedNUSMeshViewer *viewer;
    void PrintSelf(ostream& os, vtkIndent indent){};
    void PrintHeader(ostream& os, vtkIndent indent){};
    void PrintTrailer(ostream& os, vtkIndent indent){};

protected:
    MedNUSMeshViewerCallback();
    ~MedNUSMeshViewerCallback();
};
#endif // MedNUSMeshViewerCALLBACK_H

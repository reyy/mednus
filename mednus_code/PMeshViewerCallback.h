#ifndef PMESHVIEWERCALLBACK_H
#define PMESHVIEWERCALLBACK_H
#include <vtkCommand.h>
#include <vtkObjectBase.h>
#include "PMeshViewer.h"

class PMeshViewerCallback: public vtkCommand
{
public:
    static PMeshViewerCallback *New() { return new PMeshViewerCallback(); }
    void Execute(vtkObject *caller, unsigned long eventId, void *callData);
    PMeshViewer *viewer;
    void PrintSelf(ostream& os, vtkIndent indent){};
    void PrintHeader(ostream& os, vtkIndent indent){};
    void PrintTrailer(ostream& os, vtkIndent indent){};

protected:
    PMeshViewerCallback();
    ~PMeshViewerCallback();
};
#endif // PMESHVIEWERCALLBACK_H

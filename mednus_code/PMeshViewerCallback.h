#ifndef PMESHVIEWERCALLBACK_H
#define PMESHVIEWERCALLBACK_H
#include <vtkCommand.h>
#include "PMeshViewer.h"

class PMeshViewerCallback: public vtkCommand
{
public:
    static PMeshViewerCallback *New() { return new PMeshViewerCallback(); }
    //void Execute(vtkObject *caller, unsigned long eventId, void *callData);
    void Execute( vtkObject *caller,
        unsigned long eventId, void *callData);
    PMeshViewer *viewer;

protected:
    PMeshViewerCallback();
    ~PMeshViewerCallback();
};
#endif // PMESHVIEWERCALLBACK_H

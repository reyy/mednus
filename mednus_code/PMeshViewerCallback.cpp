#include "PMeshViewerCallback.h"
//Callback

PMeshViewerCallback::PMeshViewerCallback()
{
}


PMeshViewerCallback::~PMeshViewerCallback()
{
}


void PMeshViewerCallback::Execute( vtkObject *caller,
    unsigned long eventId, void *callData)
{
//    vtkRenderWindowInteractor *interactor = viewer->getInteractor();
//    int *pos = interactor->GetEventPosition();
//    vtkRenderer *renderer = viewer->getRenderer();

//    // Pick at the mouse location provided by the interactor
//    vtkPropPicker *picker = vtkPropPicker::New();
//    int found = picker->PickProp(pos[0], pos[1], renderer);

//    if (found)
//    {
//        vtkProp *prop = picker->GetViewProp();
//        viewer->highlight(vtkActor::SafeDownCast(prop));
//    }
//    else
//        viewer->highlight(NULL);
}


#include <QApplication>
#include <vtkAutoInit.h>
#include "PMeshViewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    VTK_MODULE_INIT(vtkRenderingOpenGL);
    VTK_MODULE_INIT(vtkRenderingFreeTypeOpenGL);
    VTK_MODULE_INIT(vtkInteractionStyle);
    VTK_MODULE_INIT(vtkRenderingVolumeOpenGL);

    PMeshViewer viewer(false);
    viewer.show();
    return app.exec();
}


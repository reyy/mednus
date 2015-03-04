#include <QApplication>
#include <vtkAutoInit.h>

#include "MedNUSMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    VTK_MODULE_INIT(vtkRenderingOpenGL);
    VTK_MODULE_INIT(vtkRenderingFreeTypeOpenGL);
    VTK_MODULE_INIT(vtkInteractionStyle);
    VTK_MODULE_INIT(vtkRenderingVolumeOpenGL);

    MedNUSMainWindow win;
    win.show();

    return app.exec();
}

/* MedNUSMeshViewer.cpp

   3D mesh viewer.
   
   Copyright 2012, 2103, 2014 National University of Singapore
   Author: Leow Wee Kheng

   Note: This file was originally written by Prof Leow Wee Kheng,
         as such parts of the code have been commented out to temporarily
         disable these features.

         Also, the MedNUS team is currently in the midst of converting it
         to use the MedNUS naming conventions.
*/


#include "MedNUSMeshViewer.h"
#include <QtGui>
#include "vtkCommand.h"
#include "vtkOBJReader.h"
#include "vtkPLYReader.h"
#include "vtkSTLReader.h"
#include "vtkPLYWriter.h"
#include "vtkSTLWriter.h"
#include "vtkWindowToImageFilter.h"
#include "vtkJPEGWriter.h"
#include "vtkPNGWriter.h"
#include "vtkTIFFWriter.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkCellArray.h"
#include "MedNUSMeshViewerCallback.h"

#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include <vtkOutlineFilter.h>
#include <vtkOutlineSource.h>

#include <vtkTextActor3D.h>
#include <vtkTextProperty.h>

#include <vtkCaptionActor2D.h>

#include <unistd.h>
using namespace std;

// MedNUSMeshViewer class

MedNUSMeshViewer::MedNUSMeshViewer(QString dir, bool withMeshPanel)
{
    this->setAccessibleName(dir);
    // Initialisation
    appName = QString("Mesh Viewer");
    projectName = QString("Untitled");
    readDir = QString(".");
    writeDir = QString(".");
    loaded = false;
    hideFrontFace = 0;

    // Create GUI
    createWidgets(withMeshPanel);
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    QMessageBox msgBox;
    msgBox.setText("Depending on the size of the model. It may take awhile to load.");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {
        try {
            // Start the timer.
            QElapsedTimer et;
            et.start();

            // Load the meshes.
            loadMeshDir(dir);

            // Check the time taken to load the meshes.
            qint64 timeTaken = et.elapsed();
            /*qDebug() << "Time Taken for mesh: " << timeTaken << " ms.";
            qDebug() << "# of vertices: " << meshModel.getData(0)->GetNumberOfVerts();
            qDebug() << "# of polygons: " << meshModel.getData(0)->GetNumberOfPolys();
            qDebug() << "# of vertices: " << meshModel.getData(1)->GetNumberOfVerts();
            qDebug() << "# of polygons: " << meshModel.getData(1)->GetNumberOfPolys();*/
        } catch (int a) {
            loadMesh();
        }
    } else {
        return;
    }

    //drawSphere(0.5,-1,-1,-1);
    //drawSphere(0.8,2,2,2);
    //drawBoundingBox();

    //Loaded sucessfully!
    this->setProperty("Loaded",true);

    //DEBUG
//       QTimer *timer = new QTimer(this);
//          connect(timer, &QTimer::timeout, [=](){
//              double a[] = {5,5,5};
//              double b[] = {50,50,50};
//              double c[]={0,0,0};

//             renderer->GetActiveCamera()->GetPosition(a);
//             renderer->GetActiveCamera()->GetFocalPoint(b);
//             qDebug() << renderer->GetActiveCamera()->GetViewAngle();
//             renderer->GetActiveCamera()->GetViewUp(c);
//             qDebug() << a[0] << a[1] << a[2];
//             qDebug() << b[0] << b[1] << b[2];
//             qDebug() << c[0] << c[1] << c[2];

//          });

//          timer->start(1000);

}


MedNUSMeshViewer::~MedNUSMeshViewer()
{
    uninstallPipeline();
    style->Delete();
}

void MedNUSMeshViewer::forceAddMesh(const QString &fileName)
{
    try {
        QStringList list;
        list.append(fileName);
        addMesh(list);
    } catch (int a) {}
}

void MedNUSMeshViewer::forceLoadMesh(const QString &fileName)
{
    try {
        QStringList list;
        list.append(fileName);
        loadMesh(list);
    } catch (int a) {}
}

void MedNUSMeshViewer::setCameraView(int cameraViewAngle, double cameraPosition[], double cameraFocalPoint[], double cameraViewUp[])
{
    vtkCameraInterpolator *interpolator = vtkCameraInterpolator::New();
    interpolator->SetInterpolationType(vtkCameraInterpolator::INTERPOLATION_TYPE_SPLINE);

    vtkCamera *cam = renderer->GetActiveCamera();
    vtkCamera *moveTo = vtkCamera::New();
    moveTo->DeepCopy(cam);
    moveTo->SetViewAngle(cameraViewAngle);
    moveTo->SetPosition(cameraPosition);
    moveTo->SetFocalPoint(cameraFocalPoint);
    moveTo->SetViewUp(cameraViewUp);

    interpolator->Initialize();
    interpolator->AddCamera(0,cam);
    interpolator->AddCamera(200,moveTo);

    double delT = static_cast<double>(200) / 48;

    double t=0.0;
    for (int i=0; i < 48; i++, t+=delT)
    {
        interpolator->InterpolateCamera(t,cam);
        renderWindow->Render();
    }
    interpolator->Delete();
    moveTo->Delete();

    renderWindow->Render();
}

int MedNUSMeshViewer::getViewAngle() {return renderer->GetActiveCamera()->GetViewAngle();}

//VERY BAD STUFF //MUST CHANGE!
double *MedNUSMeshViewer::getCamerPosition() {double a[3]; renderer->GetActiveCamera()->GetPosition(a);return a;}

double *MedNUSMeshViewer::getCameraFocalPoint() {double a[3]; renderer->GetActiveCamera()->GetFocalPoint(a); return a;}

double *MedNUSMeshViewer::getCameraViewUp() {double a[3]; renderer->GetActiveCamera()->GetViewUp(a); return a;}

// Event handling

void MedNUSMeshViewer::closeEvent(QCloseEvent *event)
{
    if (meshTable)
        meshTable->close();
    event->accept();
}


// Create window widgets

#define NumRow 0
#define NumColumn 7
#define VisibleColumn 0
#define ColorColumn 1
#define Null1Column 2
#define TransparencyColumn 3
#define Null2Column 4
#define NameColumn 5
#define SourceColumn 6

void MedNUSMeshViewer::createWidgets(bool withMeshPanel)
{


    // Create vtk objects
    vtkWidget = new QVTKWidget(this);

    // QVTKWidget has already created a render window and interactor
    renderWindow = vtkWidget->GetRenderWindow();
    renderer = vtkRenderer::New();
    renderer->SetBackground(0.0, 0.0, 0.0);
    renderWindow->AddRenderer(renderer);
    
    interactor = renderWindow->GetInteractor();
    style = vtkInteractorStyleTrackballCamera::New();
    interactor->SetInteractorStyle(style);

    if (withMeshPanel)
    {
        MedNUSMeshViewerCallback *callback = MedNUSMeshViewerCallback::New();
        callback->viewer = this;
        interactor->AddObserver(vtkCommand::RightButtonPressEvent, callback);
        callback->Delete();
    }
    
    // Central widget
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(vtkWidget, 0, 0);
//    QWidget *main = new QWidget;
//    main->setLayout(grid);
//    main->setStyleSheet("background-color: #285183;");
    grid->setContentsMargins(0,0,0,0);
    this->setLayout(grid);
    //setStyleSheet("background-color: #1a394a;");
    //setCentralWidget(main);
    
    // Create mesh mode box
//    meshModeBox = new QComboBox(this);
//    QStringList choices;
//    choices << "smooth surface" << "flat surface" << "flat lines"
//        << "wire frame" << "points";
//    meshModeBox->insertItems(0, choices);
//    connect(meshModeBox, SIGNAL(activated(int)), this,
//        SLOT(setMeshMode(int)));
    
    if (withMeshPanel)
    {
        // Create mesh table
        meshTable = new PMeshTableWidget;
        connect(meshTable, SIGNAL(visibilityChanged(int, bool)),
            this, SLOT(setVisibility(int, bool)));
        connect(meshTable, SIGNAL(colorChanged(int, QColor)),
            this, SLOT(setColor(int, QColor)));
        connect(meshTable, SIGNAL(transparencyChanged(int, float)),
            this, SLOT(setTransparency(int, float)));
        connect(meshTable, SIGNAL(itemSelected(int)),
            this, SLOT(blink(int)));
        connect(meshTable, SIGNAL(itemDeleted(int)),
            this, SLOT(deleteMesh(int)));
        meshTable->setStyleSheet("background-color: #285183;");



        // Create dock widget for bottom dock area.
        meshPanel = new QDockWidget("Mesh List");
        meshPanel->setWidget(meshTable);
        meshPanel->setAllowedAreas(Qt::BottomDockWidgetArea);
        meshPanel->setStyleSheet("background-color: #285183;");
//        QMainWindow *mw = new QMainWindow( this );
//        this->layout()->addWidget(mw);
//        mw->addDockWidget(Qt::BottomDockWidgetArea, meshPanel);
        this->layout()->addWidget(meshPanel);
    }
    else
    {
        meshTable = NULL;
        meshPanel = NULL;
    }

    vtkWidget->updateGeometry();
}


void MedNUSMeshViewer::createActions()
{
}


void MedNUSMeshViewer::createMenus()
{
}


void MedNUSMeshViewer::createToolBars()
{
}


void MedNUSMeshViewer::createStatusBar()
{
}


// Slot methods

void MedNUSMeshViewer::newProject()
{
    uninstallPipeline();
    renderer = vtkRenderer::New();
    //renderer->SetBackground(26/255.0, 57/255.0, 74/255.0);
    renderer->SetBackground(0/255.0, 0/255.0, 0/255.0);
    renderWindow->AddRenderer(renderer);
    renderWindow->Render();
}


void MedNUSMeshViewer::openProject()
{
}


void MedNUSMeshViewer::saveProject()
{
}


void MedNUSMeshViewer::saveProjectAs()
{
}


void MedNUSMeshViewer::loadDir()
{
    QString dirName = QFileDialog::getExistingDirectory(this,
        QObject::tr("Load all mesh files in a directory"), readDir);

    if (!dirName.isEmpty())
    {
        QDir dir(dirName);
        QStringList filters;
        filters << "*.obj" << "*.ply" << "*.stl";
        QStringList list = dir.entryList(filters);
        
        if (!list.isEmpty())
        {
            QStringList fileNames;
            for (int i = 0; i < list.size(); ++i)
                fileNames.append(dirName + "/" + list[i]);
                
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if (loadMesh(fileNames))
                readDir = dirName;
            QApplication::restoreOverrideCursor();
        }
    }
}


void MedNUSMeshViewer::loadMesh()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
        QObject::tr("Load one or more mesh files"), readDir,
        QObject::tr("3D mesh files: *.obj, *.ply, *.stl (*.obj *.ply *.stl)"));
        
    if (!fileNames.isEmpty())
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if (loadMesh(fileNames))
            readDir = QFileInfo(fileNames[fileNames.size()-1]).path();
        QApplication::restoreOverrideCursor();
    }
}


void MedNUSMeshViewer::loadMeshDir(const QString &dirName) {

    if (!dirName.isEmpty())
    {
        QDir dir(dirName);
        QStringList filters;
        filters << "*.obj" << "*.ply" << "*.stl";
        QStringList list = dir.entryList(filters);

        if (!list.isEmpty())
        {
            QStringList fileNames;
            for (int i = 0; i < list.size(); ++i)
                fileNames.append(dirName + "/" + list[i]);

            QApplication::setOverrideCursor(Qt::WaitCursor);
            loadMesh(fileNames);
            QApplication::restoreOverrideCursor();
        }
    }
}


void MedNUSMeshViewer::addMesh()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
        QObject::tr("Add one or more mesh files"), readDir,
        QObject::tr("3D mesh files: *.obj, *.ply, *.stl (*.obj *.ply *.stl)"));
        
    if (!fileNames.isEmpty())
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if (addMesh(fileNames))
            readDir = QFileInfo(fileNames[fileNames.size()-1]).path();
        QApplication::restoreOverrideCursor();
    }
}


void MedNUSMeshViewer::saveDirPly()
{
}


void MedNUSMeshViewer::saveDirStl()
{
}


void MedNUSMeshViewer::saveView()
{
    if (!loaded)  // Nothing to save.
        return;
        
    QString fileName = QFileDialog::getSaveFileName(this,
        QObject::tr("Save current view to a file"), writeDir,
        QObject::tr("Image files: *.jpg, *.png, *.tif (*.jpg *.png *.tif)"));
        
    if (fileName.isEmpty())
        return;

    if (saveImage(fileName))
        writeDir = QFileInfo(fileName).path();
}


void MedNUSMeshViewer::toggleFrontFace()
{
    if (meshList.isEmpty())
        return;
        
    hideFrontFace = !hideFrontFace;
    
    for (int i = 0; i < meshList.size(); ++i)
        meshList[i].actor->GetProperty()->
            SetFrontfaceCulling(hideFrontFace);
    renderWindow->Render();
}


void MedNUSMeshViewer::setMeshMode(int mode)
{
    int rep, interpol, edgeOn;
    
    switch (mode)
    {
        case 0: // smooth surface
            rep = VTK_SURFACE;
            interpol = VTK_GOURAUD;
            edgeOn = 0;
            break;
            
        case 1: // flat surface
            rep = VTK_SURFACE;
            interpol = VTK_FLAT;
            edgeOn = 0;
            break;
            
        case 2: // surface + lines
            rep = VTK_SURFACE;
            interpol = VTK_FLAT;
            edgeOn = 1;
            break;
            
        case 3: // wire frame
            rep = VTK_WIREFRAME;
            interpol = VTK_FLAT;
            edgeOn = 1;
            break;
            
        case 4: // mesh point
            rep = VTK_POINTS;
            interpol = VTK_FLAT;
            edgeOn = 0;
            break;
            
        default: // smooth surface
            rep = VTK_SURFACE;
            interpol = VTK_GOURAUD;
            edgeOn = 0;
            break;
    }
    
    for (int i = 0; i < meshList.size(); ++i)
    {
        vtkProperty *property = meshList[i].actor->GetProperty();
        property->SetRepresentation(rep);
        property->SetInterpolation(interpol);
        property->SetEdgeVisibility(edgeOn);
    }
    
    renderWindow->Render();
}


void MedNUSMeshViewer::setSmoothSurface()
{
    meshModeBox->setCurrentIndex(0);
    setMeshMode(0);
}


void MedNUSMeshViewer::setFlatSurface()
{
    meshModeBox->setCurrentIndex(1);
    setMeshMode(1);
}


void MedNUSMeshViewer::setFlatLines()
{
    meshModeBox->setCurrentIndex(2);
    setMeshMode(2);
}


void MedNUSMeshViewer::setWireFrame()
{
    meshModeBox->setCurrentIndex(3);
    setMeshMode(3);
}


void MedNUSMeshViewer::setMeshPoint()
{
    meshModeBox->setCurrentIndex(4);
    setMeshMode(4);
}


void MedNUSMeshViewer::info()
{
}


void MedNUSMeshViewer::help()
{
}


void MedNUSMeshViewer::about()
{
}


void MedNUSMeshViewer::setAppName(const QString &name)
{
    appName = name;
}


void MedNUSMeshViewer::setVisibility(int row, bool visible)
{
   if (row < 0 || row >= meshList.size())
       return;
       
    meshList[row].actor->SetVisibility(visible ? 1 : 0);
    renderWindow->Render();
}


void MedNUSMeshViewer::setColor(int row, QColor color)
{
   if (row < 0 || row >= meshList.size())
       return;
    
    double red = color.red() / 255.0;
    double green = color.green() / 255.0;
    double blue = color.blue() / 255.0;
    meshList[row].actor->GetProperty()->SetColor(red, green, blue);
    renderWindow->Render();
}


void MedNUSMeshViewer::setTransparency(int row, float value)
{
   if (row < 0 || row >= meshList.size())
       return;
    
    meshList[row].actor->GetProperty()->SetOpacity(1.0 - value);
    renderWindow->Render();
}


void MedNUSMeshViewer::blink(int row)
{
   if (row < 0 || row >= meshList.size())
       return;
        
    double red, green, blue, r, g, b;
    vtkProperty *property = meshList[row].actor->GetProperty();
    property->GetColor(red, green, blue);
    r = g = b = 0.5;
    
    for (int i = 0; i < 2; ++i)  // Blink 2 times
    {
        property->SetColor(r, g, b);
        renderWindow->Render();
        usleep(250000);
        property->SetColor(red, green, blue);
        renderWindow->Render();
        usleep(250000);
    }
}


void MedNUSMeshViewer::deleteMesh(int row)
{
   /*if (row < 0 || row >= meshList.size())
       return;
       
    renderer->RemoveActor(meshList[row]);
    meshList.removeAt(row);
    renderWindow->Render();*/
    QList<QTableWidgetSelectionRange> list = meshTable->selectedRanges();
    if (list.isEmpty())
        return;

    QTableWidgetSelectionRange range = list[0];  // Only 1 range.
    for (int i = range.bottomRow(); i >= range.topRow(); --i)
    {
        meshTable->removeRow(i);
        renderer->RemoveActor(meshList[i].actor);
        meshList[i].normals->Delete();
        meshList[i].mapper->Delete();
        meshList[i].actor->Delete();
        meshList.removeAt(i);
    }

    renderWindow->Render();
}


// Supporting methods

void MedNUSMeshViewer::installPipeline(int startIndex)
{
    if (!renderer)
    {
        renderer = vtkRenderer::New();  // Camera is also reset.
        renderer->SetBackground(26/255.0, 57/255.0, 74/255.0);
        renderWindow->AddRenderer(renderer);
    }
    
    for (int i = startIndex; i < meshList.size(); ++i) {
        //renderer->AddActor(meshModel.getActor(i));
        //meshModel.getActor(i)->SetScale(i*0.1 + 1);
        renderer->AddActor(meshList[i].actor);
    }

    hideFrontFace = 0;
    //frontFaceAction->setChecked(false);
    //meshModeBox->setCurrentIndex(0);
    //smoothSurfaceAction->setChecked(true);
    renderer->ResetCamera();
    loaded = true;
    //vtkWidget->SetRenderWindow(renderWindow);
}



void MedNUSMeshViewer::drawSphere(double radius, double xpos, double ypos, double zpos)
{
    vtkSmartPointer<vtkSphereSource> sphereSource =
        vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(radius);
    sphereSource->Update();
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> sphereActor =
        vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);
    sphereActor->SetPosition(xpos,ypos,zpos);
    renderer->AddActor(sphereActor);
}

void MedNUSMeshViewer::drawBoundingBox()
{
    vector<vtkActor*> temp;
    vtkActorCollection *act= renderer->GetActors();
    act->InitTraversal();
    for (int i=0;i<act->GetNumberOfItems();++i) {
        vtkActor* acty = act->GetNextActor();
        vtkOutlineSource *bb = vtkOutlineSource::New();

        // set the bb
        bb->SetBounds(acty->GetBounds());

        // bb -> mapper
        vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
        mapper->SetInputConnection(bb->GetOutputPort());

        // mapper -> actor
        vtkActor *actor = vtkActor::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetColor(255,255,0);
        renderer->AddActor(actor);
    }
    for(int i=0; i<temp.size(); i++)
        renderer->AddActor(temp[i]);
}

void MedNUSMeshViewer::uninstallPipeline()
{   
    /*if (renderer)
    {
        renderer->RemoveAllViewProps();
        renderWindow->RemoveRenderer(renderer);
        renderer->Delete();
        renderer = NULL;
    }
    
    meshModel.clear();
    if (meshTable)
    {
        int size = meshTable->rowCount();
        for (int i = size - 1; i >= 0; --i)
            meshTable->removeRow(i);  // Remove rows from bottom up.
    }
    projectName = QString("Untitled");
    loaded = false;*/

    if (renderer) {
        renderer->RemoveAllViewProps();
        renderWindow->RemoveRenderer(renderer);
        renderer->Delete();
        renderer = NULL;
    }

    for (int i = 0; i < meshList.size(); i++) {
        meshList[i].normals->Delete();
        meshList[i].mapper->Delete();
        meshList[i].actor->Delete();
        meshTable->removeRow(0);
    }
    meshList.clear();
    projectName = QString("Untitled");
    loaded = false;
}


#define RowHeight 20
#define MaxHeight 600

bool MedNUSMeshViewer::loadMesh(const QStringList &fileNames)
{
    uninstallPipeline();
    return addMesh(fileNames);
}


bool MedNUSMeshViewer::addMesh(const QStringList &fileNames)
{
    int startIndex = meshList.size();
    
    for (int i = 0; i < fileNames.size(); ++i)
    {
        QString suffix = QFileInfo(fileNames[i]).suffix();
       
        vtkPolyDataAlgorithm *reader = NULL;

        if (suffix == "obj")
        {
            vtkOBJReader *rd = vtkOBJReader::New();
            rd->SetFileName(fileNames[i].toLatin1().data());
            reader = rd;
        }
        else if (suffix == "ply")
        {
            vtkPLYReader *rd = vtkPLYReader::New();
            rd->SetFileName(fileNames[i].toLatin1().data());
            reader = rd;
        }
        else if (suffix == "stl")
        {
            vtkSTLReader *rd = vtkSTLReader::New();
            rd->SetFileName(fileNames[i].toLatin1().data());
            reader = rd;
        }
        else // This should not happen, but just in case.
        {
            uninstallPipeline();
            cout << "Error: In MedNUSMeshViewer::loadMesh(): file type " <<
                suffix.toLatin1().data() << " is unsupported.\n" << flush;
            return false;
        }
        
        // Try to read data
        /*reader->Update();
        unsigned long err = reader->GetErrorCode();
        
        if (err != 0)
        {
            QMessageBox::critical(this, appName,
                QString("Error reading mesh file %1.").arg(fileNames[i]));
            reader->Delete();
            return false;
        }
        
        // Append to mesh model
        QString name = QFileInfo(fileNames[i]).baseName();
        QString source = fileNames[i];
        vtkPolyData *data = reader->GetOutput();
        meshModel.append(name, source, data);
        reader->Delete();

        if (meshTable)
            meshTable->append(name, source);*/

        // Append to mesh list
        PMeshPart part;
        part.name = QFileInfo(fileNames[i]).baseName();
        part.source = fileNames[i];
        part.data = reader->GetOutput();
        reader->Update();

        part.normals = vtkPolyDataNormals::New();
        part.normals->SetInputData(part.data);
        part.mapper = vtkPolyDataMapper::New();
        part.mapper->SetInputConnection(part.normals->GetOutputPort());
        part.mapper->Update();
        part.actor = vtkActor::New();
        part.actor->SetMapper(part.mapper);
        meshList.append(part);
        reader->Delete();

        // Append to mesh table
        int j = meshTable->rowCount();
        meshTable->insertRow(j);
        meshTable->setRowHeight(j, RowHeight);

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        meshTable->setItem(j, 0, item);

        item = new QTableWidgetItem;
        item->setFlags(Qt::ItemIsEnabled);
        item->setBackground(QBrush(QColor(255, 255, 255)));
        meshTable->setItem(j, 1, item);

        item = new QTableWidgetItem(part.name);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
            Qt::ItemIsEditable);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        meshTable->setItem(j, 2, item);

        item = new QTableWidgetItem(part.source);
        item->setFlags(Qt::ItemIsEnabled);
        item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        meshTable->setItem(j, 3, item);
    }
       
    installPipeline(startIndex);
    setWindowTitle(QString("%1 - %2").arg(appName).arg(projectName));
    return true;
}


bool MedNUSMeshViewer::saveImage(const QString &fileName)
{
    vtkWindowToImageFilter *filter = vtkWindowToImageFilter::New();
    filter->SetInput(vtkWidget->GetRenderWindow());

    QString suffix = QFileInfo(fileName).suffix();
    vtkImageWriter *writer;

    if (suffix == "jpg")
        writer = vtkJPEGWriter::New();
    else if (suffix == "png")
        writer = vtkPNGWriter::New();
    else if (suffix == "tif")
        writer = vtkTIFFWriter::New();
    else
    {
        QMessageBox::critical(this, appName,
            QString("File type %1 is unsupported.").arg(suffix));
        return false;
    }

    writer->SetInputData(filter->GetOutput());
    writer->SetFileName(fileName.toLatin1().data());
    writer->Write();
    writer->Delete();
    filter->Delete();
    return true;
}

// For callback

vtkRenderWindowInteractor *MedNUSMeshViewer::getInteractor()
{
    return interactor;
}


vtkRenderer *MedNUSMeshViewer::getRenderer()
{
    return renderer;
}


void MedNUSMeshViewer::highlight(vtkActor *actor)
{
    if (!actor)
        meshTable->setCurrentCell(-1, -1);
    else
    {
        for (int i = 0; i < meshList.size(); ++i)
            if (meshList[i].actor == actor)
                meshTable->setCurrentCell(i, NameColumn);
    }
}




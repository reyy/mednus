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
    withMeshPanel = false;
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
            qDebug() << "Time Taken for mesh: " << timeTaken << " ms.";
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

    //return;
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
    vtkCamera *cam = renderer->GetActiveCamera();
    cam->SetViewAngle(cameraViewAngle);
    cam->SetPosition(cameraPosition);
    cam->SetFocalPoint(cameraFocalPoint);
    cam->SetViewUp(cameraViewUp);
    renderWindow->Render();
}

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
//    newProjectAction = new QAction(QObject::tr("&New Project"), this);
//    newProjectAction->setIcon(QIcon(":/images/new.png"));
//    newProjectAction->setShortcut(QObject::tr("Ctrl+N"));
//    newProjectAction->setStatusTip(QObject::tr("Create a new project"));
//    connect(newProjectAction, SIGNAL(triggered()),
//        this, SLOT(newProject()));
    
//    openProjectAction = new QAction(QObject::tr("&Open Project"), this);
//    openProjectAction->setIcon(QIcon(":/images/open.png"));
//    openProjectAction->setShortcut(QObject::tr("Ctrl+O"));
//    openProjectAction->setStatusTip(QObject::tr("Open a project file"));
//    connect(openProjectAction, SIGNAL(triggered()),
//        this, SLOT(openProject()));

//    saveProjectAction = new QAction(QObject::tr("&Save Project"), this);
//    saveProjectAction->setIcon(QIcon(":/images/save.png"));
//    saveProjectAction->setShortcut(QObject::tr("Ctrl+S"));
//    saveProjectAction->setStatusTip(QObject::tr("Save project to file"));
//    connect(saveProjectAction, SIGNAL(triggered()),
//        this, SLOT(saveProject()));

//    saveProjectAsAction = new QAction(QObject::tr("Save Project &As"), this);
//    saveProjectAsAction->setIcon(QIcon(":/images/saveas.png"));
//    saveProjectAsAction->setShortcut(QObject::tr("Ctrl+A"));
//    saveProjectAsAction->setStatusTip(QObject::tr("Save project to another file"));
//    connect(saveProjectAsAction, SIGNAL(triggered()),
//        this, SLOT(saveProjectAs()));
    
//    loadDirAction = new QAction(QObject::tr("Load Mesh by &Directory"), this);
//    loadDirAction->setIcon(QIcon(":/images/loaddir.png"));
//    loadDirAction->setShortcut(QObject::tr("Ctrl+D"));
//    loadDirAction->setStatusTip(QObject::tr("Load mesh files in a directory"));
//    connect(loadDirAction, SIGNAL(triggered()), this, SLOT(loadDir()));
    
//    loadMeshAction = new QAction(QObject::tr("Load Mesh by &File"), this);
//    loadMeshAction->setIcon(QIcon(":/images/loadmesh.png"));
//    loadMeshAction->setShortcut(QObject::tr("Ctrl+F"));
//    loadMeshAction->setStatusTip(QObject::tr("Load meshes by file names"));
//    connect(loadMeshAction, SIGNAL(triggered()), this, SLOT(loadMesh()));
    
//    addMeshAction = new QAction(QObject::tr("Add Mesh by File"), this);
//    addMeshAction->setIcon(QIcon(":/images/addmesh.png"));
//    addMeshAction->setStatusTip(QObject::tr("Add meshes by file names"));
//    connect(addMeshAction, SIGNAL(triggered()), this, SLOT(addMesh()));
        
//    saveDirPlyAction = new QAction(QObject::tr("in PLY format"), this);
//    connect(saveDirPlyAction, SIGNAL(triggered()), this, SLOT(saveDirPly()));

//    saveDirStlAction = new QAction(QObject::tr("in STL format"), this);
//    connect(saveDirStlAction, SIGNAL(triggered()), this, SLOT(saveDirStl()));
    
//    exitAction = new QAction(QObject::tr("E&xit"), this);
//    exitAction->setShortcut(QObject::tr("Ctrl+Q"));
//    exitAction->setStatusTip(QObject::tr("Exit Mesh Viewer"));
//    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    
//    saveViewAction = new QAction(QObject::tr("Save &View"), this);
//    saveViewAction->setIcon(QIcon(":/images/saveview.png"));
//    saveViewAction->setShortcut(QObject::tr("Shift+V"));
//    saveViewAction->setStatusTip(QObject::tr("Save current view to a file"));
//    connect(saveViewAction, SIGNAL(triggered()), this, SLOT(saveView()));
    
//    frontFaceAction = new QAction(QObject::tr("&Hide Front Faces"), this);
//    frontFaceAction->setIcon(QIcon(":/images/hide.png"));
//    frontFaceAction->setShortcut(QObject::tr("Shift+H"));
//    frontFaceAction->setStatusTip(QObject::tr("Hide/show front faces"));
//    frontFaceAction->setCheckable(true);
//    connect(frontFaceAction, SIGNAL(triggered()), this,
//        SLOT(toggleFrontFace()));
        
//    smoothSurfaceAction = new QAction(QObject::tr("&Smooth Surface"), this);
//    smoothSurfaceAction->setShortcut(QObject::tr("Shift+S"));
//    smoothSurfaceAction->setStatusTip(QObject::tr("Smooth surface"));
//    smoothSurfaceAction->setCheckable(true);
//    connect(smoothSurfaceAction, SIGNAL(triggered()), this,
//        SLOT(setSmoothSurface()));
        
//    flatSurfaceAction = new QAction(QObject::tr("&Flat Surface"), this);
//    flatSurfaceAction->setShortcut(QObject::tr("Shift+F"));
//    flatSurfaceAction->setStatusTip(QObject::tr("Flat surface"));
//    flatSurfaceAction->setCheckable(true);
//    connect(flatSurfaceAction, SIGNAL(triggered()), this,
//        SLOT(setFlatSurface()));
        
//    flatLinesAction = new QAction(QObject::tr("Flat &Lines"), this);
//    flatLinesAction->setShortcut(QObject::tr("Shift+L"));
//    flatLinesAction->setStatusTip(QObject::tr("Flat surface & lines"));
//    flatLinesAction->setCheckable(true);
//    connect(flatLinesAction, SIGNAL(triggered()), this,
//        SLOT(setFlatLines()));
        
//    wireFrameAction = new QAction(QObject::tr("&Wire Frame"), this);
//    wireFrameAction->setShortcut(QObject::tr("Shift+W"));
//    wireFrameAction->setStatusTip(QObject::tr("Wire frame"));
//    wireFrameAction->setCheckable(true);
//    connect(wireFrameAction, SIGNAL(triggered()), this,
//        SLOT(setWireFrame()));
        
//    meshPointAction = new QAction(QObject::tr("&Points"), this);
//    meshPointAction->setShortcut(QObject::tr("Shift+P"));
//    meshPointAction->setStatusTip(QObject::tr("Point"));
//    meshPointAction->setCheckable(true);
//    connect(meshPointAction, SIGNAL(triggered()), this,
//        SLOT(setMeshPoint()));
        
//    QActionGroup *meshModeGroup = new QActionGroup(this);
//    meshModeGroup->addAction(smoothSurfaceAction);
//    meshModeGroup->addAction(flatSurfaceAction);
//    meshModeGroup->addAction(flatLinesAction);
//    meshModeGroup->addAction(wireFrameAction);
//    meshModeGroup->addAction(meshPointAction);
//    smoothSurfaceAction->setChecked(true);
    
//    if (meshPanel)
//    {
//        toggleMeshPanelAction = meshPanel->toggleViewAction();
//        toggleMeshPanelAction->setIcon(QIcon(":/images/list.png"));
//    }
//    else
//        toggleMeshPanelAction = NULL;
       
//    infoAction = new QAction(QObject::tr("&Info"), this);
//    infoAction->setIcon(QIcon(":/images/info.png"));
//    infoAction->setShortcut(QObject::tr("Ctrl+I"));
//    infoAction->setStatusTip(QObject::tr("Data information"));
//    connect(infoAction, SIGNAL(triggered()), this,
//        SLOT(info()));

//    helpAction = new QAction(QObject::tr("&Help"), this);
//    helpAction->setIcon(QIcon(":/images/help.png"));
//    helpAction->setShortcut(QObject::tr("Ctrl+H"));
//    helpAction->setStatusTip(QObject::tr("Help information"));
//    connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));
        
//    aboutAction = new QAction(QObject::tr("About"), this);
//    aboutAction->setStatusTip(QObject::tr("About this software"));
//    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}


void MedNUSMeshViewer::createMenus()
{
//    fileMenu = menuBar()->addMenu(QObject::tr("&File"));
//    fileMenu->addAction(newProjectAction);
//    fileMenu->addAction(openProjectAction);
//    fileMenu->addAction(saveProjectAction);
//    fileMenu->addAction(saveProjectAsAction);
//    fileMenu->addSeparator();

//    fileMenu->addAction(loadDirAction);
//    fileMenu->addAction(loadMeshAction);
//    fileMenu->addAction(addMeshAction);
    
//    saveMeshMenu = fileMenu->addMenu(QObject::tr("&Save Visible Meshes"));
//    saveMeshMenu->setIcon(QIcon("images/savedir.png"));
//    saveMeshMenu->addAction(saveDirPlyAction);
//    saveMeshMenu->addAction(saveDirStlAction);
    
//    fileMenu->addSeparator();
//    fileMenu->addAction(exitAction);

//    //viewMenu = menuBar()->addMenu(QObject::tr("&View"));
//    viewMenu->addAction(saveViewAction);
//    viewMenu->addAction(frontFaceAction);
    
//    //meshModeMenu = viewMenu->addMenu(QObject::tr("&Mesh Mode"));
//    meshModeMenu->addAction(smoothSurfaceAction);
//    meshModeMenu->addAction(flatSurfaceAction);
//    meshModeMenu->addAction(flatLinesAction);
//    meshModeMenu->addAction(wireFrameAction);
//    meshModeMenu->addAction(meshPointAction);

//    if (toggleMeshPanelAction)
//        viewMenu->addAction(toggleMeshPanelAction);
//    viewMenu->addAction(infoAction);

//    menuBar()->addSeparator();

//    helpMenu = menuBar()->addMenu(QObject::tr("&Help"));
//    helpMenu->addAction(helpAction);
//    helpMenu->addAction(aboutAction);
}


void MedNUSMeshViewer::createToolBars()
{
    //
    //fileToolBar = addToolBar(QObject::tr("&File"));
//    fileToolBar = new QToolBar("File", this);
//    fileToolBar->addAction(newProjectAction);
//    fileToolBar->addAction(openProjectAction);
//    fileToolBar->addAction(saveProjectAction);
//    fileToolBar->addAction(loadDirAction);
//    fileToolBar->addAction(loadMeshAction);
//    fileToolBar->addAction(addMeshAction);
//    //this->layout()->addWidget(fileToolBar);
    
//    //viewToolBar = addToolBar(QObject::tr("&View"));
//    //viewToolBar = new QToolBar("View", this);
//    fileToolBar->addAction(saveViewAction);
//    fileToolBar->addAction(frontFaceAction);
//    fileToolBar->addWidget(meshModeBox);
//    //
    
//    if (toggleMeshPanelAction)
//        fileToolBar->addAction(toggleMeshPanelAction);
//    fileToolBar->addAction(infoAction);
//    //this->layout()->addWidget(viewToolBar);

//    //fileToolBar = new QToolBar("Help", this);
//    fileToolBar->addAction(helpAction);
    //this->layout()->addWidget(fileToolBar);
}


void MedNUSMeshViewer::createStatusBar()
{
    //statusBar()->showMessage(QObject::tr(""));
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
            if (loadMesh(fileNames))
                readDir = dirName;
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
    QString dirName = QFileDialog::getExistingDirectory(this,
        QObject::tr("Save visible meshes into a directory in PLY format"),
        writeDir);
        
    if (!dirName.isEmpty() && !meshModel.isEmpty())
    {
        for (int i = 0; i < meshModel.size(); ++i)
            if (meshModel.getActor(i)->GetVisibility())
            {
                QString fileName = dirName + QString("/%1.ply").
                    arg(i, 8, 10, QChar('0'));
                vtkPLYWriter *writer = vtkPLYWriter::New();
                writer->SetFileName(fileName.toLatin1().data());
                writer->SetInputData(meshModel.getData(i));
                writer->Update();
                writer->Delete();
            }
            
        writeDir = dirName;
    }
}


void MedNUSMeshViewer::saveDirStl()
{
    QString dirName = QFileDialog::getExistingDirectory(this,
        QObject::tr("Save visible meshes into a directory in STL format"),
        writeDir);
        
    if (!dirName.isEmpty() && !meshModel.isEmpty())
    {
        for (int i = 0; i < meshModel.size(); ++i)
            if (meshModel.getActor(i)->GetVisibility())
            {
                QString fileName = dirName + QString("/%1.stl").
                    arg(i, 8, 10, QChar('0'));
                vtkSTLWriter *writer = vtkSTLWriter::New();
                writer->SetFileName(fileName.toLatin1().data());
                writer->SetInputData(meshModel.getData(i));
                writer->Update();
                writer->Delete();
            }
            
        writeDir = dirName;
    }
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
    if (meshModel.isEmpty())
        return;
        
    hideFrontFace = !hideFrontFace;
    
    for (int i = 0; i < meshModel.size(); ++i)
        meshModel.getActor(i)->GetProperty()->
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
    
    for (int i = 0; i < meshModel.size(); ++i)
    {
        vtkProperty *property = meshModel.getActor(i)->GetProperty();
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
    QString msg;
    
    if (loaded)
    {
        msg = QString("Project name: %1<br><br>").arg(projectName);
        
        int numFaces = 0;
        long memSize = 0;
        
        for (int i = 0; i < meshModel.size(); ++i)
        {
            vtkPolyData *data = meshModel.getData(i);
            numFaces += data->GetPolys()->GetNumberOfCells();
            memSize += data->GetPolys()->GetActualMemorySize();
        }
        
        msg += QString("%1 faces, %2 MB<br>").
           arg(numFaces).arg(memSize / 1000.0, 0, 'f', 2);
    }
    else
        msg = QString("No mesh loaded.");
        
           
    QMessageBox::about(this, QObject::tr("Data Information"),
        QString("<h2>%1</h2> <p></p>").arg(appName) + msg);
}


void MedNUSMeshViewer::help()
{
    QMessageBox::about(this, QObject::tr("Help information"),
       QString("<h2>%1</h2>").arg(appName) +
       "<p></p>" +
       "<p>Control keys:<br><br>" +
       "left mouse button: camera rotate<br>" +
       "SHIFT + left mouse button: camera pan<br>" +
       "CTRL + left mouse button: camera spin<br>" +
       "middle mouse wheel: zoom<br>" +
       "middle mouse button: camera pan<br>" +
       "right mouse button: camera zoom<br>" +
       "r: reset camera<br><br>" +
       "right mouse click: select mesh<br>" +
       "DEL: delete selected meshes<br>");
}


void MedNUSMeshViewer::about()
{
    QMessageBox::about(this, QString("About %1").arg(appName),
       QString("<h2>%1</h2>").arg(appName) +
       "<p>Copyright &copy; 2012, 2013, 2014<br>" +
       "Leow Wee Kheng<br>" +
       "Department of Computer Science<br>" +
       "National University of Singapore<p></p>" +
       "Implemented using Qt, VTK.");
}


void MedNUSMeshViewer::setAppName(const QString &name)
{
    appName = name;
}


void MedNUSMeshViewer::setVisibility(int row, bool visible)
{
   if (row < 0 || row >= meshModel.size())
       return;
       
    meshModel.getActor(row)->SetVisibility(visible ? 1 : 0);
    renderWindow->Render();
}


void MedNUSMeshViewer::setColor(int row, QColor color)
{
   if (row < 0 || row >= meshModel.size())
       return;
    
    double red = color.red() / 255.0;
    double green = color.green() / 255.0;
    double blue = color.blue() / 255.0;
    meshModel.getActor(row)->GetProperty()->SetColor(red, green, blue);
    renderWindow->Render();
}


void MedNUSMeshViewer::setTransparency(int row, float value)
{
   if (row < 0 || row >= meshModel.size())
       return;
    
    meshModel.getActor(row)->GetProperty()->SetOpacity(1.0 - value);
    renderWindow->Render();
}


void MedNUSMeshViewer::blink(int row)
{
   if (row < 0 || row >= meshModel.size())
       return;
        
    double red, green, blue, r, g, b;
    vtkProperty *property = meshModel.getActor(row)->GetProperty();
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
   if (row < 0 || row >= meshModel.size())
       return;
       
    renderer->RemoveActor(meshModel.getActor(row));
    meshModel.removeAt(row);
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
    
    for (int i = startIndex; i < meshModel.size(); ++i){
        renderer->AddActor(meshModel.getActor(i));
        meshModel.getActor(i)->SetScale(i*0.1 + 1);
}
    //****CRAPPY TEST CODE


    //drawSphere(20,-1,-1,-1);
    //drawSphere(20,1,1,1);
    //drawBoundingBox();

    //****END OF CRAPPY CODE

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
    if (renderer)
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
    int startIndex = meshModel.size();
    
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
        reader->Update();
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
            meshTable->append(name, source);
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
        for (int i = 0; i < meshModel.size(); ++i)
            if (meshModel.getActor(i) == actor)
                meshTable->setCurrentCell(i, NameColumn);
    }
}




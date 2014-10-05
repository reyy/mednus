/* PMeshViewer.cpp

   3D mesh viewer.
   
   Copyright 2012, 2103, 2014 National University of Singapore
   Author: Leow Wee Kheng
*/


#include "PMeshViewer.h"
#include <QtGui>

#include "PMeshViewerCallback.h"


#include <unistd.h>
using namespace std;




// PMeshViewer class

PMeshViewer::PMeshViewer(bool withMeshPanel)
{
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

    //drawSphere(0.5,-1,-1,-1);
    //drawSphere(0.8,2,2,2);
    //drawBoundingBox();
}


PMeshViewer::~PMeshViewer()
{
    //uninstallPipeline();
    //style->Delete();
}


// Event handling

void PMeshViewer::closeEvent(QCloseEvent *event)
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

void PMeshViewer::createWidgets(bool withMeshPanel)
{




    if (withMeshPanel)
    {
        PMeshViewerCallback *callback = PMeshViewerCallback::New();
        callback->viewer = this;
        //interactor->AddObserver(vtkCommand::RightButtonPressEvent, callback);
        callback->Delete();
    }
    
    // Central widget
    QGridLayout *grid = new QGridLayout;
    //grid->addWidget(vtkWidget, 0, 0);

    QWidget *main = new QWidget;
    main->setLayout(grid);
    main->setStyleSheet("background-color: #285183;");
    setCentralWidget(main); 
    
    // Overall
    setWindowTitle(appName);
    setWindowIcon(QIcon(":/images/panax-icon.png"));
    
    // Create mesh mode box
    meshModeBox = new QComboBox(this);
    QStringList choices;
    choices << "smooth surface" << "flat surface" << "flat lines" 
        << "wire frame" << "points";
    meshModeBox->insertItems(0, choices);
    connect(meshModeBox, SIGNAL(activated(int)), this,
        SLOT(setMeshMode(int)));
    
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

        // Create lesson table
        lessonTable = new MedNUSLessonPanel;
        lessonTable->setStyleSheet("background-color: #285183;");
        
        // Create dock widget for bottom dock area.
        lessonPanel = new QDockWidget("Lesson List");
        lessonPanel->setWidget(lessonTable);
        lessonPanel->setAllowedAreas(Qt::RightDockWidgetArea);
        lessonPanel->setStyleSheet("background-color: #285183;");
        addDockWidget(Qt::RightDockWidgetArea, lessonPanel);

        // Create dock widget for bottom dock area.
        meshPanel = new QDockWidget("Mesh List");
        meshPanel->setWidget(meshTable);
        meshPanel->setAllowedAreas(Qt::BottomDockWidgetArea);
        meshPanel->setStyleSheet("background-color: #285183;");
        addDockWidget(Qt::BottomDockWidgetArea, meshPanel);
    }
    else
    {
        meshTable = NULL;
        meshPanel = NULL;
        lessonTable = NULL;
        lessonPanel = NULL;
    }
    
    winWidth = 850;
    winHeight = 600;
    setMinimumSize(winWidth, winHeight);
    //vtkWidget->updateGeometry();
}


void PMeshViewer::createActions()
{
    newProjectAction = new QAction(QObject::tr("&New Project"), this);
    newProjectAction->setIcon(QIcon(":/images/new.png"));
    newProjectAction->setShortcut(QObject::tr("Ctrl+N"));
    newProjectAction->setStatusTip(QObject::tr("Create a new project"));
    connect(newProjectAction, SIGNAL(triggered()),
        this, SLOT(newProject()));
    
    openProjectAction = new QAction(QObject::tr("&Open Project"), this);
    openProjectAction->setIcon(QIcon(":/images/open.png"));
    openProjectAction->setShortcut(QObject::tr("Ctrl+O"));
    openProjectAction->setStatusTip(QObject::tr("Open a project file"));
    connect(openProjectAction, SIGNAL(triggered()),
        this, SLOT(openProject()));

    saveProjectAction = new QAction(QObject::tr("&Save Project"), this);
    saveProjectAction->setIcon(QIcon(":/images/save.png"));
    saveProjectAction->setShortcut(QObject::tr("Ctrl+S"));
    saveProjectAction->setStatusTip(QObject::tr("Save project to file"));
    connect(saveProjectAction, SIGNAL(triggered()),
        this, SLOT(saveProject()));

    saveProjectAsAction = new QAction(QObject::tr("Save Project &As"), this);
    saveProjectAsAction->setIcon(QIcon(":/images/saveas.png"));
    saveProjectAsAction->setShortcut(QObject::tr("Ctrl+A"));
    saveProjectAsAction->setStatusTip(QObject::tr("Save project to another file"));
    connect(saveProjectAsAction, SIGNAL(triggered()),
        this, SLOT(saveProjectAs()));
    
    loadDirAction = new QAction(QObject::tr("Load Mesh by &Directory"), this);
    loadDirAction->setIcon(QIcon(":/images/loaddir.png"));
    loadDirAction->setShortcut(QObject::tr("Ctrl+D"));
    loadDirAction->setStatusTip(QObject::tr("Load mesh files in a directory"));
    connect(loadDirAction, SIGNAL(triggered()), this, SLOT(loadDir()));
    
    loadMeshAction = new QAction(QObject::tr("Load Mesh by &File"), this);
    loadMeshAction->setIcon(QIcon(":/images/loadmesh.png"));
    loadMeshAction->setShortcut(QObject::tr("Ctrl+F"));
    loadMeshAction->setStatusTip(QObject::tr("Load meshes by file names"));
    connect(loadMeshAction, SIGNAL(triggered()), this, SLOT(loadMesh()));
    
    addMeshAction = new QAction(QObject::tr("Add Mesh by File"), this);
    addMeshAction->setIcon(QIcon(":/images/addmesh.png"));
    addMeshAction->setStatusTip(QObject::tr("Add meshes by file names"));
    connect(addMeshAction, SIGNAL(triggered()), this, SLOT(addMesh()));
        
    saveDirPlyAction = new QAction(QObject::tr("in PLY format"), this);
    connect(saveDirPlyAction, SIGNAL(triggered()), this, SLOT(saveDirPly()));

    saveDirStlAction = new QAction(QObject::tr("in STL format"), this);
    connect(saveDirStlAction, SIGNAL(triggered()), this, SLOT(saveDirStl()));
    
    exitAction = new QAction(QObject::tr("E&xit"), this);
    exitAction->setShortcut(QObject::tr("Ctrl+Q"));
    exitAction->setStatusTip(QObject::tr("Exit Mesh Viewer"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    
    saveViewAction = new QAction(QObject::tr("Save &View"), this);
    saveViewAction->setIcon(QIcon(":/images/saveview.png"));
    saveViewAction->setShortcut(QObject::tr("Shift+V"));
    saveViewAction->setStatusTip(QObject::tr("Save current view to a file"));
    connect(saveViewAction, SIGNAL(triggered()), this, SLOT(saveView()));
    
    frontFaceAction = new QAction(QObject::tr("&Hide Front Faces"), this);
    frontFaceAction->setIcon(QIcon(":/images/hide.png"));
    frontFaceAction->setShortcut(QObject::tr("Shift+H"));
    frontFaceAction->setStatusTip(QObject::tr("Hide/show front faces"));
    frontFaceAction->setCheckable(true);
    connect(frontFaceAction, SIGNAL(triggered()), this,
        SLOT(toggleFrontFace()));
        
    smoothSurfaceAction = new QAction(QObject::tr("&Smooth Surface"), this);
    smoothSurfaceAction->setShortcut(QObject::tr("Shift+S"));
    smoothSurfaceAction->setStatusTip(QObject::tr("Smooth surface"));
    smoothSurfaceAction->setCheckable(true);
    connect(smoothSurfaceAction, SIGNAL(triggered()), this,
        SLOT(setSmoothSurface()));
        
    flatSurfaceAction = new QAction(QObject::tr("&Flat Surface"), this);
    flatSurfaceAction->setShortcut(QObject::tr("Shift+F"));
    flatSurfaceAction->setStatusTip(QObject::tr("Flat surface"));
    flatSurfaceAction->setCheckable(true);
    connect(flatSurfaceAction, SIGNAL(triggered()), this,
        SLOT(setFlatSurface()));
        
    flatLinesAction = new QAction(QObject::tr("Flat &Lines"), this);
    flatLinesAction->setShortcut(QObject::tr("Shift+L"));
    flatLinesAction->setStatusTip(QObject::tr("Flat surface & lines"));
    flatLinesAction->setCheckable(true);
    connect(flatLinesAction, SIGNAL(triggered()), this,
        SLOT(setFlatLines()));
        
    wireFrameAction = new QAction(QObject::tr("&Wire Frame"), this);
    wireFrameAction->setShortcut(QObject::tr("Shift+W"));
    wireFrameAction->setStatusTip(QObject::tr("Wire frame"));
    wireFrameAction->setCheckable(true);
    connect(wireFrameAction, SIGNAL(triggered()), this,
        SLOT(setWireFrame()));
        
    meshPointAction = new QAction(QObject::tr("&Points"), this);
    meshPointAction->setShortcut(QObject::tr("Shift+P"));
    meshPointAction->setStatusTip(QObject::tr("Point"));
    meshPointAction->setCheckable(true);
    connect(meshPointAction, SIGNAL(triggered()), this,
        SLOT(setMeshPoint()));
        
    QActionGroup *meshModeGroup = new QActionGroup(this);
    meshModeGroup->addAction(smoothSurfaceAction);
    meshModeGroup->addAction(flatSurfaceAction);
    meshModeGroup->addAction(flatLinesAction);
    meshModeGroup->addAction(wireFrameAction);
    meshModeGroup->addAction(meshPointAction);
    smoothSurfaceAction->setChecked(true);
    
    if (meshPanel)
    {
        toggleMeshPanelAction = meshPanel->toggleViewAction();
        toggleMeshPanelAction->setIcon(QIcon(":/images/list.png"));
    }
    else
        toggleMeshPanelAction = NULL;
       
    infoAction = new QAction(QObject::tr("&Info"), this);
    infoAction->setIcon(QIcon(":/images/info.png"));
    infoAction->setShortcut(QObject::tr("Ctrl+I"));
    infoAction->setStatusTip(QObject::tr("Data information"));
    connect(infoAction, SIGNAL(triggered()), this,
        SLOT(info()));

    helpAction = new QAction(QObject::tr("&Help"), this);
    helpAction->setIcon(QIcon(":/images/help.png"));
    helpAction->setShortcut(QObject::tr("Ctrl+H"));
    helpAction->setStatusTip(QObject::tr("Help information"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));
        
    aboutAction = new QAction(QObject::tr("About"), this);
    aboutAction->setStatusTip(QObject::tr("About this software"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}


void PMeshViewer::createMenus()
{
    fileMenu = menuBar()->addMenu(QObject::tr("&File"));
    fileMenu->addAction(newProjectAction);
    fileMenu->addAction(openProjectAction);
    fileMenu->addAction(saveProjectAction);
    fileMenu->addAction(saveProjectAsAction);
    fileMenu->addSeparator();

    fileMenu->addAction(loadDirAction);
    fileMenu->addAction(loadMeshAction);
    fileMenu->addAction(addMeshAction);
    
    saveMeshMenu = fileMenu->addMenu(QObject::tr("&Save Visible Meshes"));
    saveMeshMenu->setIcon(QIcon("images/savedir.png"));
    saveMeshMenu->addAction(saveDirPlyAction);
    saveMeshMenu->addAction(saveDirStlAction);
    
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    viewMenu = menuBar()->addMenu(QObject::tr("&View"));
    viewMenu->addAction(saveViewAction);
    viewMenu->addAction(frontFaceAction);
    
    meshModeMenu = viewMenu->addMenu(QObject::tr("&Mesh Mode"));
    meshModeMenu->addAction(smoothSurfaceAction);
    meshModeMenu->addAction(flatSurfaceAction);
    meshModeMenu->addAction(flatLinesAction);
    meshModeMenu->addAction(wireFrameAction);
    meshModeMenu->addAction(meshPointAction);

    if (toggleMeshPanelAction)
        viewMenu->addAction(toggleMeshPanelAction);
    viewMenu->addAction(infoAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(QObject::tr("&Help"));
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);
}


void PMeshViewer::createToolBars()
{
    fileToolBar = addToolBar(QObject::tr("&File"));
    fileToolBar->addAction(newProjectAction);
    fileToolBar->addAction(openProjectAction);
    fileToolBar->addAction(saveProjectAction);
    fileToolBar->addAction(loadDirAction);
    fileToolBar->addAction(loadMeshAction);
    fileToolBar->addAction(addMeshAction);
    
    viewToolBar = addToolBar(QObject::tr("&View"));
    viewToolBar->addAction(saveViewAction);
    viewToolBar->addAction(frontFaceAction);
    viewToolBar->addWidget(meshModeBox);
    
    if (toggleMeshPanelAction)
        viewToolBar->addAction(toggleMeshPanelAction);
    viewToolBar->addAction(infoAction);

    helpToolBar = addToolBar(QObject::tr("&Help"));
    helpToolBar->addAction(helpAction);
}


void PMeshViewer::createStatusBar()
{
    statusBar()->showMessage(QObject::tr(""));
}


// Slot methods

void PMeshViewer::newProject()
{
//    uninstallPipeline();
//    renderer = vtkRenderer::New();
//    renderer->SetBackground(0.0, 0.0, 0.0);
//    renderWindow->AddRenderer(renderer);
//    renderWindow->Render();
}


void PMeshViewer::openProject()
{
}


void PMeshViewer::saveProject()
{
}


void PMeshViewer::saveProjectAs()
{
}



void PMeshViewer::saveDirPly()
{
//    QString dirName = QFileDialog::getExistingDirectory(this,
//        QObject::tr("Save visible meshes into a directory in PLY format"),
//        writeDir);
        
//    if (!dirName.isEmpty() && !meshModel.isEmpty())
//    {
//        for (int i = 0; i < meshModel.size(); ++i)
//            if (meshModel.getActor(i)->GetVisibility())
//            {
//                QString fileName = dirName + QString("/%1.ply").
//                    arg(i, 8, 10, QChar('0'));
//                vtkPLYWriter *writer = vtkPLYWriter::New();
//                writer->SetFileName(fileName.toLatin1().data());
//                writer->SetInputData(meshModel.getData(i));
//                writer->Update();
//                writer->Delete();
//            }
            
//        writeDir = dirName;
//    }

}


void PMeshViewer::saveDirStl()
{
//    QString dirName = QFileDialog::getExistingDirectory(this,
//        QObject::tr("Save visible meshes into a directory in STL format"),
//        writeDir);
        
//    if (!dirName.isEmpty() && !meshModel.isEmpty())
//    {
//        for (int i = 0; i < meshModel.size(); ++i)
//            if (meshModel.getActor(i)->GetVisibility())
//            {
//                QString fileName = dirName + QString("/%1.stl").
//                    arg(i, 8, 10, QChar('0'));
//                vtkSTLWriter *writer = vtkSTLWriter::New();
//                writer->SetFileName(fileName.toLatin1().data());
//                writer->SetInputData(meshModel.getData(i));
//                writer->Update();
//                writer->Delete();
//            }
            
//        writeDir = dirName;
//    }
}


void PMeshViewer::saveView()
{
//    if (!loaded)  // Nothing to save.
//        return;
        
//    QString fileName = QFileDialog::getSaveFileName(this,
//        QObject::tr("Save current view to a file"), writeDir,
//        QObject::tr("Image files: *.jpg, *.png, *.tif (*.jpg *.png *.tif)"));
        
//    if (fileName.isEmpty())
//        return;

//    if (saveImage(fileName))
//        writeDir = QFileInfo(fileName).path();
}





void PMeshViewer::info()
{
    QString msg;
    
    if (loaded)
    {
        msg = QString("Project name: %1<br><br>").arg(projectName);
        
//        int numFaces = 0;
//        long memSize = 0;
        
//        for (int i = 0; i < meshModel.size(); ++i)
//        {
//            vtkPolyData *data = meshModel.getData(i);
//            numFaces += data->GetPolys()->GetNumberOfCells();
//            memSize += data->GetPolys()->GetActualMemorySize();
//        }
        
//        msg += QString("%1 faces, %2 MB<br>").
//           arg(numFaces).arg(memSize / 1000.0, 0, 'f', 2);
    }
    else
        msg = QString("No mesh loaded.");
        
           
    QMessageBox::about(this, QObject::tr("Data Information"),
        QString("<h2>%1</h2> <p></p>").arg(appName) + msg);
}


void PMeshViewer::help()
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


void PMeshViewer::about()
{
    QMessageBox::about(this, QString("About %1").arg(appName),
       QString("<h2>%1</h2>").arg(appName) +
       "<p>Copyright &copy; 2012, 2013, 2014<br>" +
       "Leow Wee Kheng<br>" +
       "Department of Computer Science<br>" +
       "National University of Singapore<p></p>" +
       "Implemented using Qt, VTK.");
}


void PMeshViewer::setAppName(const QString &name)
{
    appName = name;
}







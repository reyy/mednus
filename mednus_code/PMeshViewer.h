/* PMeshViewer.h

   3D mesh viewer.

   Copyright 2012, 2103, 2014 National University of Singapore
   Author: Leow Wee Kheng
*/

#ifndef PMESHVIEWER_H
#define PMESHVIEWER_H
#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
#include <QDockWidget>
#include <QComboBox>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QGridLayout>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTranslator>
#include <QObject>
#include <QString>
#include "PMeshTableWidget.h"
#include "MedNUSLessonPanel.h"

class QAction;
class QComboBox;
class QDoubleSpinBox;
class QPushButton;

#include "QVTKWidget.h"
#include "vtkPolyDataAlgorithm.h"
#include "vtkPolyDataNormals.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkPropPicker.h"
#include "vtkMINCImageWriter.h"
#include "vtkMINCImageReader.h"
#include "vtkObjectBase.h"

// class PMeshPart
// {
//     friend class PMeshViewer;
//     QString name;
//     QString source;
//     vtkPolyData *data;
//     vtkPolyDataNormals *normals;
//     vtkPolyDataMapper *mapper;
//     vtkActor *actor;
//     QColor color;
//     float transparency;
// };


class PMeshViewer: public QMainWindow
{
    Q_OBJECT

public:
    PMeshViewer(bool withMeshPanel);
    ~PMeshViewer();
    void setAppName(const QString &name);
   

    
protected:
    void closeEvent(QCloseEvent *event);
    //Test Code Below

    
protected slots:
    void newProject();
    void openProject();
    void saveProject();
    void saveProjectAs();

    void saveDirPly();
    void saveDirStl();
    void saveView();

    void info();
    void help();
    void about();



protected:
    void createWidgets(bool withMeshPanel);
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    QAction *newProjectAction;
    QAction *openProjectAction;
    QAction *saveProjectAction;
    QAction *saveProjectAsAction;
    QAction *loadDirAction;
    QAction *loadMeshAction;
    QAction *addMeshAction;
    QAction *saveDirPlyAction;
    QAction *saveDirStlAction;
    QAction *exitAction;
    QAction *saveViewAction;
    QAction *frontFaceAction;
    QAction *smoothSurfaceAction;
    QAction *flatSurfaceAction;
    QAction *flatLinesAction;
    QAction *wireFrameAction;
    QAction *meshPointAction;
    QAction *toggleMeshPanelAction;
    QAction *infoAction;
    QAction *helpAction;
    QAction *aboutAction;
    
    QMenu *fileMenu;
    QMenu *saveMeshMenu;
    QMenu *viewMenu;
    QMenu *meshModeMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    
    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QToolBar *windowToolBar;
    QToolBar *helpToolBar;

    // Widgets

    QComboBox *meshModeBox;
    QDockWidget *meshPanel;
    QDockWidget *lessonPanel;
    PMeshTableWidget *meshTable;
    MedNUSLessonPanel *lessonTable;
//     QList<PMeshPart> meshList;


    
    // Internal variables.
    QString appName;
    QString projectName;
    QString readDir;  // Current read directory
    QString writeDir; // Current write directory
    bool loaded;
    int winWidth, winHeight;
    int hideFrontFace;
    

};

 //Callback class




#endif

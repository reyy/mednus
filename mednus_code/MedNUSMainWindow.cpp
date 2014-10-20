#include "MedNUSMainWindow.h"

MedNUSMainWindow::MedNUSMainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setWindowTitle(QString("MedNUS"));
    setWindowIcon(QIcon(":/images/panax-icon.png"));

    int winWidth = 800;
    int winHeight = 600;
    setMinimumSize(winWidth, winHeight);

    createWidgets();
    createMenus();
}

void MedNUSMainWindow::createWidgets()
{
    //Tab (Push out to new class)
    tabs = new QTabWidget();
    tabs->setTabsClosable(true);
    tabs->setMovable(true);
    tabs->setDocumentMode(true);
    tabs->setUsesScrollButtons(true);
    tabs->setElideMode(Qt::ElideRight);
    tabs->setTabShape(QTabWidget::Rounded);
    setCentralWidget(tabs);

    //Add Model
    MedNUSMeshViewer *view = new MedNUSMeshViewer(false);
    vid = new MedNUSVideoViewer();
    tabs->addTab(view,"Skull Model");
    tabs->addTab(new MedNUSPdfViewer(),"PDF Test");
    tabs->addTab(vid, "Video");
    //tabs->addTab(new QWidget(),"TAB 2");

    // Create lesson table
    MedNUSLessonPanel *lessonTable;
    lessonTable = new MedNUSLessonPanel(this);
    lessonTable->addLesson("LSM 1301 - Biology","Professor Gopal","Module about biology.");
    lessonTable->addLesson("LSM 1302 - Difficult Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1303 - Very difficult Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1304 - Crazy Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1301 - Biology","Professor Gopal","Module about biology.");
    lessonTable->addLesson("LSM 1302 - Difficult Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1303 - Very difficult Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1304 - Crazy Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1301 - Biology","Professor Gopal","Module about biology.");
    lessonTable->addLesson("LSM 1302 - Difficult Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1303 - Very difficult Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1304 - Crazy Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1301 - Biology","Professor Gopal","Module about biology.");
    lessonTable->addLesson("LSM 1302 - Difficult Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1303 - Very difficult Biology","Professor Gopal","Module about more biology.");
    lessonTable->addLesson("LSM 1304 - Crazy Biology","Professor Gopal","Module about more biology.");

    // Create dock widget for bottom dock area.
    QDockWidget *lessonPanel;
    lessonPanel = new QDockWidget("Lesson Panel");
    lessonPanel->setWidget(lessonTable);

    lessonPanel->setTitleBarWidget(0);
    //lessonPanel->setAllowedAreas(Qt::RightDockWidgetArea);
    lessonPanel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    addDockWidget(Qt::RightDockWidgetArea, lessonPanel);
}

void MedNUSMainWindow::createMenus()
{
    //
}

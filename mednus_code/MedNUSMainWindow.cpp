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
    //Logo Display
    QLabel *label = new QLabel(this);
    label->setGeometry(QRect(20, 10, 114, 31));
    label->setPixmap(QPixmap(QString::fromStdString(":/images/logo.png")));

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
    tabs->addTab(vid, "Video");
    tabs->addTab(new QWidget(),"TAB 2");

    // Create lesson table
    MedNUSLessonPanel *lessonTable;
    lessonTable = new MedNUSLessonPanel;
    lessonTable->setStyleSheet("background-color: #285183;");

    // Create dock widget for bottom dock area.
    QDockWidget *lessonPanel;
    lessonPanel = new QDockWidget("Lesson List");
    lessonPanel->setWidget(lessonTable);
    lessonPanel->setAllowedAreas(Qt::RightDockWidgetArea);
    lessonPanel->setStyleSheet("background-color: #285183;");
    addDockWidget(Qt::RightDockWidgetArea, lessonPanel);
}

void MedNUSMainWindow::createMenus()
{
    //
}

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
     QWidget *centralWidget = new QWidget(this);
     QGridLayout *mainLayout = new QGridLayout();
     mainLayout->setColumnStretch(0,1);
     mainLayout->setColumnStretch(1,0);
     mainLayout->setRowStretch(0,0);
     mainLayout->setRowStretch(0,1);
     mainLayout->setContentsMargins(0,0,0,0);
     mainLayout->setSpacing(0);
     mainLayout->setSizeConstraint(QLayout::SetNoConstraint);

    //Add FrontBar (Top bar that has logo)
     MedNUSFrontBar *fb = new MedNUSFrontBar(this);
     fb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
     mainLayout->addWidget(fb,0,0);

    //Add UserBar (Top right bar that has user info)
     MedNUSUserBar *ub = new MedNUSUserBar(this);
     ub->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
     mainLayout->addWidget(ub,0,1);

    //Add Content View
    tabs = new MedNUSContentPanel();
    //Add Model
    MedNUSMeshViewer *view = new MedNUSMeshViewer(false);
    vid = new MedNUSVideoViewer();
    tabs->addTab(view,"Skull Model");
    tabs->addTab(new MedNUSPdfViewer(),"PDF Test");
    tabs->addTab(vid, "Video");
    //tabs->addTab(new QWidget(),"TAB 2");
    tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(tabs,1,0);

    // Create lesson table
    MedNUSLessonPanel *lessonTable;
    lessonTable = new MedNUSLessonPanel(this);
    QStringList content;
    content.push_back(":/content/test.pdf");
    content.push_back(":/content/samplevideo.mp4");
    lessonTable->addLesson("LSM 1301 - Biology","Professor Gopal","Module about biology.",content);
    /*lessonTable->addLesson("LSM 1302 - Difficult Biology","Professor Gopal","Module about more biology.");
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
    lessonTable->addLesson("LSM 1304 - Crazy Biology","Professor Gopal","Module about more biology.");*/

    lessonTable->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    mainLayout->addWidget(lessonTable,1,1);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


}

void MedNUSMainWindow::createMenus()
{
    //
}

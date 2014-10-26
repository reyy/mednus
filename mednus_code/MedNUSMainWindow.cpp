#include "MedNUSMainWindow.h"
#include <QDir>

MedNUSMainWindow::MedNUSMainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setWindowTitle(QString("MedNUS"));
    setWindowIcon(QIcon(":/images/panax-icon.png"));

    int winWidth = 800;
    int winHeight = 600;
    setMinimumSize(winWidth, winHeight);

    _trayOut=true;

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
     ub = new MedNUSUserBar(this);
     ub->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
     mainLayout->addWidget(ub,0,1);

    //Add Content View
    tabs = new MedNUSContentPanel();
    //Add Model
    QString dir = "";
    dir.append(QDir::homePath());
    dir.append("/mednus/lesson1/models/craniofacial.ply");
    MedNUSMeshViewer *view = new MedNUSMeshViewer(dir,false);
    tabs->addTab(view,"Skull Model");

    dir = "";
    dir.append(QDir::homePath());
    dir.append("/mednus/lesson1/pdf/thorax.ppt.pdf");
    tabs->addTab(new MedNUSPdfViewer(dir),"PDF Test");

    dir = "";
    dir.append(QDir::homePath());
    dir.append("/mednus/lesson1/videos/Untitled.mov");
    vid = new MedNUSVideoViewer(dir);
    tabs->addTab(vid, "Video");

    tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(tabs,1,0);

    // Create lesson table
    lp = new MedNUSLessonPanel(this);
    QStringList content;
    content.push_back(":/content/test.pdf");
    content.push_back(":/content/samplevideo.mp4");
    lp->addLesson("LSM 1301 - Biology","Professor Gopal","Module about biology.",content);

    lp->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    mainLayout->addWidget(lp,1,1);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


}

void MedNUSMainWindow::createMenus()
{
    //
}

void MedNUSMainWindow::mousePressEvent ( QMouseEvent * event )
{
    if(event->buttons() == Qt::LeftButton) {
        if(lp!=NULL) {
            if(lp->checkTrayButton(event->x(),event->y())) {
                _trayOut=!_trayOut;
            }

            lp->setTrayOut(_trayOut);
            ub->setTrayOut(_trayOut);
        }
    }
}

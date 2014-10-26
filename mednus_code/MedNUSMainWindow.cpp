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

    _trayOut=false;
    _widgetsCreated=false;
    _menuCreated=false;

    login = new MedNUSLogin(this);
    setCentralWidget(login);
    //createWidgets();
    //createMenus();
    //_layoutCreated=true;
}

MedNUSMainWindow::~MedNUSMainWindow() {
    deleteWidgets();
    deleteMenus();
    delete login;
}

void MedNUSMainWindow::createWidgets()
{
    if(!_widgetsCreated) {
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
         fb = new MedNUSFrontBar(this);
         fb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
         fb->setName("Jay Chua");
         fb->setAvatar(":/images/ivle_profile.jpg");
         mainLayout->addWidget(fb,0,0);

        //Add UserBar (Top right bar that has user info)
         ub = new MedNUSUserBar(this);
         ub->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
         ub->setName("Jay Chua");
         ub->setAvatar(":/images/ivle_profile.jpg");
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
        lp->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        mainLayout->addWidget(lp,1,1);

        //Content Manager
        MedNUSContentManager *contentManager;
        contentManager = new MedNUSContentManager();
        connect(contentManager, SIGNAL(callAddLesson(QString,QString,QString,QStringList)), lp, SLOT(addLesson(QString,QString,QString,QStringList)));
        connect(lp, SIGNAL(emitOpenFile(QString,QString,int)), contentManager, SLOT(openFile(QString,QString,int)));
        connect(contentManager, SIGNAL(callAddTab(QWidget*,QString)), tabs, SLOT(addTab(QWidget*,QString)));
        contentManager->initLessonList(QStringList());
        //lessonTable->l

        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);

        _widgetsCreated = true;
    }
}

void MedNUSMainWindow::createMenus()
{
    if(!_menuCreated) {
        _menuCreated=true;
    }
}

void MedNUSMainWindow::deleteWidgets() {
    if(_widgetsCreated) {
        delete lp;
        delete fb;
        delete ub;
        delete tabs;
        delete vid;
        _widgetsCreated=false;
    }
}

void MedNUSMainWindow::deleteMenus() {
    if(_menuCreated) {
        _menuCreated=false;
    }
}

void MedNUSMainWindow::mousePressEvent ( QMouseEvent * event )
{
    if(event->buttons() == Qt::LeftButton) {
        if(_widgetsCreated) {
            if(lp->checkTrayButton(event->x(),event->y())) {
                _trayOut=!_trayOut;
            }

            lp->setTrayOut(_trayOut);
            ub->setTrayOut(_trayOut);
            fb->setTrayOut(_trayOut);
        }
    }
}

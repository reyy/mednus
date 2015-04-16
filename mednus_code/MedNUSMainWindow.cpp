#include "MedNUSMainWindow.h"
#include "MedNUSAUISettings.h"

#include <QDir>

#define SKIP_LOGIN 0

MedNUSMainWindow::MedNUSMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(QString("MedNUS"));
    setWindowIcon(QIcon(":/images/mednus-icon.png"));

    int winWidth = 800;
    int winHeight = 600;
    setMinimumSize(winWidth, winHeight);

    _trayOut=false;
    _widgetsCreated=false;
    _menuCreated=false;
    _currentMode = NONE;

    network = new MedNUSNetwork();

    if(!SKIP_LOGIN)
    {
        login = new MedNUSLogin(this);
        connect(network, SIGNAL(showLoadingScreen(bool)), login, SLOT(setLoading(bool)));
        network->tryAutoLogin();

        setCentralWidget(login);
        connect(login, SIGNAL(callLogin(QString,QString,bool)), network, SLOT(login(QString,QString,bool)));
        connect(network,SIGNAL(loginResults(bool,QString,QString,interfaceMode)),this,SLOT(loginCompleted(bool,QString,QString,interfaceMode)));
    }
    else
    {
        this->userName = "Temporary Testing Name";
        login = NULL;
        createWidgets();
        createMenus();
    }

    //Tab Bkg Change
    this->setStyleSheet("MedNUSMainWindow{border-image: url(:/images/login_background.png) 0 0 0 0 stretch stretch;}");

    _imageLine.load(":/images/login_lines.png");
    _imageBar.load(":/images/screentop.png");
}


MedNUSMainWindow::~MedNUSMainWindow() {
    deleteWidgets();
    deleteMenus();
    if(login!=NULL)
        delete login;
}


void MedNUSMainWindow::createWidgets()
{
    if(!_widgetsCreated) {
         if(_currentMode==LECTURER)
             this->setStyleSheet("MedNUSMainWindow{border-image: url(:/images/login_background2.png) 0 0 0 0 stretch stretch;}");
         centralWidget = new QWidget(this);
         mainLayout = new QGridLayout();
         mainLayout->setColumnStretch(0,1);
         mainLayout->setColumnStretch(1,0);
         mainLayout->setRowStretch(0,0);
         mainLayout->setRowStretch(0,1);
         mainLayout->setContentsMargins(0,0,0,0);
         mainLayout->setSpacing(0);
         mainLayout->setSizeConstraint(QLayout::SetNoConstraint);

        //Add FrontBar (Top bar that has logo)
         fb = new MedNUSFrontBar(this);
         fb->setMode(_currentMode);
         fb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
         mainLayout->addWidget(fb,0,0);

        //Add UserBar (Top right bar that has user info)
         ub = new MedNUSUserBar(this);
         ub->setMode(_currentMode);
         ub->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
         ub->setName(userName);
         ub->setAvatar(":/images/ivle_profile.png");
         mainLayout->addWidget(ub,0,1);

         //connect ub to logout
         connect(ub,SIGNAL(emitLogout()),this,SLOT(logout()));

        //Add Content View
        tabs = new MedNUSContentPanel();
        tabs->setMode(_currentMode);
        mainLayout->addWidget(tabs,1,0);

        // Create lesson table
        lp = new MedNUSLessonPanel(this);
        lp->setMode(_currentMode);
        lp->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        mainLayout->addWidget(lp,1,1);
        connect(tabs, SIGNAL(tabClosedSignal(QString)), lp, SLOT(tabClosed(QString)));
        connect(tabs, SIGNAL(tabOpenedSignal(QString, QWidget*)), lp, SLOT(tabOpened(QString, QWidget*)));

        //Content Manager
        MedNUSContentManager *contentManager = new MedNUSContentManager();

        //Connections
        connect(contentManager, SIGNAL(callAddLesson(QString,QString,QString,QStringList,QString)), lp, SLOT(addLesson(QString,QString,QString,QStringList,QString)));
        connect(lp, SIGNAL(emitOpenFile(QString,QString,int)), contentManager, SLOT(openFile(QString,QString,int)));
        connect(contentManager, SIGNAL(callAddTab(QWidget*,QString,QString)), tabs, SLOT(addTab(QWidget*,QString,QString)));
        connect(fb,SIGNAL(toggleLayout(int)),tabs,SLOT(toggleView(int)));
        connect(network, SIGNAL(receivedLessonList(QJsonDocument)), contentManager, SLOT(initLessonList(QJsonDocument)));
        connect(network, &MedNUSNetwork::receivedLessonList, [=](){
            //Hacky Fix!
            //TODO: Find out why cannot add lessons to UI properly
            qDebug()<<"HI";
            centralWidget->setLayout(mainLayout);
            setCentralWidget(centralWidget);

            _widgetsCreated = true;


            lp->setTrayOut(true);
            ub->setTrayOut(true);
            fb->setTrayOut(true);

        });

        //Fetch Lesson List
        network->downloadLessonList();
        //contentManager->initLessonList(NULL);
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
        //delete vid; //todo: fix crashing bug
        _widgetsCreated=false;
    }
}


void MedNUSMainWindow::deleteMenus() {
    if(_menuCreated) {
        _menuCreated=false;
    }
}


void MedNUSMainWindow::loginCompleted(bool success, QString matric, QString name, interfaceMode permission)
{
    if(!success) {
        qDebug() << "\nWrong User/Pass.";
        login->setLoading(false);
        login->setErrorMessage("Login Failed.\nPlease Try again.");
    }
    else
    {
        this->matricNo = matric;
        this->userName = name;

        //De-capitalize
        //Todo: push to function
        QRegExp re("\\W\\w");
        int pos=-1;
        name = name.toLower();
        QChar * base = name.data();
        QChar * ch;
        do{
         pos++;
         ch = base + pos;
         pos = name.indexOf(re, pos);
         *ch = ch->toUpper();
        }while(pos >= 0);
        this->userName = name;

        layout()->removeWidget(login);
        delete login;

        login = NULL;

        //Determined if its student or a staff.
        _currentMode=permission;
        _currentMode=STUDENT;
        qDebug() << "Set mode to  " << _currentMode;

        createMenus();
        createWidgets();
    }
}


void MedNUSMainWindow::logout()
{
    this->setStyleSheet("MedNUSMainWindow{border-image: url(:/images/login_background.png) 0 0 0 0 stretch stretch;}");
    network->logout();
    deleteWidgets();
    deleteMenus();
    login = new MedNUSLogin(this);
    setCentralWidget(login);
    connect(login, SIGNAL(callLogin(QString,QString,bool)), network, SLOT(login(QString,QString,bool)));
    //connect(network,SIGNAL(loginResults(bool,QString,QString)),this,SLOT(loginCompleted(bool,QString,QString)));
}


void MedNUSMainWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->buttons() == Qt::LeftButton) {
        if(_widgetsCreated) {
            _trayOut=lp->checkTray();
            lp->setTrayOut(_trayOut);
            ub->setTrayOut(_trayOut);
            fb->setTrayOut(_trayOut);
        }
    }
    event->ignore();
}


void MedNUSMainWindow::paintEvent(QPaintEvent *) {
    if(_widgetsCreated) {
        QPainter painter(this);
        painter.drawTiledPixmap(QRect(0,0,this->width(),this->height()),_imageLine);
        painter.drawTiledPixmap(QRect(0,0,this->width(),TOPBAR_HEIGHT),_imageBar);
    }
}

void MedNUSMainWindow::finalizeWidgets() {




}

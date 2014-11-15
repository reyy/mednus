#include "MedNUSMainWindow.h"
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

    network = new MedNUSNetwork();

    if(!SKIP_LOGIN)
    {
        login = new MedNUSLogin(this);
        connect(network, SIGNAL(showLoadingScreen(bool)), login, SLOT(setLoading(bool)));
        network->tryAutoLogin();

        setCentralWidget(login);
        connect(login, SIGNAL(callLogin(QString,QString,bool)), network, SLOT(login(QString,QString,bool)));
        connect(network,SIGNAL(loginResults(bool,QString,QString)),this,SLOT(loginCompleted(bool,QString,QString)));
    }
    else
    {
        this->userName = "Temporary Testing Name";
        login = NULL;
        createWidgets();
        createMenus();
    }

    //Tab Bkg Change
    this->setStyleSheet("MedNUSMainWindow{background:#152d3b;}");
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
         mainLayout->addWidget(fb,0,0);

        //Add UserBar (Top right bar that has user info)
         ub = new MedNUSUserBar(this);
         ub->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
         ub->setName(userName);
         ub->setAvatar(":/images/ivle_profile.jpg");
         mainLayout->addWidget(ub,0,1);

         //connect ub to logout
         connect(ub,SIGNAL(emitLogout()),this,SLOT(logout()));

        //Add Content View
        tabs = new MedNUSContentPanel();
        mainLayout->addWidget(tabs,1,0);

        // Create lesson table
        lp = new MedNUSLessonPanel(this);
        lp->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        mainLayout->addWidget(lp,1,1);
        connect(tabs, SIGNAL(tabClosedSignal(QString)), lp, SLOT(tabClosed(QString)));
        connect(tabs, SIGNAL(tabOpenedSignal(QString)), lp, SLOT(tabOpened(QString)));

        //Content Manager
        MedNUSContentManager *contentManager;
        contentManager = new MedNUSContentManager();
        connect(contentManager, SIGNAL(callAddLesson(QString,QString,QString,QStringList)), lp, SLOT(addLesson(QString,QString,QString,QStringList)));
        connect(lp, SIGNAL(emitOpenFile(QString,QString,int)), contentManager, SLOT(openFile(QString,QString,int)));
        connect(contentManager, SIGNAL(callAddTab(QWidget*,QString,QString)), tabs, SLOT(addTab(QWidget*,QString,QString)));
        contentManager->initLessonList(QStringList());


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
        //delete vid; //todo: fix crashing bug
        _widgetsCreated=false;
    }
}

void MedNUSMainWindow::deleteMenus() {
    if(_menuCreated) {
        _menuCreated=false;
    }
}

void MedNUSMainWindow::loginCompleted(bool success, QString matric, QString name)
{
    if(!success) {
        qDebug() << "\nWrong User/Pass.";
        login->setLoading(false);
        login->setErrorMessage("Wrong credentials.\nTry again.");
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

        createMenus();
        createWidgets();
    }
}

void MedNUSMainWindow::logout()
{
    network->logout();
    deleteWidgets();
    deleteMenus();
    login = new MedNUSLogin(this);
    setCentralWidget(login);
    connect(login, SIGNAL(callLogin(QString,QString,bool)), network, SLOT(login(QString,QString,bool)));
    connect(network,SIGNAL(loginResults(bool,QString,QString)),this,SLOT(loginCompleted(bool,QString,QString)));
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
    event->ignore();
}



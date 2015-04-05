#include "MedNUSContentPanel.h"

MedNUSContentPanel::MedNUSContentPanel(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(800-LESSONPANEL_WIDTH);
    this->setMinimumHeight(600-32);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _currentMode=NONE;

    layout = new QGridLayout();
    for(int i=0; i<3; i++)
        tabList[i] = new MedNUSTab(this);
    layout->setMargin(0);
    layout->setContentsMargins(10,10,10,10);
    layout->setSpacing(5);

    //Tab Bkg Change
    this->setStyleSheet("background-color:rgba(0,0,0,0);color:#ffffff");
    this->setLayout(layout);
}


void MedNUSContentPanel::setMode(interfaceMode mode) {
    _currentMode=mode;

    if (mode==STUDENT) {
        this->setMinimumWidth(800-LESSONPANEL_WIDTH);
    } else {
        this->setMinimumWidth(800-LESSONPANEL_WIDTH_L);
    }
}


void MedNUSContentPanel::addTab(QWidget* toAdd,QString title, QString dir)
{
    //Determine where to add by 'default view' rules
    int type;
    if(dynamic_cast<MedNUSVideoViewer*>(toAdd) != NULL)
        type = VIDEO_INDEX;
    else if(dynamic_cast<MedNUSPdfViewer*>(toAdd) != NULL)
        type = PDF_INDEX;
    else if(dynamic_cast<MedNUSMeshViewer*>(toAdd) != NULL || dynamic_cast<MedNUSQuiz*>(toAdd) != NULL)
        type = MESH_INDEX;
    else
        throw "Invalid Widget. Cannot add to Tab";

    //Do add tabs
    if(!layout->children().contains(tabList[type]))
    {
        //layout->addWidget(tabList[type],type%2,type==2?1:0,type==2?2:1,1);//1,0,1,1 //0,1,2,1
        int j = (rotateCount + type) % 3;
        layout->addWidget(tabList[type],j%2,j==2?1:0,j==2?2:1,1);
        tabList[type]->show();
    }

    //Check whether already open anot!
    for(int i=0 ; i<tabList[type]->count(); i++)
        if(tabList[type]->widget(i)->accessibleName().compare(dir) == 0)
        {
            delete toAdd;
            tabList[type]->setCurrentIndex(i);
            return;
        }

    //Check if tab opened properly
    if(toAdd->property("Loaded").isValid())
    {
        tabList[type]->addTab(toAdd, title);
        tabList[type]->setCurrentIndex(tabList[type]->count()-1);

        emit tabOpenedSignal(dir);
    }
    else
    {
        //Cleanup
        delete toAdd;
    }
}


void MedNUSContentPanel::toggleView(int view)
{
    //de-activate button if
    if(layout->count() < 3 - view || layout->count() == 1)
        return;

    //VERY HACKY! MUST REWRITE!
    int lastOpen = 0;
    for(int i=0; i<3; i++)
        if(tabList[i]->isVisible())
            lastOpen = i;

    tabList[0]->hide();
    tabList[1]->hide();
    tabList[2]->hide();

    if(viewType == view)
    {
        rotateCount = (++rotateCount >= 3) ? 0 : rotateCount;

        for(int i=0; i<3; i++)
        {

            if(tabList[i]->parent() != NULL)
            {
                int j = (rotateCount + i) % 3;
                layout->removeWidget(tabList[i]);
                layout->addWidget(tabList[i],j%2,j==2?1:0,j==2?2:1,1);
            }
        }
    }

    if(layout->count() == 3)
    {
        tabList[rotateCount]->show();

        if(view == 0)
            tabList[(rotateCount + 1) % 3]->show();

        if(view <= 1)
            tabList[(rotateCount + 2) % 3]->show();
    }
    else //count = 2
    {
        for(int i=0 ;i<3; i++)
            if(tabList[(rotateCount + i) % 3]->parent() != NULL)
            {
                if(view!=2 || (view == 2 && (rotateCount + i) % 3 != lastOpen) )
                {
                    tabList[(rotateCount + i) % 3]->show();
                    if(view == 2)
                        break;
                }
            }
    }

    viewType = view;
}


void MedNUSContentPanel::closeTab(MedNUSTab* index)
{
    for(int i=0; i<3; i++)
        if(tabList[i] == index)
        {
            layout->removeWidget(index);
            delete tabList[i];
            tabList[i] = new MedNUSTab(this);

        }
}


MedNUSContentPanel::~MedNUSContentPanel()
{
    delete layout;
}


MedNUSTab::MedNUSTab(QWidget *parent)
{
    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    this->setTabsClosable(true);
    this->setMovable(true);
    //this->setDocumentMode(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumWidth(100);
    this->setMinimumHeight(100);
    this->setContentsMargins(0,0,0,0);
    //this->setAutoFillBackground(true);
    this->tabBar()->setUsesScrollButtons(true);
    this->tabBar()->setElideMode(Qt::ElideLeft);
    this->tabBar()->setAutoFillBackground(true);

    QString tempDirectory = ":/images/tabwidget.css";
    /*if(_currentMode != STUDENT) {
        tempDirectory = ":/images/tabwidget2.css";
    }*/
    //To do, make MedNUSTab recognise current mode.

    QFile file(tempDirectory);

    if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        this->setStyleSheet(file.readAll());
        file.close();
    }

    connect(this,SIGNAL(noMoreTabs(MedNUSTab*)),parent,SLOT(closeTab(MedNUSTab*)));
    connect(this,SIGNAL(tabClosedSignal(QString)),parent,SLOT(tabClosed(QString)));
}


void MedNUSTab::closeTab(int index)
{
    emit tabClosedSignal(this->widget(index)->accessibleName());

    QWidget *temp = this->widget(index);
    this->removeTab(index);
    delete temp;

    if(this->count()==0)
        emit noMoreTabs(this);
}

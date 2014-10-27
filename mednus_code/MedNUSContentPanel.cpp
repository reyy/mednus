#include "MedNUSContentPanel.h"

MedNUSContentPanel::MedNUSContentPanel(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(800-300);
    this->setMinimumHeight(600-32);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout = new QGridLayout();
    for(int i=0; i<3; i++)
        tabList[i] = new MedNUSTab(this);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(1);

    this->setAutoFillBackground(true);
    this->setStyleSheet("background-color: #152d3b;");
    this->setLayout(layout);

}

void MedNUSContentPanel::addTab(QWidget* toAdd,QString title)
{
    //Add by 'default view' rules
    if(dynamic_cast<MedNUSVideoViewer*>(toAdd) != NULL)
    {
        if(!layout->children().contains(tabList[VIDEO_INDEX]))
            layout->addWidget(tabList[VIDEO_INDEX],0,0,1,1);
        //Todo: Check whether already open anot!
        tabList[VIDEO_INDEX]->addTab(toAdd, title);
        tabList[VIDEO_INDEX]->setCurrentIndex(tabList[VIDEO_INDEX]->count()-1);
    }
    else if(dynamic_cast<MedNUSPdfViewer*>(toAdd) != NULL)
    {
        if(!layout->children().contains(tabList[PDF_INDEX]))
            layout->addWidget(tabList[PDF_INDEX],1,0,1,1);
        tabList[PDF_INDEX]->addTab(toAdd, title);
        tabList[PDF_INDEX]->setCurrentIndex(tabList[PDF_INDEX]->count()-1);
    }
    else if(dynamic_cast<MedNUSMeshViewer*>(toAdd) != NULL || dynamic_cast<MedNUSQuiz*>(toAdd) != NULL)
    {
        if(!layout->children().contains(tabList[MESH_INDEX]))
            layout->addWidget(tabList[MESH_INDEX],0,1,2,1);
        tabList[MESH_INDEX]->addTab(toAdd, title);
        tabList[MESH_INDEX]->setCurrentIndex(tabList[MESH_INDEX]->count()-1);
    }
    else
    {
        throw "Invalid Widget. Cannot add to Tab";
    }
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
}




MedNUSTab::MedNUSTab(QWidget *parent)
{
    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    this->setTabsClosable(true);
    this->setMovable(true);
    this->setDocumentMode(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumWidth(100);
    this->setMinimumHeight(100);
    this->setContentsMargins(0,0,0,0);

    connect(this,SIGNAL(noMoreTabs(MedNUSTab*)),parent,SLOT(closeTab(MedNUSTab*)));
}

void MedNUSTab::closeTab(int index)
{
    delete this->widget(index);
    this->removeTab(index);
    if(this->count()==0)
        emit noMoreTabs(this);
}

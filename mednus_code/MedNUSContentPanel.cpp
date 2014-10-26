#include "MedNUSContentPanel.h"

MedNUSContentPanel::MedNUSContentPanel(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(800-300);
    this->setMinimumHeight(600-32);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout = new QGridLayout();
    for(int i=0; i<3; i++)
    {
        tabList[i].setTabsClosable(true);
        tabList[i].setMovable(true);
        tabList[i].setDocumentMode(true);
        tabList[i].setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        tabList[i].setMinimumWidth(100);
        tabList[i].setMinimumHeight(100);
        tabList[i].setContentsMargins(0,0,0,0);

        connect(&tabList[i],SIGNAL(noMoreTabs(MedNUSTab*)),this,SLOT(closeTab(MedNUSTab*)));
    }

//    layout->addWidget(&tabList[0],0,0,1,1);
//    layout->addWidget(&tabList[1],1,0,1,1);
//    layout->addWidget(&tabList[2],0,1,2,1);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(1);

    this->setAutoFillBackground(true);
    this->setStyleSheet("background-color: #193b50;");
    this->setLayout(layout);

}

void MedNUSContentPanel::addTab(QWidget* toAdd,QString title)
{
    //Add by 'default view' rules
    if(dynamic_cast<MedNUSVideoViewer*>(toAdd) != 0)
    {
        if(!layout->children().contains(&tabList[VIDEO_INDEX]))
            layout->addWidget(&tabList[VIDEO_INDEX],0,0,1,1);
        tabList[VIDEO_INDEX].addTab(toAdd, title);
    }
    else if(dynamic_cast<MedNUSPdfViewer*>(toAdd) != 0)
    {
        if(!layout->children().contains(&tabList[PDF_INDEX]))
            layout->addWidget(&tabList[PDF_INDEX],1,0,1,1);
        tabList[PDF_INDEX].addTab(toAdd, title);
    }
    else if(dynamic_cast<MedNUSMeshViewer*>(toAdd) != 0)
    {
        if(!layout->children().contains(&tabList[MESH_INDEX]))
            layout->addWidget(&tabList[MESH_INDEX],0,1,2,1);
        tabList[MESH_INDEX].addTab(toAdd, title);
    }
}

void MedNUSContentPanel::closeTab(MedNUSTab* index)
{
    layout->removeWidget(index);
};

MedNUSContentPanel::~MedNUSContentPanel()
{
}



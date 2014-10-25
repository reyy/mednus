#include "MedNUSContentPanel.h"
#include <QStyle>
#include <QLabel>

MedNUSContentPanel::MedNUSContentPanel(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(800-300);
    this->setMinimumHeight(600-32);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //this->setMaximumHeight(600-32);

//    this->setTabsClosable(true);
//    this->setMovable(true);
//    this->setDocumentMode(true);
    //this->setUsesScrollButtons(true);
    //this->setElideMode(Qt::ElideRight);
    //this->setTabShape(QTabWidget::Rounded);
    layout = new QGridLayout();
    for(int i=0; i<4; i++)
    {
        tabList[i].setTabsClosable(true);
        tabList[i].setMovable(true);
        tabList[i].setDocumentMode(true);
        tabList[i].setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        tabList[i].setMinimumWidth(100);
        tabList[i].setMinimumHeight(100);
        tabList[i].setContentsMargins(0,0,0,0);
    }
    //layout->setRowStretch(0,10);
    //layout->setRowStretch(1,5);
    layout->addWidget(&tabList[0],0,0,1,1);
    layout->addWidget(&tabList[1],1,0,1,1);
    layout->addWidget(&tabList[2],0,1,2,1);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(1);

    this->setAutoFillBackground(true);
    this->setStyleSheet("background-color: #193b50;");
    this->setLayout(layout);
}

void MedNUSContentPanel::addTab(QWidget* toAdd,QString title)
{
    tabList[counter--].addTab(toAdd,title);
}

MedNUSContentPanel::~MedNUSContentPanel()
{
}



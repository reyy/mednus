#include "MedNUSContentPanel.h"

MedNUSContentPanel::MedNUSContentPanel(QWidget *parent) :
    QTabWidget(parent)
{
    this->setMinimumWidth(800-300);
    this->setMinimumHeight(600-32);
    this->setMaximumHeight(600-32);

    this->setTabsClosable(true);
    this->setMovable(true);
    this->setDocumentMode(true);
    this->setUsesScrollButtons(true);
    this->setElideMode(Qt::ElideRight);
    this->setTabShape(QTabWidget::Rounded);
}

MedNUSContentPanel::~MedNUSContentPanel()
{
}

void MedNUSContentPanel::resizeEvent(QResizeEvent* event)
{
   //qDebug() << "Resize";
}


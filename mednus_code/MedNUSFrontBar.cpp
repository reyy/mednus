#include "MedNUSFrontBar.h"

MedNUSFrontBar::MedNUSFrontBar(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(800-300);
    this->setMinimumHeight(64);

    _background = new QLabel(parent);
    _background->setGeometry(QRect(0, 0, 500, 64));
    _background->setStyleSheet("background-color: #152d3b;");
}


MedNUSFrontBar::~MedNUSFrontBar()
{
    delete _background;
}

void MedNUSFrontBar::resizeEvent(QResizeEvent* event)
{
   //qDebug() << "Resize";
    _background->setGeometry(QRect(0, 0, this->geometry().width(), 64));
}

#include "MedNUSFrontBar.h"

MedNUSFrontBar::MedNUSFrontBar(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(800-300);
    this->setMinimumHeight(32);
    this->setMaximumHeight(32);

    _background = new QLabel(parent);
    _background->setGeometry(QRect(0, 0, 500, 32));
    _background->setStyleSheet("background-color: #152d3b;");

    _logo = new QLabel(parent);
    _logo->setGeometry(QRect(5, 7, 76, 21));
    _logo->setPixmap(QPixmap(QString::fromStdString(":/images/logo_inapp.png")));
}

MedNUSFrontBar::~MedNUSFrontBar()
{
    delete _background;
    delete _logo;
}

void MedNUSFrontBar::resizeEvent(QResizeEvent* event)
{
   //qDebug() << "Resize";
    _background->setGeometry(QRect(0, 0, this->geometry().width(), this->geometry().height()));
}

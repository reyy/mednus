#include "MedNUSUserBar.h"

MedNUSUserBar::MedNUSUserBar(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(300);
    this->setMinimumHeight(32);
    this->setMaximumHeight(32);

    _background = new QLabel(this);
    _background->setGeometry(QRect(this->x(), this->y(), 300, 32));
    _background->setStyleSheet("background-color: #1c4f6e;");
}

MedNUSUserBar::~MedNUSUserBar()
{
    delete _background;
}

void MedNUSUserBar::resizeEvent(QResizeEvent* event)
{
}

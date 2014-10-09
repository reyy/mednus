#include "MedNUSLessonPackage.h"
#include <QFontMetrics>

MedNUSLessonPackage::MedNUSLessonPackage(QWidget *parent) :
    QWidget(parent)
{
    this->setBaseSize(300,this->height());
    this->setMinimumWidth(300);
    this->setStyleSheet("background-color: #1c4f6e;");

    QString _ss = QString("background-color: #1c4f6e;");

    _background = new QLabel(parent);
    _background->setGeometry(QRect(this->x(), this->y(), 300, 64));
    _background->setStyleSheet(_ss+"background-color: #1c4f6e;");
    //_background->setPixmap(QPixmap(QString::fromStdString(":/images/copy.png")));

    _moduleTitle = new QLabel(parent);
    _moduleTitle->setGeometry(QRect(this->x()+10, this->y(), 280, 25));
    _moduleTitle->setStyleSheet(_ss+"color:#FFF;font-size:12px;font-weight:bold;");
    QFontMetrics metrics1(_moduleTitle->font());
    _moduleTitle->setText(metrics1.elidedText("LSM2203 - Experimental Microbiology", Qt::ElideRight, _moduleTitle->width()));

    _subHeader = new QLabel(parent);
    _subHeader->setGeometry(QRect(this->x()+10, this->y()+22, 280, 20));
    _subHeader->setStyleSheet(_ss+"color:#FFF;font-size:10px;");
    QFontMetrics metrics2(_subHeader->font());
    _subHeader->setText(metrics2.elidedText("Prof Mary Ng & A/Prof Lee Yuan Kun", Qt::ElideRight, _subHeader->width()));

    _description = new QLabel(parent);
    _description->setGeometry(QRect(this->x()+10, this->y()+40, 280, 20));
    _description->setStyleSheet(_ss+"color:#FFF;font-size:10px;");
    QFontMetrics metrics3(_description->font());
    _description->setText(metrics3.elidedText("Prerequisite: LSM1101 or LSM1102 or LSM1103", Qt::ElideRight, _description->width()));

}

MedNUSLessonPackage::~MedNUSLessonPackage()
{
    delete _background;
}

void MedNUSLessonPackage::mousePressEvent ( QMouseEvent * event )
{
    qDebug() << "Debug Message";
}


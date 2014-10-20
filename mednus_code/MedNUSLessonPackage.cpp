#include "MedNUSLessonPackage.h"
#include <QFontMetrics>
#include <math.h>

MedNUSLessonPackage::MedNUSLessonPackage(QWidget *parent) :
    QWidget(parent) {
    _collapse =false;
    _height = 64;
    _collapsedHeight = 24;
    _tone=1;
    _y=0;

    this->setBaseSize(300,_height);
    this->setMinimumWidth(300);
    this->setStyleSheet("background-color: #1c4f6e;");

    _background = new QLabel(parent);
    _background->setGeometry(QRect(0, _y, 300, _height));
    //_background->setPixmap(QPixmap(QString::fromStdString(":/images/copy.png")));

    _moduleTitle = new QLabel(parent);
    _moduleTitle->setGeometry(QRect(0+10, _y, 280, 25));
    _moduleTitle->setStyleSheet("color:#FFF;font-size:12px;font-weight:bold;");

    _subHeader = new QLabel(parent);
    _subHeader->setGeometry(QRect(0+10, _y+22, 280, 20));
    _subHeader->setStyleSheet("color:#FFF;font-size:10px;");

    _description = new QLabel(parent);
    _description->setGeometry(QRect(0+10, _y+40, 280, 20));
    _description->setStyleSheet("color:#FFF;font-size:10px;");
}

MedNUSLessonPackage::~MedNUSLessonPackage() {
    delete _background;
}

void MedNUSLessonPackage::setY(int value) {
    _y=value;
}

int MedNUSLessonPackage::getY() {
    return _y;
}

void MedNUSLessonPackage::setTitle(QString value) {
    _title = value;
    QFontMetrics metrics1(_moduleTitle->font());
    _moduleTitle->setText(metrics1.elidedText(value, Qt::ElideRight, _moduleTitle->width()));
}

QString MedNUSLessonPackage::getTitle() {
    return _title;
}

void MedNUSLessonPackage::setSubHeader(QString value) {
    QFontMetrics metrics2(_subHeader->font());
    _subHeader->setText(metrics2.elidedText(value, Qt::ElideRight, _subHeader->width()));
}

void MedNUSLessonPackage::setDescription(QString value) {

    QFontMetrics metrics3(_description->font());
    _description->setText(metrics3.elidedText(value, Qt::ElideRight, _description->width()));
}

void MedNUSLessonPackage::setTone(int value) {
    _tone=std::min(std::max(value,0),16);
}

int MedNUSLessonPackage::getHeight(){
    if(_collapse)
        return _collapsedHeight;
    else
        return _height;
}

void MedNUSLessonPackage::toggleCollapse() {
    _collapse=!_collapse;
}

void MedNUSLessonPackage::toggleCollapse(bool value) {
    _collapse=value;
}

void MedNUSLessonPackage::updateGUI() {
    if(_tone%2==0)
        _background->setStyleSheet("background-color: rgb("+QString::number(10+_tone)+","+QString::number(85-_tone*2)+","+QString::number(127-3.6*_tone)+");");
    else
        _background->setStyleSheet("background-color: rgb("+QString::number(10+_tone-10)+","+QString::number(85-_tone*2-10)+","+QString::number(127-3.6*_tone-10)+");");

    if(_collapse) {
        _moduleTitle->setStyleSheet("color:#FFF;font-size:12px;font-weight:bold;");
        _subHeader->setStyleSheet("color:#FFF;font-size:10px;");
        _description->setStyleSheet("color:#FFF;font-size:10px;");
        _background->setGeometry(QRect(0, _y, 300, 24));
        _moduleTitle->setGeometry(QRect(0+10, _y, 280, 24));
        _subHeader->setGeometry(QRect(0+10, _y+22, 280, 20));
        _description->setGeometry(QRect(0+10, _y+40, 280, 20));

        _subHeader->setVisible(false);
        _description->setVisible(false);
    } else {
        _moduleTitle->setStyleSheet("color:#FFF;font-size:12px;font-weight:bold;");
        _subHeader->setStyleSheet("color:#FFF;font-size:10px;");
        _description->setStyleSheet("color:#FFF;font-size:10px;");
        _background->setGeometry(QRect(0, _y, 300, 64));
        _moduleTitle->setGeometry(QRect(0+10, _y, 280, 24));
        _subHeader->setGeometry(QRect(0+10, _y+22, 280, 20));
        _description->setGeometry(QRect(0+10, _y+40, 280, 20));

        _subHeader->setVisible(true);
        _description->setVisible(true);
    }
}


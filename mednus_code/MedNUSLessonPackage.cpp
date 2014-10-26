#include "MedNUSLessonPackage.h"
#include "MedNUSAUISettings.h"
#include <QFontMetrics>
#include <math.h>

MedNUSLessonPackage::MedNUSLessonPackage(QWidget *parent) :
    QWidget(parent) {
    _collapse = true;
    _height = 164;
    _interactiveHeight = 64;
    _collapsedHeight = 24;
    _border = 5;
    _tone=1;
    _x=SIDEBAR_OFFSET;
    _y=0;
    _parent = parent;

    this->setBaseSize(LESSONPANEL_WIDTH,_height);
    this->setMinimumWidth(LESSONPANEL_WIDTH);
    this->setStyleSheet("background-color: #1c4f6e;");


    _background = new QLabel(parent);
    _background->setGeometry(QRect(_x, _y, LESSONPANEL_WIDTH, _interactiveHeight));
    //_background->setPixmap(QPixmap(QString::fromStdString(":/images/copy.png")));

    _contentBackground = new QLabel(parent);
    _contentBackground->setGeometry(QRect(_x+TOPBAR_HEIGHT, _y, LESSONPANEL_WIDTH, _height));
    _contentBackground->setStyleSheet("background-color: #2d3949;");

    _moduleTitle = new QLabel(parent);
    _moduleTitle->setGeometry(QRect(_x+TOPBAR_HEIGHT, _y, LESSONPANEL_WIDTH, 25));
    _moduleTitle->setStyleSheet("color:#FFF;font-size:12px;font-weight:bold;");

    _subHeader = new QLabel(parent);
    _subHeader->setGeometry(QRect(_x+TOPBAR_HEIGHT, _y+22, LESSONPANEL_WIDTH, 20));
    _subHeader->setStyleSheet("color:#FFF;font-size:10px;");

    _description = new QLabel(parent);
    _description->setGeometry(QRect(_x+TOPBAR_HEIGHT, _y+40, LESSONPANEL_WIDTH, 20));
    _description->setStyleSheet("color:#FFF;font-size:10px;");
}

MedNUSLessonPackage::~MedNUSLessonPackage() {
    delete _background;
    delete _contentBackground;
    delete _moduleTitle;
    delete _subHeader;
    delete _description;
}

void MedNUSLessonPackage::setY(int value) {
    _y=value;
}

int MedNUSLessonPackage::getY() {
    return _y;
}

MedNUSLessonIcon* MedNUSLessonPackage::getContentItem(int value) {
    return _listOfItems.at(value);
}

int MedNUSLessonPackage::getContentSize() {
    return _listOfItems.size();
}

void MedNUSLessonPackage::addContent(QString filename, QPixmap directory) {
    MedNUSLessonIcon *item = new MedNUSLessonIcon(filename,directory,_parent);
    item->updatePosition(_border,_y+_interactiveHeight+5+_listOfItems.size()*24);
    _listOfItems.push_back(item);

    connect(item, SIGNAL(emitOpenFile(QString,QString,int)), this, SLOT(callOpenFile(QString,QString,int)));
}

void MedNUSLessonPackage::clearContent() {
    while(_listOfItems.size()>0) {
        delete _listOfItems.front();
        _listOfItems.pop_front();
    }
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

bool MedNUSLessonPackage::getCollapse() {
    return _collapse;
}

int MedNUSLessonPackage::getHeight(){
    if(_collapse)
        return _collapsedHeight;
    else
        return _height;
}

int MedNUSLessonPackage::getInteractiveHeight(){
    if(_collapse)
        return _collapsedHeight;
    else
        return _interactiveHeight;
}

void MedNUSLessonPackage::toggleCollapse() {
    _collapse=!_collapse;
}

void MedNUSLessonPackage::toggleCollapse(bool value) {
    _collapse=value;
}

void MedNUSLessonPackage::updateGUI() {
    if(_tone%2==0)
        _background->setStyleSheet("background-color: #286c91;");
    else
        _background->setStyleSheet("background-color: #245570;");

    if(_collapse) {
        _moduleTitle->setStyleSheet("color:#FFF;font-size:12px;font-weight:bold;");
        _subHeader->setStyleSheet("color:#FFF;font-size:10px;");
        _description->setStyleSheet("color:#FFF;font-size:10px;");
        _background->setGeometry(QRect(_x, _y, LESSONPANEL_WIDTH, _collapsedHeight));
        _moduleTitle->setGeometry(QRect(_x+10, _y, LESSONPANEL_WIDTH, 24));
        _subHeader->setGeometry(QRect(_x+15, _y+22, LESSONPANEL_WIDTH, 20));
        _description->setGeometry(QRect(_x+15, _y+40, LESSONPANEL_WIDTH, 20));

        _contentBackground->setVisible(false);
        _subHeader->setVisible(false);
        _description->setVisible(false);

        for(int i=0;i<_listOfItems.size();i++)
            _listOfItems.at(i)->setVisible(false);
    } else {
        _moduleTitle->setStyleSheet("color:#FFF;font-size:12px;font-weight:bold;");
        _subHeader->setStyleSheet("color:#FFF;font-size:10px;");
        _description->setStyleSheet("color:#FFF;font-size:10px;");
        _background->setGeometry(QRect(_x, _y, LESSONPANEL_WIDTH, _height));
        _contentBackground->setGeometry(QRect(_x+_border, _y+_interactiveHeight, LESSONPANEL_WIDTH-_border*2, _height-_interactiveHeight-_border));
        _moduleTitle->setGeometry(QRect(_x+10, _y, LESSONPANEL_WIDTH, 24));
        _subHeader->setGeometry(QRect(_x+15, _y+22, LESSONPANEL_WIDTH, 20));
        _description->setGeometry(QRect(_x+15, _y+40, LESSONPANEL_WIDTH, 20));

        _contentBackground->setVisible(true);
        _subHeader->setVisible(true);
        _description->setVisible(true);

        for(int i=0;i<_listOfItems.size();i++) {
            _listOfItems.at(i)->setVisible(true);
            _listOfItems.at(i)->updatePosition(_border,_y+_interactiveHeight+5+i*24);
        }
    }
}


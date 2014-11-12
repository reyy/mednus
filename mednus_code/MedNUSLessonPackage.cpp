#include "MedNUSLessonPackage.h"
#include "MedNUSAUISettings.h"
#include <QFontMetrics>
#include <math.h>

MedNUSLessonPackageContentPanel::MedNUSLessonPackageContentPanel(int x,int y,QWidget *parent) :
    QWidget(parent) {

    this->setGeometry(QRect(0,0,LESSONPANEL_WIDTH-LESSONPANEL_BORDER*2,_listOfItems.size()*24));

    _parent=this;
}

MedNUSLessonPackageContentPanel::~MedNUSLessonPackageContentPanel(){
}

MedNUSLessonIcon* MedNUSLessonPackageContentPanel::getContentItem(int value) {
    return _listOfItems.at(value);
}

int MedNUSLessonPackageContentPanel::getContentSize() {
    return _listOfItems.size();
}

MedNUSLessonIcon* MedNUSLessonPackageContentPanel::addContent(QString filename, QPixmap directory) {
    MedNUSLessonIcon *item = new MedNUSLessonIcon(filename,directory,_parent);
    _listOfItems.push_back(item);
    return item;
}

void MedNUSLessonPackageContentPanel::clearContent() {
    while(_listOfItems.size()>0) {
        delete _listOfItems.front();
        _listOfItems.pop_front();
    }
}

void MedNUSLessonPackageContentPanel::updateGUI(int x, int y, bool collapse) {
    this->setGeometry(QRect(0,0,LESSONPANEL_WIDTH,_listOfItems.size()*24));

    if(collapse) {
        for(int i=0;i<_listOfItems.size();i++)
            _listOfItems.at(i)->setVisible(false);
    } else {
        for(int i=0;i<_listOfItems.size();i++) {
            _listOfItems.at(i)->setVisible(true);
            _listOfItems.at(i)->updatePosition(x,y,0,5+i*24);
        }
    }
}

MedNUSLessonPackage::MedNUSLessonPackage(QWidget *parent) :
    QWidget(parent) {
    _collapse = true;
    _tone=1;
    _x=SIDEBAR_OFFSET;
    _y=0;
    _parent = parent;

    this->setBaseSize(LESSONPANEL_WIDTH,LESSONPANEL_HEIGHT);
    this->setMinimumWidth(LESSONPANEL_WIDTH);
    this->setStyleSheet("background-color: #1c4f6e;");


    _background = new QLabel(parent);
    _background->setGeometry(QRect(_x, _y, LESSONPANEL_WIDTH, LESSONPANEL_CLICKHEIGHT));
    //_background->setPixmap(QPixmap(QString::fromStdString(":/images/copy.png")));

    _moduleTitle = new QLabel(parent);
    _moduleTitle->setGeometry(QRect(_x+TOPBAR_HEIGHT, _y, LESSONPANEL_WIDTH, 25));
    _moduleTitle->setStyleSheet("color:#FFF;font-size:12px;font-weight:bold;");

    _subHeader = new QLabel(parent);
    _subHeader->setGeometry(QRect(_x+TOPBAR_HEIGHT, _y+22, LESSONPANEL_WIDTH, 20));
    _subHeader->setStyleSheet("color:#FFF;font-size:10px;");

    _description = new QLabel(parent);
    _description->setGeometry(QRect(_x+TOPBAR_HEIGHT, _y+40, LESSONPANEL_WIDTH, 20));
    _description->setStyleSheet("color:#FFF;font-size:10px;");

    _contentPanel = new MedNUSLessonPackageContentPanel(_x,_y,parent);

    _scrollArea = new QScrollArea(parent);
    _scrollArea->setWidgetResizable(false);
    _scrollArea->setContentsMargins(0,0,0,0);
    _scrollArea->setGeometry(_contentPanel->geometry());
    _scrollArea->setAutoFillBackground(true);
    _scrollArea->setWidget(_contentPanel);
    _scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded);

    //Load scrollbar style.
    QFile file(":/images/scrollbar.css");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        _scrollArea->setStyleSheet(file.readAll());
        file.close();
    }
}

MedNUSLessonPackage::~MedNUSLessonPackage() {
    delete _background;
    delete _moduleTitle;
    delete _subHeader;
    delete _description;
    delete _contentPanel;
    delete _scrollArea;
}

void MedNUSLessonPackage::setY(int value) {
    _y=value;
}

int MedNUSLessonPackage::getY() {
    return _y;
}

void MedNUSLessonPackage::addContent(QString filename, QPixmap directory) {
    MedNUSLessonIcon *item = _contentPanel->addContent(filename,directory);
    connect(this->parent(), SIGNAL(tabClosedSignal(QString)), item, SLOT(tabClosed(QString)));
    connect(this->parent(), SIGNAL(tabOpenedSignal(QString)), item, SLOT(tabOpened(QString)));
    connect(item, SIGNAL(emitOpenFile(QString,QString,int)), this, SLOT(callOpenFile(QString,QString,int)));
}

void MedNUSLessonPackage::clearContent() {
    _contentPanel->clearContent();
}

int MedNUSLessonPackage::getContentSize() {
    return _contentPanel->getContentSize();
}

MedNUSLessonIcon* MedNUSLessonPackage::getContentItem(int value) {
    return _contentPanel->getContentItem(value);
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
    _tone=value;
}

bool MedNUSLessonPackage::getCollapse() {
    return _collapse;
}

int MedNUSLessonPackage::getHeight(){
    if(_collapse)
        return LESSONPANEL_CONTRACTED_CLICKHEIGHT;
    else
        return LESSONPANEL_HEIGHT;
}

int MedNUSLessonPackage::getInteractiveHeight(){
    if(_collapse)
        return LESSONPANEL_CONTRACTED_CLICKHEIGHT;
    else
        return LESSONPANEL_CLICKHEIGHT;
}

void MedNUSLessonPackage::toggleCollapse() {
    _collapse=!_collapse;

    _contentPanel->updateGUI(_x,_y,_collapse);
}

void MedNUSLessonPackage::toggleCollapse(bool value) {
    _collapse=value;

    _contentPanel->updateGUI(_x,_y,_collapse);
}

void MedNUSLessonPackage::updateGUI() {
    //_contentPanel->updateGUI(_x,_y,_collapse);

    if(_tone%2==0)
        _background->setStyleSheet("background-color: #286c91;");
    else
        _background->setStyleSheet("background-color: #245570;");

    if(_collapse) {
        _moduleTitle->setStyleSheet("color:#FFF;font-size:12px;font-weight:bold;");
        _subHeader->setStyleSheet("color:#FFF;font-size:10px;");
        _description->setStyleSheet("color:#FFF;font-size:10px;");
        _background->setGeometry(QRect(_x, _y, LESSONPANEL_WIDTH, LESSONPANEL_CONTRACTED_CLICKHEIGHT));
        _moduleTitle->setGeometry(QRect(_x+10, _y, LESSONPANEL_WIDTH, 24));
        _subHeader->setGeometry(QRect(_x+15, _y+22, LESSONPANEL_WIDTH, 20));
        _description->setGeometry(QRect(_x+15, _y+40, LESSONPANEL_WIDTH, 20));

        _scrollArea->setVisible(false);
        _subHeader->setVisible(false);
        _description->setVisible(false);
    } else {
        _moduleTitle->setStyleSheet("color:#FFF;font-size:12px;font-weight:bold;");
        _subHeader->setStyleSheet("color:#FFF;font-size:10px;");
        _description->setStyleSheet("color:#FFF;font-size:10px;");
        _background->setGeometry(QRect(_x, _y, LESSONPANEL_WIDTH, LESSONPANEL_HEIGHT));
        _moduleTitle->setGeometry(QRect(_x+10, _y, LESSONPANEL_WIDTH, 24));
        _subHeader->setGeometry(QRect(_x+15, _y+22, LESSONPANEL_WIDTH, 20));
        _description->setGeometry(QRect(_x+15, _y+40, LESSONPANEL_WIDTH, 20));
        _scrollArea->setGeometry(QRect(_x+LESSONPANEL_BORDER, _y+LESSONPANEL_CLICKHEIGHT, LESSONPANEL_WIDTH-LESSONPANEL_BORDER*4, LESSONPANEL_HEIGHT-LESSONPANEL_CLICKHEIGHT-LESSONPANEL_BORDER));

        _scrollArea->setVisible(true);
        _subHeader->setVisible(true);
        _description->setVisible(true);
    }
}


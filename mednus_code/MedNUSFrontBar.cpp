#include "MedNUSFrontBar.h"

MedNUSFrontBar::MedNUSFrontBar(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumHeight(TOPBAR_HEIGHT);
    this->setMaximumHeight(TOPBAR_HEIGHT);
    this->setMinimumWidth(800-LESSONPANEL_WIDTH);

    _currentMode=NONE;

    _logo = new QLabel(parent);
    int space = (TOPBAR_HEIGHT-TOPBAR_HEIGHT/40*21)/2;
    _logo->setGeometry(QRect(this->x()+space, this->y()+space, TOPBAR_HEIGHT/40*76, TOPBAR_HEIGHT/40*21));
    _logo->setPixmap(QPixmap(QString::fromStdString(":/images/logo.png")));
    _logo->setStyleSheet("background-color:rgba(0,0,0,0);");
    _logo->setScaledContents(true);
    _logo->setVisible(true);

    _btView1 = new QPushButton(parent);
    _btView1->setIconSize(QSize(24,24));
    _btView1->setFlat(true);
    _btView1->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_view1.png);} QPushButton::pressed {background-image: url(:/images/bt_view1_p.png);}");
    _btView1->setGeometry(QRect(this->x()+this->geometry().width()-36*1, this->y()+space, 32,24));
    _btView1->setVisible(false);

    _btView2 = new QPushButton(parent);
    _btView2->setIconSize(QSize(24,24));
    _btView2->setFlat(true);
    _btView2->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_view2.png);} QPushButton::pressed {background-image: url(:/images/bt_view2_p.png);}");
    _btView2->setGeometry(QRect(this->x()+this->geometry().width()-36*2, this->y()+space, 32,24));
    _btView2->setVisible(false);

    _btView3 = new QPushButton(parent);
    _btView3->setIconSize(QSize(24,24));
    _btView3->setFlat(true);
    _btView3->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_view3.png);} QPushButton::pressed {background-image: url(:/images/bt_view3_p.png);}");
    _btView3->setGeometry(QRect(this->x()+this->geometry().width()-36*3, this->y()+space, 32,24));
    _btView3->setVisible(false);

    connect(_btView1,SIGNAL(clicked()),this,SLOT(changeFullScreenView()));
    connect(_btView2,SIGNAL(clicked()),this,SLOT(changeLessonView()));
    connect(_btView3,SIGNAL(clicked()),this,SLOT(changeDefaultView()));

    _trayOut = true;
}


MedNUSFrontBar::~MedNUSFrontBar()
{
    _btView1->setVisible(false);
    _btView2->setVisible(false);
    _btView3->setVisible(false);
    _logo->setVisible(false);
}


void MedNUSFrontBar::setTrayOut(bool value) {
    _trayOut = value;

    if (_currentMode==STUDENT) {
        this->setMinimumWidth(800-LESSONPANEL_WIDTH);
    } else {
        this->setMinimumWidth(800-LESSONPANEL_WIDTH_L);
    }
}


void MedNUSFrontBar::setMode(interfaceMode mode) {
    _currentMode=mode;
}


void MedNUSFrontBar::changeFullScreenView() {
    //Todo: Set Screen to fullscreen.
    emit toggleLayout(2);
}


void MedNUSFrontBar::changeLessonView() {
    //Todo: Set Screen to lessonview. Video and pdf.
    emit toggleLayout(1);
}


void MedNUSFrontBar::changeDefaultView(){
    //Todo: Set Screen to default 3 screen view.
    emit toggleLayout(0);
}


void MedNUSFrontBar::resizeEvent(QResizeEvent* event)
{
    int space = (TOPBAR_HEIGHT-24)/2;
    _logo->setGeometry(QRect(this->x()+space, this->y()+space, TOPBAR_HEIGHT/40*76, TOPBAR_HEIGHT/40*21));
    _btView1->setGeometry(QRect(this->x()+this->geometry().width()-36*1, this->y()+space, 32,24));
    _btView2->setGeometry(QRect(this->x()+this->geometry().width()-36*2, this->y()+space, 32,24));
    _btView3->setGeometry(QRect(this->x()+this->geometry().width()-36*3, this->y()+space, 32,24));
    _logo->setVisible(true);

    if(_currentMode==STUDENT) {
        _btView1->setVisible(true);
        _btView2->setVisible(true);
        _btView3->setVisible(true);
    } else {
        _btView1->setVisible(false);
        _btView2->setVisible(false);
        _btView3->setVisible(false);
    }
}

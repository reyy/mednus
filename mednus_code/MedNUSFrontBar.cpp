#include "MedNUSFrontBar.h"
#include "MedNUSAUISettings.h"

MedNUSFrontBar::MedNUSFrontBar(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumHeight(TOPBAR_HEIGHT);
    this->setMaximumHeight(TOPBAR_HEIGHT);

    _background = new QLabel(parent);
    _background->setGeometry(QRect(this->x(), this->y(), this->width(), TOPBAR_HEIGHT));
    _background->setStyleSheet("background-color: #152d3b;");
    _background->setVisible(true);

    _logo = new QLabel(parent);
    int space = (TOPBAR_HEIGHT-TOPBAR_HEIGHT/40*21)/2;
    _logo->setGeometry(QRect(this->x()+space, this->y()+space, TOPBAR_HEIGHT/40*76, TOPBAR_HEIGHT/40*21));
    _logo->setPixmap(QPixmap(QString::fromStdString(":/images/logo.png")));
    _logo->setStyleSheet("background-color:rgba(0,0,0,0);");
    _logo->setScaledContents(true);
    _logo->setVisible(true);

    _avatar = new QLabel(parent);
    _avatar->setGeometry(QRect(this->x()+this->width()-TOPBAR_HEIGHT*0.9, this->y()+TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
    _avatar->setStyleSheet("background-color:rgba(0,0,0,0);");
    _avatar->setVisible(false);
    _avatar->setScaledContents(true);

    _cutoutAvatar = new QLabel(parent);
    _cutoutAvatar->setGeometry(QRect(this->x()+this->width()-TOPBAR_HEIGHT*0.9, this->y()+TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
    _cutoutAvatar->setPixmap(QPixmap(QString::fromStdString(":/images/avatar_cutout_dark.png")));
    _cutoutAvatar->setStyleSheet("background-color:rgba(0,0,0,0);");
    _cutoutAvatar->setVisible(false);
    _cutoutAvatar->setScaledContents(true);

    _name = new QLabel(parent);
    _name->setStyleSheet("background-color:rgba(0,0,0,0);font-size:14px;color:#FFFFFF;");
    _name->setAlignment(Qt::AlignRight);
    _name->setGeometry(QRect(this->x()+this->geometry().width()-LESSONPANEL_WIDTH+40, this->y()+(TOPBAR_HEIGHT-20)*0.5, LESSONPANEL_WIDTH-100, 20));
    _name->setVisible(false);

    _trayOut = true;
}

MedNUSFrontBar::~MedNUSFrontBar()
{
    delete _background;
    delete _logo;
    delete _avatar;
    delete _cutoutAvatar;
    delete _name;
}

void MedNUSFrontBar::setName(QString value) {
    QFontMetrics metrics(_name->font());
    _name->setText(metrics.elidedText(value, Qt::ElideRight, _name->width()));
    _name->setAlignment(Qt::AlignRight);
}

void MedNUSFrontBar::setAvatar(QString path) {
    _avatar->setPixmap(QPixmap(path));
}

void MedNUSFrontBar::setTrayOut(bool value) {
    _trayOut = value;

    if(!value) {
        _avatar->setVisible(false);
        _cutoutAvatar->setVisible(false);
        _name->setVisible(false);

    } else {
        _avatar->setVisible(true);
        _cutoutAvatar->setVisible(true);
        _name->setVisible(true);
    }
}

void MedNUSFrontBar::resizeEvent(QResizeEvent* event)
{
    _background->setGeometry(QRect(this->x(), this->y(), this->geometry().width(), TOPBAR_HEIGHT));
    int space = (TOPBAR_HEIGHT-TOPBAR_HEIGHT/40*21)/2;
    _logo->setGeometry(QRect(this->x()+space, this->y()+space, TOPBAR_HEIGHT/40*76, TOPBAR_HEIGHT/40*21));
    _avatar->setGeometry(QRect(this->x()+this->geometry().width()-TOPBAR_HEIGHT*0.9, this->y()+TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
    _cutoutAvatar->setGeometry(QRect(this->x()+this->geometry().width()-TOPBAR_HEIGHT*0.9, this->y()+TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
    _name->setGeometry(QRect(this->x()+this->geometry().width()-LESSONPANEL_WIDTH+40, this->y()+(TOPBAR_HEIGHT-20)*0.5, LESSONPANEL_WIDTH-100, 20));
}

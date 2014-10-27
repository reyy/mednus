#include "MedNUSUserBar.h"
#include "MedNUSAUISettings.h"

MedNUSUserBar::MedNUSUserBar(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(LESSONPANEL_WIDTH);
    this->setMinimumHeight(TOPBAR_HEIGHT);
    this->setMaximumHeight(TOPBAR_HEIGHT);

    _background = new QLabel(this);
    _background->setGeometry(QRect(this->x(), this->y(), LESSONPANEL_WIDTH, TOPBAR_HEIGHT));
    _background->setStyleSheet("background-color: #1c4f6e;");

    _avatar = new QLabel(this);
    _avatar->setGeometry(QRect(this->x()+LESSONPANEL_WIDTH-TOPBAR_HEIGHT*0.9-SIDEBAR_OFFSET, this->y()+TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
    _avatar->setPixmap(QPixmap(QString::fromStdString(":/images/ivle_profile.jpg")));
    _avatar->setScaledContents(true);

    _cutoutAvatar = new QLabel(this);
    _cutoutAvatar->setGeometry(QRect(this->x()+LESSONPANEL_WIDTH-TOPBAR_HEIGHT*0.9-SIDEBAR_OFFSET, this->y()+TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
    _cutoutAvatar->setPixmap(QPixmap(QString::fromStdString(":/images/avatar_cutout.png")));
    _cutoutAvatar->setScaledContents(true);

    _name = new QLabel(this);
    _name->setStyleSheet("font-size:14px;color:#FFFFFF;");
    _name->setGeometry(QRect(this->x()+LESSONPANEL_WIDTH-SIDEBAR_OFFSET-100-10-TOPBAR_HEIGHT, this->y()+(TOPBAR_HEIGHT-20)*0.5, 100, 20));
    _name->setAlignment(Qt::AlignRight);
}

MedNUSUserBar::~MedNUSUserBar()
{
    delete _background;
    delete _avatar;
    delete _cutoutAvatar;
    delete _name;
}

void MedNUSUserBar::setName(QString value) {
    QFontMetrics metrics(_name->font());
    _name->setText(metrics.elidedText(value, Qt::ElideRight, _name->width()));
}

void MedNUSUserBar::setAvatar(QString path) {
    _avatar->setPixmap(QPixmap(path));
}

void MedNUSUserBar::setTrayOut(bool value) {
    _trayOut = value;

    if(value) {
        this->setMinimumWidth(SIDEBAR_OFFSET);
        this->setMinimumHeight(TOPBAR_HEIGHT);
        this->setMaximumHeight(TOPBAR_HEIGHT);
        _avatar->setVisible(false);
        _cutoutAvatar->setVisible(false);
        _name->setVisible(false);
    } else {
        this->setMinimumWidth(LESSONPANEL_WIDTH);
        this->setMinimumHeight(TOPBAR_HEIGHT);
        this->setMaximumHeight(TOPBAR_HEIGHT);
        _avatar->setVisible(true);
        _cutoutAvatar->setVisible(true);
        _name->setVisible(true);
    }
}

void MedNUSUserBar::resizeEvent(QResizeEvent* event)
{
}

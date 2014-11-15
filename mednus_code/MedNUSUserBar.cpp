#include "MedNUSUserBar.h"
#include "MedNUSAUISettings.h"
#include <QMenu>

MedNUSUserBar::MedNUSUserBar(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(LESSONPANEL_WIDTH);
    this->setMinimumHeight(TOPBAR_HEIGHT);
    this->setMaximumHeight(TOPBAR_HEIGHT);
    this->setStyleSheet("background-color: #152d3b;");

    _background = new QLabel(this);
    _background->setGeometry(QRect(this->x()+SIDEBAR_OFFSET, this->y(), LESSONPANEL_WIDTH, TOPBAR_HEIGHT));
    _background->setStyleSheet("background-color: #1c4f6e;");

    _avatar = new QLabel(this);
    _avatar->setPixmap(QPixmap(QString::fromStdString(":/images/ivle_profile.jpg")));
    _avatar->setScaledContents(true);
    _avatar->setStyleSheet("background-color:rgba(0,0,0,0);");

    _cutoutAvatar = new QLabel(this);
    _cutoutAvatar->setPixmap(QPixmap(QString::fromStdString(":/images/avatar_cutout.png")));
    _cutoutAvatar->setScaledContents(true);
    _cutoutAvatar->setStyleSheet("background-color:rgba(0,0,0,0);");

    _name = new QLabel(this);
    _name->setStyleSheet("background-color:rgba(0,0,0,0);font-size:14px;color:#FFFFFF;text-align:right;");
    _name->setGeometry(QRect(this->x()+this->geometry().width()-LESSONPANEL_WIDTH-SIDEBAR_OFFSET+60, this->y()+(TOPBAR_HEIGHT-20)*0.5, LESSONPANEL_WIDTH-100, 20));

    this->setTrayOut(false);
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
    _name->setAlignment(Qt::AlignRight);
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
        _avatar->setGeometry(QRect(SIDEBAR_OFFSET+TOPBAR_HEIGHT*0.1, this->y()+TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
        _cutoutAvatar->setGeometry(QRect(SIDEBAR_OFFSET+TOPBAR_HEIGHT*0.1, this->y()+TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
         _avatar->setVisible(true);
        _name->setVisible(false);
    } else {
        this->setMinimumWidth(LESSONPANEL_WIDTH);
        this->setMinimumHeight(TOPBAR_HEIGHT);
        this->setMaximumHeight(TOPBAR_HEIGHT);
        _avatar->setGeometry(QRect(LESSONPANEL_WIDTH-TOPBAR_HEIGHT*0.9, this->y()+TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
        _cutoutAvatar->setGeometry(QRect(LESSONPANEL_WIDTH-TOPBAR_HEIGHT*0.9, this->y()+TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
        _avatar->setVisible(true);
        _name->setVisible(true);
    }
}

void MedNUSUserBar::resizeEvent(QResizeEvent* event)
{
}

void MedNUSUserBar::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton) {
        if(event->x()>=_avatar->x()&&
           event->x()<=_avatar->x()+_avatar->width()&&
           event->y()>=_avatar->y()&&
           event->y()<=_avatar->y()+_avatar->height())
        showContextMenu(event->pos());
    }
}

void MedNUSUserBar::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = this->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("Logout");
    QAction* selectedItem = myMenu.exec(this->mapToGlobal(_avatar->pos())+QPoint(-24,24));
    if (selectedItem)
    {
        qDebug() <<"Logout";
        emit emitLogout();
    }
    else
    {
    }
}

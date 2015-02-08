#include "MedNUSUserBar.h"
#include <QMenu>

MedNUSUserBar::MedNUSUserBar(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(LESSONPANEL_WIDTH);
    this->setMinimumHeight(TOPBAR_HEIGHT);
    this->setMaximumHeight(TOPBAR_HEIGHT);
    this->setStyleSheet("background-color: #152d3b;");

    _currentMode=NONE;

    _backgroundLine = new QLabel(this);
    _backgroundLine->setGeometry(QRect(this->x(), this->y(), SIDEBAR_OFFSET, TOPBAR_HEIGHT));
    _backgroundLine->setStyleSheet("QLabel {background-image: url(:/images/login_lines.png);background-repeat: repeat-xy;}");
    _backgroundLine->setVisible(false);

    _background = new QLabel(this);
    _background->setGeometry(QRect(this->x()+SIDEBAR_OFFSET, this->y(), LESSONPANEL_WIDTH, TOPBAR_HEIGHT));
    _background->setStyleSheet("background-color: #13181b;");

    _avatar = new QLabel(this);
    _avatar->setPixmap(QPixmap(QString::fromStdString(":/images/ivle_profile.png")));
    _avatar->setScaledContents(true);
    _avatar->setStyleSheet("background-color:rgba(0,0,0,0);");

    _cutoutAvatar = new QLabel(this);
    _cutoutAvatar->setPixmap(QPixmap(QString::fromStdString(":/images/avatar_cutout.png")));
    _cutoutAvatar->setScaledContents(true);
    _cutoutAvatar->setStyleSheet("background-color:rgba(0,0,0,0);");

    _name = new QLabel(this);
    _name->setStyleSheet("background-color:rgba(0,0,0,0);color:#FFFFFF;text-align:right;");
    _name->setGeometry(QRect(this->x()+this->geometry().width()-LESSONPANEL_WIDTH-SIDEBAR_OFFSET+70, this->y()+(TOPBAR_HEIGHT-20)*0.5+3, LESSONPANEL_WIDTH-100, 20));
    _name->setFont (QFont ("Helvetica", 14,QFont::Bold,false));
    this->setTrayOut(true);

    _btNewLesson = new QPushButton(this);
    _btNewLesson->setIconSize(QSize(24,24));
    _btNewLesson->setFlat(true);
    _btNewLesson->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_newlesson.png);} QPushButton::pressed {background-image: url(:/images/bt_newlesson_p.png);}");
    _btNewLesson->setGeometry(QRect(24.0,8.0, 60,24));
    _btNewLesson->setVisible(true);

    _btViewStats = new QPushButton(this);
    _btViewStats->setIconSize(QSize(24,24));
    _btViewStats->setFlat(true);
    _btViewStats->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_viewstats.png);} QPushButton::pressed {background-image: url(:/images/bt_viewstats_p.png);}");
    _btViewStats->setGeometry(QRect(24.0+65.0,8.0, 60,24));
    _btViewStats->setVisible(true);
}

MedNUSUserBar::~MedNUSUserBar()
{
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
        if(_currentMode==STUDENT) {
            this->setMinimumWidth(LESSONPANEL_WIDTH);
            this->setMaximumWidth(LESSONPANEL_WIDTH);
            this->setMinimumHeight(TOPBAR_HEIGHT);
            this->setMaximumHeight(TOPBAR_HEIGHT);
            _avatar->setGeometry(QRect(LESSONPANEL_WIDTH-TOPBAR_HEIGHT*0.9, TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
            _cutoutAvatar->setGeometry(QRect(LESSONPANEL_WIDTH-TOPBAR_HEIGHT*0.9, TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
            _background->setGeometry(QRect(SIDEBAR_OFFSET, 0.0, LESSONPANEL_WIDTH, TOPBAR_HEIGHT));
            _name->setGeometry(QRect(this->geometry().width()-LESSONPANEL_WIDTH-SIDEBAR_OFFSET+70, this->y()+(TOPBAR_HEIGHT-20)*0.5+3, LESSONPANEL_WIDTH-100, 20));
        } else {
            this->setMinimumWidth(LESSONPANEL_WIDTH_L);
            this->setMaximumWidth(LESSONPANEL_WIDTH_L);
            this->setMinimumHeight(TOPBAR_HEIGHT);
            this->setMaximumHeight(TOPBAR_HEIGHT);
            _avatar->setGeometry(QRect(LESSONPANEL_WIDTH_L-TOPBAR_HEIGHT*0.9, TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
            _cutoutAvatar->setGeometry(QRect(LESSONPANEL_WIDTH_L-TOPBAR_HEIGHT*0.9, TOPBAR_HEIGHT*0.1, TOPBAR_HEIGHT*0.8, TOPBAR_HEIGHT*0.8));
            _background->setGeometry(QRect(SIDEBAR_OFFSET, 0.0, LESSONPANEL_WIDTH_L, TOPBAR_HEIGHT));
            _name->setGeometry(QRect(this->geometry().width()-LESSONPANEL_WIDTH_L-SIDEBAR_OFFSET+70, (TOPBAR_HEIGHT-20)*0.5+3, LESSONPANEL_WIDTH_L-100, 20));

        }
        _avatar->setVisible(true);
        _name->setVisible(true);
    }


    if(_currentMode==STUDENT) {
        _cutoutAvatar->setPixmap(QPixmap(QString::fromStdString(":/images/avatar_cutout.png")));
        _background->setStyleSheet("background-color: #13181b;");
    } else {
        _cutoutAvatar->setPixmap(QPixmap(QString::fromStdString(":/images/avatar_cutout2.png")));
        _background->setStyleSheet("background-color: #392d0f;");
    }
}

void MedNUSUserBar::setMode(interfaceMode mode) {
    _currentMode=mode;
}


void MedNUSUserBar::resizeEvent(QResizeEvent* event)
{
    bool toShow=!_trayOut;
    if(_currentMode==STUDENT)
        toShow=false;
    _btNewLesson->setVisible(toShow);
    _btViewStats->setVisible(toShow);
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

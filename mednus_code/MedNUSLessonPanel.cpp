/* MedNUSLessonPanel.h

   Table widget for lesson plan organization.

   Author: Jay Chua
*/

#include "MedNUSLessonPanel.h"
#include <QtGui>
#include <QPalette>

MedNUSLessonPanel::MedNUSLessonPanel(QWidget *parent) : QWidget(parent) {
    this->setMinimumWidth(LESSONPANEL_WIDTH);
    this->setMaximumWidth(LESSONPANEL_WIDTH);

    _currentMode=NONE;

    loadPixmap();
    _background = new QLabel(this);
    _background->setGeometry(QRect(SIDEBAR_OFFSET,0, this->width()-SIDEBAR_OFFSET, this->height()));
    _background->setStyleSheet("background-color: #13181b;");

    _button = new QLabel(this);
    _button->setGeometry(QRect(0,this->height()/2-SIDEBAR_OFFSET*3.2, SIDEBAR_OFFSET, SIDEBAR_OFFSET*6.4));
    _button->setScaledContents(true);
    _button->setPixmap(_button_toclose);

    this->setTrayOut(true);
}

MedNUSLessonPanel::~MedNUSLessonPanel() {
    clearLesson();
}

void MedNUSLessonPanel::setMode(interfaceMode mode) {
    _currentMode=mode;

    if(mode==STUDENT) {
        this->setMinimumWidth(LESSONPANEL_WIDTH);
        this->setMaximumWidth(LESSONPANEL_WIDTH);
        _background->setGeometry(QRect(SIDEBAR_OFFSET,0, this->width()-SIDEBAR_OFFSET, this->height()));
        _background->setStyleSheet("background-color: #13181b;");
    } else  {
        this->setMinimumWidth(LESSONPANEL_WIDTH_L);
        this->setMaximumWidth(LESSONPANEL_WIDTH_L);
        _background->setGeometry(QRect(SIDEBAR_OFFSET,0, this->width()-SIDEBAR_OFFSET, this->height()));
        _background->setStyleSheet("background-color: #392d0f;");
    }
}

void MedNUSLessonPanel::loadPixmap() {
    _button_toopen = QPixmap(QString::fromStdString(":/images/button_trayout.png"));
    _button_toclose = QPixmap(QString::fromStdString(":/images/button_trayin.png"));
    _button_toopen2 = QPixmap(QString::fromStdString(":/images/button_trayout2.png"));
    _button_toclose2 = QPixmap(QString::fromStdString(":/images/button_trayin2.png"));
    _icon_3d = QPixmap(QString::fromStdString(":/images/icon_3d.png"));
    _icon_image = QPixmap(QString::fromStdString(":/images/icon_image.png"));
    _icon_pdf = QPixmap(QString::fromStdString(":/images/icon_pdf.png"));
    _icon_quiz = QPixmap(QString::fromStdString(":/images/icon_quiz.png"));
    _icon_video = QPixmap(QString::fromStdString(":/images/icon_video.png"));

    for(int i=0;i<11;i++) {
        _loadingIcon[i] = QPixmap(QString::fromStdString(":/images/lessonLoading_")+QString::number(i)+QString::fromStdString(".png"));
    }
}

void MedNUSLessonPanel::addLesson(MedNUSLessonPackage * _package) {
    _package->setMode(_currentMode);
    _lessonList.push_back(_package);
    updateGUI();
}

void MedNUSLessonPanel::addLesson(QString title,QString subTitle, QString description, QStringList directories) {
    MedNUSLessonPackage *_package = new MedNUSLessonPackage(this);
    _package->setTitle(title);
    _package->setSubHeader(subTitle);
    _package->setDescription(description);
    _package->setMode(_currentMode);

    for(int i=0;i<directories.size();i++) {
        QString directory = directories.at(i);
        QPixmap icon_directory;
        if(directory.contains(".png", Qt::CaseInsensitive))
            icon_directory = _icon_image;
        if(directory.contains(".ply", Qt::CaseInsensitive))
            icon_directory = _icon_3d;
        if(directory.contains(".pdf", Qt::CaseInsensitive))
            icon_directory = _icon_pdf;
        if(directory.contains(".qiz", Qt::CaseInsensitive))
            icon_directory = _icon_quiz;
        if(directory.contains(".mp4", Qt::CaseInsensitive))
            icon_directory = _icon_video;
        _package->addContent(directory,icon_directory);
    }

    _lessonList.push_back(_package);
    updateGUI(); 

    connect(_package, SIGNAL(emitOpenFile(QString,QString,int)), this, SLOT(callOpenFile(QString,QString,int)));
}

bool MedNUSLessonPanel::removeLesson(QString title) {
    for(int i=0;i<(int)_lessonList.size();i++) {
        MedNUSLessonPackage *temp = _lessonList.at(i);
        if(temp->getTitle().compare(title)) {
            delete temp;
            updateGUI();
            return true;
        }
    }
    return false;
}

void MedNUSLessonPanel::clearLesson() {
    while(_lessonList.size()>0) {
        delete _lessonList.back();
        _lessonList.pop_back();
    }
    updateGUI();
}

void MedNUSLessonPanel::updateGUI() {
    int offset=0;
    for(int i=0;i<(int)_lessonList.size();i++) {
        MedNUSLessonPackage *temp = _lessonList.at(i);
        temp->setTone(i);
        temp->setY(offset);
        temp->updateGUI(_trayOut);
        offset+=temp->getHeight();
    }
}

void MedNUSLessonPanel::setTrayOut(bool value) {
    _trayOut = value;

    qDebug() << "LP mode is " << _currentMode;

    if(value) {
        this->setMinimumWidth(SIDEBAR_OFFSET+TOPBAR_HEIGHT);
        this->setMaximumWidth(SIDEBAR_OFFSET+TOPBAR_HEIGHT);
        if(_currentMode==STUDENT) {
            _button->setPixmap(_button_toopen);
        } else {
            _button->setPixmap(_button_toopen2);
        }

        //Collapsed everything.
        for(int i=0;i<(int)_lessonList.size();i++) {
            _lessonList.at(i)->toggleCollapse(true);
            _lessonList.at(i)->updateGUI(true);
        }
        this->updateGUI();

    } else {
        if(_currentMode==STUDENT) {
            this->setMinimumWidth(LESSONPANEL_WIDTH);
            this->setMaximumWidth(LESSONPANEL_WIDTH);
            _button->setPixmap(_button_toclose);
        } else {
            this->setMinimumWidth(LESSONPANEL_WIDTH_L);
            this->setMaximumWidth(LESSONPANEL_WIDTH_L);
            _button->setPixmap(_button_toclose2);
        }
    }
}

bool MedNUSLessonPanel::checkTray() {
    return _trayOut;
}

QPixmap MedNUSLessonPanel::getLoadingIcon(int range) {
    return _loadingIcon[MIN(10,MAX(0,range))];
}

void MedNUSLessonPanel::mousePressEvent ( QMouseEvent * event )
{
    if(event->buttons() == Qt::LeftButton) {
        if(event->x()>=0&&event->x()<=SIDEBAR_OFFSET&&event->y()>=this->height()/2-SIDEBAR_OFFSET*3.2&&event->y()<=this->height()/2+SIDEBAR_OFFSET*3.2){
            setTrayOut(!_trayOut);
        }
        if(event->x()>SIDEBAR_OFFSET)
            setTrayOut(false);

        //qDebug() << "Debug Message";
        bool collapseEveryoneElse=false;
        MedNUSLessonPackage *temp=NULL;
        MedNUSLessonPackage *selected=NULL;
        MedNUSLessonPackage *temp2=NULL;

        for(int i=0;i<(int)_lessonList.size();i++) {
            temp = _lessonList.at(i);
            if(event->pos().x()>10&&event->pos().y()>=temp->getY()&&event->pos().y()<=temp->getY()+temp->getInteractiveHeight()) {
                if(event->pos().x()<LESSONPANEL_WIDTH_L) {
                    selected=temp;
                    selected->toggleCollapse();
                    selected->updateGUI(false);
                    collapseEveryoneElse=true;
                    break;
                }
             }
        }

        if(collapseEveryoneElse) {
            for(int i=0;i<(int)_lessonList.size();i++) {
                temp2 = _lessonList.at(i);
                if(selected!=temp2) {
                    temp2->toggleCollapse(true);
                    temp2->updateGUI(false);
                }
            }
        }

        this->updateGUI();
    }

    event->ignore();
}

void MedNUSLessonPanel::resizeEvent(QResizeEvent* event)
{
    _background->setGeometry(QRect(SIDEBAR_OFFSET,0, this->width()-SIDEBAR_OFFSET,this->height()));
    _button->setGeometry(QRect(0,this->height()/2-32, SIDEBAR_OFFSET, 64));
}

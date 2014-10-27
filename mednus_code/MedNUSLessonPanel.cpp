/* MedNUSLessonPanel.h

   Table widget for lesson plan organization.

   Author: Jay Chua
*/

#include "MedNUSLessonPanel.h"
#include "MedNUSAUISettings.h"
#include <QtGui>
#include <QPalette>



MedNUSLessonPanel::MedNUSLessonPanel(QWidget *parent) : QWidget(parent) {
    this->setMinimumWidth(LESSONPANEL_WIDTH);
    this->setMaximumWidth(LESSONPANEL_WIDTH);

    loadPixmap();
    _background = new QLabel(this);
    _background->setGeometry(QRect(0,0, this->width(), this->height()));
    _background->setStyleSheet("background-color: #1a394a;");

    _dividerBackground = new QLabel(this);
    _dividerBackground->setGeometry(QRect(0,0, SIDEBAR_OFFSET, this->height()));
    _dividerBackground->setStyleSheet("background-color: #162a37;");

    _button = new QLabel(this);
    _button->setGeometry(QRect(0,this->height()/2-SIDEBAR_OFFSET*3.2, SIDEBAR_OFFSET, SIDEBAR_OFFSET*6.4));
    _button->setScaledContents(true);
    _button->setPixmap(_button_toclose);

    _trayOut = true;
}

MedNUSLessonPanel::~MedNUSLessonPanel() {
    clearLesson();
    delete _background;
    delete _dividerBackground;
    delete _button;
}

void MedNUSLessonPanel::loadPixmap() {

    _button_toopen = QPixmap(QString::fromStdString(":/images/button_trayout.png"));
    _button_toclose = QPixmap(QString::fromStdString(":/images/button_trayin.png"));
    _icon_3d = QPixmap(QString::fromStdString(":/images/icon_3d_small.png"));
    _icon_image = QPixmap(QString::fromStdString(":/images/icon_image_small.png"));
    _icon_pdf = QPixmap(QString::fromStdString(":/images/icon_pdf_small.png"));
    _icon_quiz = QPixmap(QString::fromStdString(":/images/icon_quiz_small.png"));
    _icon_video = QPixmap(QString::fromStdString(":/images/icon_video_small.png"));
}

void MedNUSLessonPanel::addLesson(MedNUSLessonPackage * _package) {
    _lessonList.push_back(_package);
    updateGUI();
}

void MedNUSLessonPanel::addLesson(QString title,QString subTitle, QString description, QStringList directories) {
    MedNUSLessonPackage *_package = new MedNUSLessonPackage(this);
    _package->setTitle(title);
    _package->setSubHeader(subTitle);
    _package->setDescription(description);

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
        temp->setTone((int)_lessonList.size());
        temp->setY(offset);
        temp->updateGUI();
        offset+=temp->getHeight();
    }
}

void MedNUSLessonPanel::setTrayOut(bool value) {
    _trayOut = value;

    if(value) {
        this->setMinimumWidth(SIDEBAR_OFFSET);
        this->setMaximumWidth(SIDEBAR_OFFSET);
        _button->setPixmap(_button_toopen);

    } else {
        this->setMinimumWidth(LESSONPANEL_WIDTH);
        this->setMaximumWidth(LESSONPANEL_WIDTH);
        _button->setPixmap(_button_toclose);
    }
}

bool MedNUSLessonPanel::checkTrayButton(float xpos, float ypos) {
    xpos-=this->pos().x();
    ypos-=this->pos().y();

    if(xpos>=0&&xpos<=SIDEBAR_OFFSET&&ypos>=this->height()/2-SIDEBAR_OFFSET*3.2&&ypos<=this->height()/2+SIDEBAR_OFFSET*3.2) {
        return true;
    }
    return false;
}

void MedNUSLessonPanel::mousePressEvent ( QMouseEvent * event )
{
    if(event->buttons() == Qt::LeftButton) {
        //qDebug() << "Debug Message";
        bool collapseEveryoneElse=false;
        MedNUSLessonPackage *temp, *temp2;

        for(int i=0;i<(int)_lessonList.size();i++) {
            temp = _lessonList.at(i);
            if(event->pos().x()>10&&event->pos().y()>=temp->getY()&&event->pos().y()<=temp->getY()+temp->getInteractiveHeight()) {
                temp->toggleCollapse();
                temp->updateGUI();
                collapseEveryoneElse=true;
                break;
             }

            //For mouse click.
            if(!temp->getCollapse()) {
                if(event->pos().y()>=temp->getY()&&event->pos().y()<=temp->getY()+temp->getHeight()) {
                    for(int j=0;j<temp->getContentSize();j++) {
                        temp->getContentItem(j)->checkMouseClick(event->pos().x(),event->pos().y());
                    }
                }
            }
        }

        if(collapseEveryoneElse) {
            for(int i=0;i<(int)_lessonList.size();i++) {
                temp2 = _lessonList.at(i);
                if(temp!=temp2) {
                    temp2->toggleCollapse(true);
                    temp2->updateGUI();
                }
            }
        }

        this->updateGUI();
    }
    event->ignore();
}

void MedNUSLessonPanel::resizeEvent(QResizeEvent* event)
{
    _background->setGeometry(QRect(0,0, this->width(),this->height()));
    _dividerBackground->setGeometry(QRect(0,0, SIDEBAR_OFFSET, this->height()));
    _button->setGeometry(QRect(0,this->height()/2-32, SIDEBAR_OFFSET, 64));
}

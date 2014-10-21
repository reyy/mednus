/* MedNUSLessonPanel.h

   Table widget for lesson plan organization.

   Author: Jay Chua
*/

#include "MedNUSLessonPanel.h"
#include <QtGui>
#include <QPalette>



MedNUSLessonPanel::MedNUSLessonPanel(QWidget *parent) : QWidget(parent)
{
    this->setMinimumWidth(300);
    this->setMaximumWidth(300);
    this->setMinimumHeight(parent->height());
    this->setMaximumHeight(parent->height());
    _background = new QLabel(this);
    _background->setGeometry(QRect(this->x(), this->y(), this->width(), this->height()));
    _background->setStyleSheet("background-color: #193b50;");
 }


MedNUSLessonPanel::~MedNUSLessonPanel()
{
    clearLesson();
    delete _background;
}

void MedNUSLessonPanel::addLesson(QString title,QString subTitle, QString description) {
    MedNUSLessonPackage *_package = new MedNUSLessonPackage(this);
    _package->setTitle(title);
    _package->setSubHeader(subTitle);
    _package->setDescription(description);
    if((int)_lessonList.size()>0)
        _package->toggleCollapse();
    _lessonList.push_back(_package);
    updateGUI();
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
        temp->setTone((int)((float)i/(float)_lessonList.size()*16.0));
        temp->setY(offset);
        temp->updateGUI();
        offset+=temp->getHeight();
        //qDebug() << offset;
    }
}

void MedNUSLessonPanel::mousePressEvent ( QMouseEvent * event )
{
    //qDebug() << "Debug Message";
    bool collapseEveryoneElse=false;
    MedNUSLessonPackage *temp, *temp2;

    for(int i=0;i<(int)_lessonList.size();i++) {
        temp = _lessonList.at(i);
        if(event->pos().y()>=temp->getY()&&event->pos().y()<=temp->getY()+temp->getHeight())
        {
            temp->toggleCollapse();
            temp->updateGUI();
            collapseEveryoneElse=true;
            break;
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

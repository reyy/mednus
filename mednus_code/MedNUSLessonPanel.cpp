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
    QLabel *_background = new QLabel(this);
    _background->setGeometry(QRect(this->x(), this->y(), this->width(), this->height()));
    _background->setStyleSheet("background-color: #193b50;");

    _package = new MedNUSLessonPackage(this);
 }


MedNUSLessonPanel::~MedNUSLessonPanel()
{
    delete _package;
}

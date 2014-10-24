#include "MedNUSLessonIcon.h"
#include <QDebug>

MedNUSLessonIcon::MedNUSLessonIcon(QString title, QPixmap directory, QWidget *parent)
{
    _selected = false;
    _directory = directory;

    _highlight = new QLabel(parent);
    _highlight->setStyleSheet("background-color: #4e698a;border-color:#ffffff;border-style: dotted;border-width: 1px;");

    _icon = new QLabel(parent);
    _icon->setPixmap(directory);

    _text = new QLabel(parent);
    _text->setStyleSheet("color:#FFFFFF;font-size:10px;");
    QFontMetrics metrics(_text->font());
    _text->setText(metrics.elidedText(title, Qt::ElideRight, _text->width()));

    _x=0;
    _y=0;
}

MedNUSLessonIcon::~MedNUSLessonIcon() {
    delete _icon;
    delete _text;
}

void MedNUSLessonIcon::updatePosition(float x, float y) {
    _x=x;
    _y=y;
    _icon->setGeometry(QRect(_x, _y, _directory.width(), _directory.height()));
    _text->setGeometry(QRect(_x+_directory.width()+5, _y+1, 150, 16));
    _highlight->setGeometry(QRect(_x-2, _y-2, 150+4, _directory.height()+4));
}

void MedNUSLessonIcon::setSelected(bool value) {
    _selected=value;
}

void MedNUSLessonIcon::setVisible(bool value) {
    _icon->setVisible(value);
    _text->setVisible(value);
}

bool MedNUSLessonIcon::checkMouseClick(float xpos, float ypos) {
    if(xpos>=_x&&xpos<=_x+150&&ypos>=_y&&ypos<=_y+_directory.height()) {
        _selected = true;
        qDebug() << "Ya";
         _highlight->setVisible(true);
        return true;
    } else {
        _selected = false;
        _highlight->setVisible(false);
        qDebug() << "NO";
        return false;
    }
}

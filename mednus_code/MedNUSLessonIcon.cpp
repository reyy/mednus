#include "MedNUSLessonIcon.h"
#include <QDebug>

MedNUSLessonIcon::MedNUSLessonIcon(QString path, QPixmap directory, QWidget *parent)
{
    _selected = false;
    _directory = directory;

    _path = path;

    _highlight = new QLabel(parent);
    _highlight->setStyleSheet("background-color: #4e698a;border-color:#ffffff;border-style: dotted;border-width: 1px;");

    _icon = new QLabel(parent);
    _icon->setPixmap(directory);

    _text = new QLabel(parent);
    _text->setStyleSheet("color:#FFFFFF;font-size:10px;");
    QFontMetrics metrics(_text->font());

    int startIndex=_path.lastIndexOf("/");
    int nameLength=_path.size()-startIndex;
    QString filename = _path.mid(startIndex+1,nameLength-1);

    _text->setText(metrics.elidedText(filename, Qt::ElideRight, _text->width()));

    _x=0;
    _y=0;
}

MedNUSLessonIcon::~MedNUSLessonIcon() {
    delete _icon;
    delete _text;
}

void MedNUSLessonIcon::updatePosition(float x, float y) {
    _x=x+10;
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

    if(_selected&&value)
        _highlight->setVisible(true);
    else
        _highlight->setVisible(false);
}


bool MedNUSLessonIcon::checkMouseClick(float xpos, float ypos) {
    if(xpos>=_x&&xpos<=_x+150&&ypos>=_y&&ypos<=_y+_directory.height()) {
        _selected = true;
        _highlight->setVisible(true);
        emit emitOpenFile(_path, _text->text(), 0); //todo: pass actual type!
        return true;
    } else {
        _selected = false;
        _highlight->setVisible(false);
        return false;
    }
}

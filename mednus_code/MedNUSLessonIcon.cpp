#include "MedNUSLessonIcon.h"
#include "MedNUSAUISettings.h"
#include <QDebug>

MedNUSLessonIcon::MedNUSLessonIcon(QString path, QPixmap directory, QWidget *parent) : QWidget(parent)
{
    _selected = false;
    _directory = directory;

    _path = path;
    _parent = parent;

    _highlight = new QLabel(this);
    _highlight->setStyleSheet("background: rgba(229,165,57,50);");

    _icon = new QLabel(this);
    _icon->setPixmap(directory);
    _icon->setStyleSheet("background:rgba(0,0,0,0);color:#FFFFFF;");
    _icon->setScaledContents(true);

    _text = new QLabel(this);
    _text->setStyleSheet("background:rgba(0,0,0,0);color:#FFFFFF;");
    _text->setFont (QFont ("Helvetica", 10,QFont::Normal,false));

    int startIndex=_path.lastIndexOf("/");
    int nameLength=_path.size()-startIndex;
    _filename = _path.mid(startIndex+1,nameLength-1);

    QFontMetrics metrics(_text->font());
    _text->setText(metrics.elidedText(_filename, Qt::ElideRight, _text->width()));

    _x=0;
    _y=0;
}

MedNUSLessonIcon::~MedNUSLessonIcon() {
}

void MedNUSLessonIcon::updatePosition(float packageX, float packageY, float x, float y) {
    _x=packageX;
    _y=packageY+y;
    qDebug() <<packageX<<" "<<packageY<<" "<<_x<<" "<< _y;
    _icon->setGeometry(QRect(LESSONPANEL_BORDER+1,3, 12, 16));

    if(_scrollBarExist) {
        _text->setGeometry(QRect(LESSONPANEL_BORDER+15+5, 3, LESSONPANEL_WIDTH-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET-18+1, 16));
        _highlight->setGeometry(QRect(LESSONPANEL_BORDER-2, 0, LESSONPANEL_WIDTH-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET-18+1, 20+2));

        this->setGeometry(QRect(x+LESSONPANEL_BORDER-2, y, LESSONPANEL_WIDTH-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET-16+1, 20+5));
    } else {
        _text->setGeometry(QRect(LESSONPANEL_BORDER+15+5, 3, LESSONPANEL_WIDTH-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET-12+1, 16));
        _highlight->setGeometry(QRect(LESSONPANEL_BORDER-2, 0, LESSONPANEL_WIDTH-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET-12+1, 20+2));

        this->setGeometry(QRect(x+LESSONPANEL_BORDER-2, y, LESSONPANEL_WIDTH-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET-10+1, 20+5));
    }

    QFontMetrics metrics(_text->font());
    _text->setText(metrics.elidedText(_filename, Qt::ElideRight, _text->width()-30));
}

void MedNUSLessonIcon::setHighlight(bool status) {
    _selected = status;
    _highlight->setVisible(status);
    _highlight->repaint();
    _parent->repaint();
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

void MedNUSLessonIcon::mousePressEvent ( QMouseEvent * event ){
    setHighlight(true);
    emit emitOpenFile(_path, _text->text(), 0); //todo: pass actual type!
    event->ignore();
}

void MedNUSLessonIcon::tabOpened(QString path)
{
    if(path.contains( _path))
        setHighlight(true);
}

void MedNUSLessonIcon::tabClosed(QString path)
{
    if(path.contains( _path))
        setHighlight(false);
}

void MedNUSLessonIcon::setScrollBarSpace(bool value) {
    _scrollBarExist = value;
}

#include "MedNUSLessonIcon.h"
#include <QDebug>

MedNUSLessonIcon::MedNUSLessonIcon(QString path, fileType filetype, QWidget *parent) : QWidget(parent)
{
    _selected = false;
    _filetype = filetype;
    _path = path;
    _parent = parent;

    _highlight = new QLabel(this);
    _highlight->setStyleSheet("background: rgba(229,165,57,50);");

    _icon = new QLabel(this);
    switch(filetype) {
    case MODEL:_icon->setPixmap(QPixmap(QString::fromStdString(":/images/icon_3d.png")));break;
    case IMAGE:_icon->setPixmap(QPixmap(QString::fromStdString(":/images/icon_image.png")));break;
    case PDF:_icon->setPixmap(QPixmap(QString::fromStdString(":/images/icon_pdf.png")));break;
    case QUIZ:_icon->setPixmap(QPixmap(QString::fromStdString(":/images/icon_quiz.png")));break;
    case VIDEO:_icon->setPixmap(QPixmap(QString::fromStdString(":/images/icon_video.png")));break;
    default:break;
    }
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

    _currentMode=NONE;

    _btDelete = new QPushButton(this);
    _btDelete->setIconSize(QSize(16,16));
    _btDelete->setStyleSheet("QPushButton {background-color:transparent;border-style: outset; border-width: 0px;background-image: url(:/images/bt_sDelete.png);} QPushButton::pressed {background-color:transparent;background-image: url(:/images/bt_sDelete_p.png);}");
    _btDelete->setFlat(true);
    _btDelete->setVisible(false);

    _btMisc = new QPushButton(this);
    _btMisc->setIconSize(QSize(48,16));
    _btMisc->setStyleSheet("QPushButton {background-color:transparent;border-style: outset; border-width: 0px;background-image: url(:/images/bt_stats.png);} QPushButton::pressed {background-color:transparent;background-image: url(:/images/bt_stats_p.png);}");
    _btMisc->setFlat(true);
    _btMisc->setVisible(false);

    connect(_btMisc,SIGNAL(clicked()),this,SLOT(activateMISC()));
    connect(_btDelete,SIGNAL(clicked()),this,SLOT(deleteSelection()));
}

MedNUSLessonIcon::~MedNUSLessonIcon() {
}

void MedNUSLessonIcon::setMode(interfaceMode mode) {
    _currentMode=mode;
}

void MedNUSLessonIcon::updatePosition(float packageX, float packageY, float x, float y) {
    _x=packageX;
    _y=packageY+y;
    _icon->setGeometry(QRect(LESSONPANEL_BORDER+1,3, 12, 16));
    float tempWidth = 0.0;

    if(_currentMode==STUDENT) {
        tempWidth = LESSONPANEL_WIDTH;
    } else {
        tempWidth = LESSONPANEL_WIDTH_L;
    }

    if(_scrollBarExist) {
        tempWidth-=6;
    }

    _btDelete->setGeometry(QRect(tempWidth-LESSONPACKAGEBUTTON_OFFSET+24+5, 2, 16,16));
    _btMisc->setGeometry(QRect(tempWidth-LESSONPACKAGEBUTTON_OFFSET-24, 2, 48,16));
    _text->setGeometry(QRect(LESSONPANEL_BORDER+15+5, 3, tempWidth-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET-12+1-LESSONPACKAGEBUTTON_OFFSET, 16));
    _highlight->setGeometry(QRect(LESSONPANEL_BORDER-2, 0, tempWidth-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET-12+1-LESSONPACKAGEBUTTON_OFFSET, 20+2));
    QFontMetrics metrics(_text->font());
    _text->setText(metrics.elidedText(_filename, Qt::ElideRight, _text->width()-30));
    this->setGeometry(QRect(x+LESSONPANEL_BORDER-2, y, tempWidth-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET-10+1, 20+5));
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

    if(_currentMode!=STUDENT) {
        _btDelete->setVisible(value);

        //Set Stats button only for the quiz option.
        if(_filetype==fileType::QUIZ&&value)
            _btMisc->setVisible(true);
        else
            _btMisc->setVisible(false);
    }

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

void MedNUSLessonIcon::activateMISC() {
    qDebug() << "test";
}

void MedNUSLessonIcon::deleteSelection() {
    qDebug() << "test2";
}

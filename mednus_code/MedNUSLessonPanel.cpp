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

    for(int i=0;i<11;i++) {
        _loadingIcon[i] = QPixmap(QString::fromStdString(":/images/lessonLoading_")+QString::number(i)+QString::fromStdString(".png"));
    }
}


void MedNUSLessonPanel::addLesson(MedNUSLessonPackage * _package) {
    _package->setMode(_currentMode);
    _lessonList.push_back(_package);
    updateGUI();
}

void MedNUSLessonPanel::removeLesson(MedNUSLessonPackage *toDelete)
{
    for(int i=0;i<(int)_lessonList.size();i++) {
        MedNUSLessonPackage *temp = _lessonList.at(i);
        if(temp == toDelete) {
            _lessonList.removeAt(i);
            delete toDelete;
            update();
            updateGUI();
            return;
        }
    }
    qWarning() << "Lesson could not be removed.";
}


void MedNUSLessonPanel::addLesson(QString title,QString subTitle,
                                  QString description, QStringList directories, QString storyFile="") {
    MedNUSLessonPackage *_package = new MedNUSLessonPackage(this);
    _package->setTitle(title);
    _package->setSubHeader(subTitle);
    _package->setDescription(description);
    _package->setMode(_currentMode);

    _package->initStoryLine(storyFile);

    for(int i=0;i<directories.size();i++)
    {
        QString directory = directories.at(i);
        fileType filetype;
        if(directory.contains(".png", Qt::CaseInsensitive))
            filetype = fileType::IMAGE;
        //else if(directory.contains(".ply", Qt::CaseInsensitive))
        //    filetype = fileType::MODEL;
        else if(directory.contains(".pdf", Qt::CaseInsensitive))
            filetype = fileType::PDF;
        else if(directory.contains(".qiz", Qt::CaseInsensitive))
            filetype = fileType::QUIZ;
        else if(directory.contains(".mp4", Qt::CaseInsensitive))
            filetype = fileType::VIDEO;
        else
            filetype = fileType::MODEL;

        _package->addContent(directory,filetype);
    }

    _lessonList.push_back(_package);
    updateGUI(); 

    connect(_package, SIGNAL(emitOpenFile(QString,QString,int)),
            this, SLOT(callOpenFile(QString,QString,int)));
    connect(_package, SIGNAL(emitdeleteLesson(MedNUSLessonPackage*)),
            this, SLOT(removeLesson(MedNUSLessonPackage*)));
}


bool MedNUSLessonPanel::removeLesson(QString title) {
    for(int i=0;i<(int)_lessonList.size();i++) {
        MedNUSLessonPackage *temp = _lessonList.at(i);
        if(temp->getTitle().compare(title)) {
            _lessonList.removeAt(i);
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
    int listSize=(int)_lessonList.size();
    for(int i=0;i<listSize;i++) {
        MedNUSLessonPackage *temp = _lessonList.at(i);
        temp->setTone(i);
        temp->setY(offset);
        temp->updateAmtOfLesson(listSize);
        temp->updateGUI(_trayOut);
        offset+=temp->getHeight();
    }
}


void MedNUSLessonPanel::setTrayOut(bool value) {
    _trayOut = value;

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


void MedNUSLessonPanel::callOpenFile(QString str, QString title, int type) {
    emit emitOpenFile(str, title, type);
}


void MedNUSLessonPanel::tabOpened(QString t, QWidget* w) {
    emit tabOpenedSignal(t, w);
}


void MedNUSLessonPanel::tabClosed(QString t) {
    emit tabClosedSignal(t);
}


void MedNUSLessonPanel::resizeEvent(QResizeEvent* event) {
    _background->setGeometry(QRect(SIDEBAR_OFFSET,0, this->width()-SIDEBAR_OFFSET,this->height()));
    _button->setGeometry(QRect(0,this->height()/2-32, SIDEBAR_OFFSET, 64));
}

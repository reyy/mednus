#include "MedNUSLessonPackage.h"
#include "MedNUSLessonPanel.h"

#include <QFontMetrics>
#include <QMessageBox>
#include <QInputDialog>
#include <math.h>

MedNUSLessonPackageContentPanel::MedNUSLessonPackageContentPanel(int x,int y,QWidget *parent) :
    QWidget(parent) {

    _currentMode = NONE;
    this->setGeometry(QRect(0,0,LESSONPANEL_WIDTH-LESSONPANEL_BORDER*3-SIDEBAR_OFFSET,_listOfItems.size()*LESSONPACKAGEITEM_HEIGHT+2));

    _parent=this;
}


MedNUSLessonPackageContentPanel::~MedNUSLessonPackageContentPanel(){
    //QList<QWidget *> widgets = _listOfItems;
        foreach(MedNUSLessonIcon * widget, _listOfItems)
            delete widget;
}


void MedNUSLessonPackageContentPanel::setMode(interfaceMode mode) {
    _currentMode=mode;
}


MedNUSLessonIcon* MedNUSLessonPackageContentPanel::getContentItem(int value) {
    return _listOfItems.at(value);
}


int MedNUSLessonPackageContentPanel::getContentSize() {
    return _listOfItems.size();
}


MedNUSLessonIcon* MedNUSLessonPackageContentPanel::addContent(QString filename, fileType filetype) {
    MedNUSLessonIcon *item = new MedNUSLessonIcon(filename,filetype,_parent);
    item->setMode(_currentMode);
    _listOfItems.push_back(item);
    return item;
}

void MedNUSLessonPackageContentPanel::removeContent(MedNUSLessonIcon *toDelete)
{
    for(int i=0; i<_listOfItems.size(); i++)
        if(_listOfItems.at(i) == toDelete)
        {
            _listOfItems.removeAt(i);
            delete toDelete;
        }
}


void MedNUSLessonPackageContentPanel::clearContent() {
    while(_listOfItems.size()>0) {
        delete _listOfItems.front();
        _listOfItems.pop_front();
    }
}


void MedNUSLessonPackageContentPanel::updateGUI(int x, int y, bool collapse,int amtOfLesson) {
    if(_currentMode==STUDENT) {
        this->setGeometry(QRect(0,0,LESSONPANEL_WIDTH-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET,_listOfItems.size()*LESSONPACKAGEITEM_HEIGHT+2));
    } else {
        this->setGeometry(QRect(0,0,LESSONPANEL_WIDTH_L-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET,_listOfItems.size()*LESSONPACKAGEITEM_HEIGHT+2));
    }

    if(collapse) {
        for(int i=0;i<_listOfItems.size();i++)
            _listOfItems.at(i)->setVisible(false);
    } else {
        bool exceedLimit=false;
        //Todo:Fix the limit so that scrollbar appears.
        /*if(_listOfItems.size()>LOGO_LIMIT) {
            exceedLimit=true;
        }*/

        //amtOfLesson*LESSONPANEL_CONTRACTED_CLICKHEIGHT;

        for(int i=0;i<_listOfItems.size();i++) {
            _listOfItems.at(i)->setVisible(true);
            _listOfItems.at(i)->updatePosition(x,y,0,5+i*LESSONPACKAGEITEM_HEIGHT);
            _listOfItems.at(i)->setScrollBarSpace(exceedLimit);
        }
    }
}


MedNUSLessonPackage::MedNUSLessonPackage(QWidget *parent) :
    QWidget(parent) {
    _storyMan = NULL;
    _collapse = true;
    _tone=1;
    _x=SIDEBAR_OFFSET;
    _y=0;
    _parent = parent;
    _height = LESSONPANEL_HEIGHT;
    _amtOfLesson = 0;

    this->setBaseSize(LESSONPANEL_WIDTH,_height);
    this->setMinimumWidth(LESSONPANEL_WIDTH);
    this->setStyleSheet("background-color: #ededed;");

    //_currentMode = NONE;

    _background = new QLabel(parent);
    //_background->setPixmap(QPixmap(QString::fromStdString(":/images/copy.png")));

    _loadStatus = new QLabel(parent);
    _loadStatus->setPixmap(((MedNUSLessonPanel*)parent)->getLoadingIcon(10));
    _loadStatus->setScaledContents(true);
    //To Do: Tag the value to load state, set the existing text to 50% opacity.

    _moduleTitle = new QLabel(parent);
    if(_currentMode == STUDENT) {
        _moduleTitle->setStyleSheet("color:#FFFFFF;padding-left: 2px;padding-top: 2px;");
    } else {
        _moduleTitle->setStyleSheet("color:#FFFFFF;padding-left: 2px;padding-top: 2px;background-color:#2e240d;");
    }
    _moduleTitle->setFont (QFont ("Helvetica", 12,QFont::Bold,false));
    _moduleTitle->setGeometry(QRect(_x+9+LESSONPANEL_BORDERICON, _y, LESSONPANEL_WIDTH, 16));

    _subHeader = new QLabel(parent);
    if(_currentMode == STUDENT) {
        _subHeader->setStyleSheet("color:#e5a539;padding-left: 2px;padding-top: 1px;");
    } else {
        _subHeader->setStyleSheet("color:#e5a539;padding-left: 2px;padding-top: 1px;background-color:#2e240d;");
    }
    _subHeader->setFont (QFont ("Helvetica", 10,QFont::Bold,false));
    _subHeader->setGeometry(QRect(_x+9+5+LESSONPANEL_BORDERICON, _y+22+3, LESSONPANEL_WIDTH, 15));

    _description = new QLabel(parent);
    if(_currentMode == STUDENT) {
        _description->setStyleSheet("color:#bacdda;padding-left: 2px;padding-top: 1px;");
    } else {
        _description->setStyleSheet("color:#bacdda;padding-left: 2px;padding-top: 1px;background-color:#2e240d;");
    }
    _description->setFont (QFont ("Helvetica", 10,QFont::Normal,true));
    _description->setGeometry(QRect(_x+9+5+LESSONPANEL_BORDERICON, _y+40+3, LESSONPANEL_WIDTH, 15));

    _contentPanel = new MedNUSLessonPackageContentPanel(_x,_y,parent);

    _scrollArea = new QScrollArea(parent);
    _scrollArea->setWidgetResizable(false);
    _scrollArea->setContentsMargins(0,0,0,0);
    _scrollArea->setGeometry(_contentPanel->geometry());
    _scrollArea->setWidget(_contentPanel);
    _scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded);

    //Load scrollbar style.
    QFile file(":/images/lessonpanel.css");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        _scrollArea->setStyleSheet(file.readAll());
        file.close();
    } 

    _btEditTitle = new QPushButton(parent);
    _btEditTitle->setIconSize(QSize(16,16));
    _btEditTitle->setFlat(true);
    _btEditTitle->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_edit.png);} QPushButton::pressed {background-image: url(:/images/bt_edit_p.png);}");
    _btEditTitle->setVisible(false);

    _btEditSubHeader = new QPushButton(parent);
    _btEditSubHeader->setIconSize(QSize(16,16));
    _btEditSubHeader->setFlat(true);
    _btEditSubHeader->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_edit.png);} QPushButton::pressed {background-image: url(:/images/bt_edit_p.png);}");
    _btEditSubHeader->setVisible(false);

    _btEditDescription = new QPushButton(parent);
    _btEditDescription->setIconSize(QSize(16,16));
    _btEditDescription->setFlat(true);
    _btEditDescription->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_edit.png);} QPushButton::pressed {background-image: url(:/images/bt_edit_p.png);}");
    _btEditDescription->setVisible(false);

    connect(_btEditTitle,SIGNAL(clicked()),this,SLOT(editTitle()));
    connect(_btEditSubHeader,SIGNAL(clicked()),this,SLOT(editSubHeader()));
    connect(_btEditDescription,SIGNAL(clicked()),this,SLOT(editDescription()));

    _btUpload = new QPushButton(parent);
    _btUpload->setIconSize(QSize(24,24));
    _btUpload->setFlat(true);
    _btUpload->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_upload.png);} QPushButton::pressed {background-image: url(:/images/bt_upload_p.png);}");
    _btUpload->setVisible(false);

    _btNewQuiz = new QPushButton(parent);
    _btNewQuiz->setIconSize(QSize(24,24));
    _btNewQuiz->setFlat(true);
    _btNewQuiz->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_addquiz.png);} QPushButton::pressed {background-image: url(:/images/bt_addquiz_p.png);}");
    _btNewQuiz->setVisible(false);

    _btDelete = new QPushButton(parent);
    _btDelete->setIconSize(QSize(24,24));
    _btDelete->setFlat(true);
    _btDelete->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/bt_delete.png);} QPushButton::pressed {background-image: url(:/images/bt_delete_p.png);}");
    _btDelete->setVisible(false);

    connect(_btUpload,SIGNAL(clicked()),this,SLOT(locateNewFile()));
    connect(_btNewQuiz,SIGNAL(clicked()),this,SLOT(addNewQuiz()));
    connect(_btDelete,SIGNAL(clicked()),this,SLOT(deleteLesson()));
}


MedNUSLessonPackage::~MedNUSLessonPackage() {
    SAFE_DELETE(_contentPanel);
    SAFE_DELETE(_loadStatus);
    SAFE_DELETE(_background);
    delete _moduleTitle;
    delete _subHeader;
    delete _description;
    delete _scrollArea;
    SAFE_DELETE(_storyMan);
    SAFE_DELETE(_btEditTitle);
    SAFE_DELETE(_btEditSubHeader);
    SAFE_DELETE(_btEditDescription);

    SAFE_DELETE(_btUpload);
    SAFE_DELETE(_btNewQuiz);
    SAFE_DELETE(_btDelete);
}


void MedNUSLessonPackage::setMode(interfaceMode mode) {
    _currentMode=mode;
    _contentPanel->setMode(_currentMode);

    if(_currentMode==STUDENT) {
        QFile file(":/images/lessonpanel.css");
        if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
            _scrollArea->setStyleSheet(file.readAll());
            file.close();
        }
    } else {
        QFile file(":/images/lessonpanel2.css");
        if(file.open(QIODevice::ReadOnly|QIODevice::Text)) {
            _scrollArea->setStyleSheet(file.readAll());
            file.close();
        }
    }
}


void MedNUSLessonPackage::setY(int value) {
    _y=value;
}


int MedNUSLessonPackage::getY() {
    return _y;
}

bool MedNUSLessonPackage::initStoryLine(QString storyFile)
{
    try
    {
        _storyMan = new MedNUSStoryManager(storyFile);
        MedNUSLessonIcon *item = _contentPanel->addContent(storyFile+"/Start Lesson Mode",fileType::GENERIC);
        connect(this->parent(), SIGNAL(tabClosedSignal(QString)), item, SLOT(tabClosed(QString)));
        connect(this->parent(), SIGNAL(tabOpenedSignal(QString,QWidget*)), item, SLOT(tabOpened(QString,QWidget*)));
        connect(item, SIGNAL(emitOpenFile(QString,QString,int)), _storyMan, SLOT(playStory()));
    }
    catch(bool err)
    {
        if(_storyMan != NULL)
        {
            delete _storyMan;
            _storyMan = NULL;
        }
        return false;
    }

    return true;
}


void MedNUSLessonPackage::addContent(QString filename, fileType filetype) {
    MedNUSLessonIcon *item = _contentPanel->addContent(filename,filetype);
    connect(this->parent(), SIGNAL(tabClosedSignal(QString)), item, SLOT(tabClosed(QString)));
    connect(this->parent(), SIGNAL(tabOpenedSignal(QString, QWidget*)), item, SLOT(tabOpened(QString, QWidget*)));
    connect(item, SIGNAL(emitOpenFile(QString,QString,int)), this, SLOT(callOpenFile(QString,QString,int)));
    connect(item, SIGNAL(emitDeleteFile(MedNUSLessonIcon*)), this, SLOT(deleteFile(MedNUSLessonIcon*)));

    if(_storyMan)
        _storyMan->checkAddedItem(filename, filetype, item);
}


void MedNUSLessonPackage::clearContent() {
    _contentPanel->clearContent();
}


int MedNUSLessonPackage::getContentSize() {
    return _contentPanel->getContentSize();
}


MedNUSLessonIcon* MedNUSLessonPackage::getContentItem(int value) {
    return _contentPanel->getContentItem(value);
}


void MedNUSLessonPackage::setTitle(QString value) {
    _title = value;
    QFontMetrics metrics1(_moduleTitle->font());
    _moduleTitle->setText(metrics1.elidedText(value, Qt::ElideRight, _moduleTitle->width()));
}


QString MedNUSLessonPackage::getTitle() {
    return _title;
}


void MedNUSLessonPackage::setSubHeader(QString value) {
    QFontMetrics metrics2(_subHeader->font());
    _subHeader->setText(metrics2.elidedText(value, Qt::ElideRight, _subHeader->width()));
}


void MedNUSLessonPackage::setDescription(QString value) {

    QFontMetrics metrics3(_description->font());
    _description->setText(metrics3.elidedText(value, Qt::ElideRight, _description->width()));
}


void MedNUSLessonPackage::setTone(int value) {
    _tone=value;
}


bool MedNUSLessonPackage::getCollapse() {
    return _collapse;
}


int MedNUSLessonPackage::getHeight(){
    if(_collapse)
        return LESSONPANEL_CONTRACTED_CLICKHEIGHT;
    else
        return _height;
}


int MedNUSLessonPackage::getInteractiveHeight(){
    if(_collapse)
        return LESSONPANEL_CONTRACTED_CLICKHEIGHT;
    else
        return LESSONPANEL_CLICKHEIGHT;
}


void MedNUSLessonPackage::toggleCollapse() {
    _collapse=!_collapse;

    _height = MAX(LESSONPANEL_CLICKHEIGHT+LESSONPANEL_BORDER*2+LESSONPACKAGEITEM_HEIGHT*_contentPanel->getContentSize(),LESSONPANEL_HEIGHT);
    _contentPanel->updateGUI(_x,_y,_collapse,_amtOfLesson);
}


void MedNUSLessonPackage::toggleCollapse(bool value) {
    _collapse=value;
    _contentPanel->updateGUI(_x,_y,_collapse,_amtOfLesson);
}


void MedNUSLessonPackage::updateAmtOfLesson(int amtOfLesson) {
    _amtOfLesson=amtOfLesson;
}


void MedNUSLessonPackage::updateGUI(bool trayOut) {

    _contentPanel->updateGUI(_x,_y,_collapse,_amtOfLesson);

    int offset=0;
    if(trayOut)
        offset+=LESSONPANEL_BORDERICON;


    if(_currentMode==STUDENT) {
        if(_tone%2==0)
            _background->setStyleSheet("background-color: #1c262c;");
        else
            _background->setStyleSheet("background-color: #23313a;");
    } else {
        if(_tone%2==0)
            _background->setStyleSheet("background-color: #493a14;");
        else
            _background->setStyleSheet("background-color: #3f3211;");
    }

    if(_collapse) {
        if(_currentMode==STUDENT) {
            _background->setGeometry(QRect(_x, _y, LESSONPANEL_WIDTH, LESSONPANEL_CONTRACTED_CLICKHEIGHT));
            _loadStatus->setGeometry(QRect(_x+offset*0.5+LESSONPANEL_BORDERICON*0.3, _y+LESSONPANEL_BORDERICON*0.3, LESSONPANEL_BORDERICON, LESSONPANEL_BORDERICON));
            _moduleTitle->setGeometry(QRect(_x+9+offset+LESSONPANEL_BORDERICON, _y+4, LESSONPANEL_WIDTH, 16));
        } else {
            _background->setGeometry(QRect(_x, _y, LESSONPANEL_WIDTH_L, LESSONPANEL_CONTRACTED_CLICKHEIGHT));
            _loadStatus->setGeometry(QRect(_x+offset*0.5+LESSONPANEL_BORDERICON*0.3, _y+LESSONPANEL_BORDERICON*0.3, LESSONPANEL_BORDERICON, LESSONPANEL_BORDERICON));
            _moduleTitle->setGeometry(QRect(_x+9+offset+LESSONPANEL_BORDERICON, _y+4, LESSONPANEL_WIDTH_L, 16));
        }
        _moduleTitle->setStyleSheet("color:#FFFFFF;padding-left: 2px;padding-top: 2px;");

        _scrollArea->setVisible(false);
        _subHeader->setVisible(false);
        _description->setVisible(false);
        _btUpload->setVisible(false);
        _btNewQuiz->setVisible(false);
        _btDelete->setVisible(false);
        _btEditTitle->setVisible(false);
        _btEditSubHeader->setVisible(false);
        _btEditDescription->setVisible(false);
    } else {
        if(_currentMode==STUDENT) {
            _background->setGeometry(QRect(_x, _y, LESSONPANEL_WIDTH, _height));
            _loadStatus->setGeometry(QRect(_x+LESSONPANEL_BORDERICON*0.3, _y+LESSONPANEL_BORDERICON*0.3, LESSONPANEL_BORDERICON, LESSONPANEL_BORDERICON));
            _scrollArea->setGeometry(QRect(_x+LESSONPANEL_BORDER, _y+LESSONPANEL_CLICKHEIGHT, LESSONPANEL_WIDTH-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET, _height-LESSONPANEL_CLICKHEIGHT-LESSONPANEL_BORDER));
        } else  {
            _background->setGeometry(QRect(_x, _y, LESSONPANEL_WIDTH_L, _height));
            _loadStatus->setGeometry(QRect(_x+LESSONPANEL_BORDERICON*0.3, _y+LESSONPANEL_BORDERICON*0.3, LESSONPANEL_BORDERICON, LESSONPANEL_BORDERICON));
            _scrollArea->setGeometry(QRect(_x+LESSONPANEL_BORDER, _y+LESSONPANEL_CLICKHEIGHT, LESSONPANEL_WIDTH_L-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET, _height-LESSONPANEL_CLICKHEIGHT-LESSONPANEL_BORDER));
            _btUpload->setGeometry(QRect(_x+LESSONPANEL_WIDTH_L-(32+5)*3-16-10, _y+LESSONPANEL_CLICKHEIGHT*0.5-12, 32,24));
            _btNewQuiz->setGeometry(QRect(_x+LESSONPANEL_WIDTH_L-(32+5)*2-16-10, _y+LESSONPANEL_CLICKHEIGHT*0.5-12, 32,24));
            _btDelete->setGeometry(QRect(_x+LESSONPANEL_WIDTH_L-(32+5)*1-16-10, _y+LESSONPANEL_CLICKHEIGHT*0.5-12, 32,24));
            _scrollArea->setGeometry(QRect(_x+LESSONPANEL_BORDER, _y+LESSONPANEL_CLICKHEIGHT, LESSONPANEL_WIDTH_L-LESSONPANEL_BORDER*2-SIDEBAR_OFFSET, _height-LESSONPANEL_CLICKHEIGHT-LESSONPANEL_BORDER));
            _btEditTitle->setGeometry(QRect(_x+LESSONPANEL_WIDTH+7, _y+4, 16,16));
            _btEditSubHeader->setGeometry(QRect(_x+LESSONPANEL_WIDTH+7, _y+25, 16,16));
            _btEditDescription->setGeometry(QRect(_x+LESSONPANEL_WIDTH+7, _y+43, 16,16));
            _btUpload->setVisible(true);
            _btNewQuiz->setVisible(true);
            _btDelete->setVisible(true);
            _btEditTitle->setVisible(true);
            _btEditSubHeader->setVisible(true);
            _btEditDescription->setVisible(true);
        }
        _moduleTitle->setStyleSheet("color:#FFFFFF;padding-left: 2px;padding-top: 2px;background-color:#2e240d;");

        _moduleTitle->setGeometry(QRect(_x+9+LESSONPANEL_BORDERICON, _y+4, LESSONPANEL_WIDTH+5-9-LESSONPANEL_BORDERICON, 16));
        _subHeader->setGeometry(QRect(_x+9+5+LESSONPANEL_BORDERICON, _y+22+3, LESSONPANEL_WIDTH-9-LESSONPANEL_BORDERICON, 15));
        _description->setGeometry(QRect(_x+9+5+LESSONPANEL_BORDERICON, _y+40+3, LESSONPANEL_WIDTH-9-LESSONPANEL_BORDERICON, 15));
        _scrollArea->setVisible(true);
        _subHeader->setVisible(true);
        _description->setVisible(true);
    }
}


QString MedNUSLessonPackage::dialogGetString(QString message,QString defaultString) {
    bool ok;
    QString toReturn = QInputDialog::getText(this, tr(""),
                                      message, QLineEdit::Normal,
                                      defaultString, &ok);
    if(ok)
        return toReturn;
    else
        return defaultString;
}


void MedNUSLessonPackage::callOpenFile(QString str, QString title, int type) {
    emit emitOpenFile(str, title, type);
}


void MedNUSLessonPackage::editTitle() {
    _moduleTitle->setText(dialogGetString("Enter the new title :",_moduleTitle->text()));
}


void MedNUSLessonPackage::editSubHeader() {
    _subHeader->setText(dialogGetString("Enter the new sub-header :",_subHeader->text()));
}


void MedNUSLessonPackage::editDescription() {
    _description->setText(dialogGetString("Enter the new description :",_description->text()));
}


void MedNUSLessonPackage::locateNewFile() {
    //To do: Select new file.
}


void MedNUSLessonPackage::addNewQuiz() {
    //Add new quiz.
    QMessageBox msgBox;
    msgBox.setText("Create a quiz");
    msgBox.setInformativeText("Are you sure?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes );
    msgBox.setDefaultButton(QMessageBox::No);

    int ret = msgBox.exec();
    switch (ret) {
       case QMessageBox::Yes:
           //HACK: Opening a dummy file.
        emit emitOpenFile("/mednus/lesson1/makequiz/maker.json", "Quiz Maker", 1);
           break;
       case QMessageBox::No:
           break;
       default:
           // should never be reached
           break;
     }
}


void MedNUSLessonPackage::deleteLesson() {
    //Show dialog and delete.

    QMessageBox msgBox;
    msgBox.setText("Lesson will be deleted permanently.");
    msgBox.setInformativeText("Are you sure?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes );
    msgBox.setDefaultButton(QMessageBox::No);

    int ret = msgBox.exec();
    switch (ret) {
       case QMessageBox::Yes:
           //To do: Delete the lesson.
           this->_collapse=true;
           this->updateGUI(false);
           this->hide();
           emit emitdeleteLesson(this);

           break;
       case QMessageBox::No:
           break;
       default:
           // should never be reached
           break;
    }
}

void MedNUSLessonPackage::deleteFile(MedNUSLessonIcon *toDelete)
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to remove this file?");
    //msgBox.setInformativeText("Are you sure you want to remove this file?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes );
    msgBox.setDefaultButton(QMessageBox::No);

    int ret = msgBox.exec();
    switch (ret) {
       case QMessageBox::Yes:
            _contentPanel->removeContent(toDelete);
            updateGUI(true);
           break;

       case QMessageBox::No:
       default:
           break;
    }

}


#ifndef MEDNUSLESSONPACKAGE_H
#define MEDNUSLESSONPACKAGE_H

#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
#include <QScrollArea>

#include "MedNUSLessonIcon.h"
#include "MedNUSAUISettings.h"
#include "MedNUSStoryManager.h"

class MedNUSLessonPackageContentPanel : public QWidget
{
public:
    explicit MedNUSLessonPackageContentPanel(int x, int y, QWidget *parent = 0);
    ~MedNUSLessonPackageContentPanel();

    void setMode(interfaceMode mode);
    MedNUSLessonIcon* getContentItem(int value);
    int getContentSize();

    MedNUSLessonIcon* addContent(QString filename, fileType filetype);
    void removeContent(MedNUSLessonIcon*);
    void clearContent();

    void updateGUI(int x, int y, bool collapse, int amtOfLesson);

private:
    QWidget *_parent;
    QVector<MedNUSLessonIcon*> _listOfItems;
    interfaceMode _currentMode;
};


class MedNUSLessonPackage : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSLessonPackage(QWidget *parent = 0);
    ~MedNUSLessonPackage();

    void setMode(interfaceMode mode);
    void setY(int value);
    int getY();

    bool initStoryLine(QString storyFile);

    void addContent(QString filename, fileType filetype);
    void clearContent();
    int getContentSize();
    MedNUSLessonIcon* getContentItem(int value);

    void setTitle(QString value);
    QString getTitle();
    void setSubHeader(QString value);
    void setDescription(QString value);
    void setTone(int value);
    int getInteractiveHeight();
    bool getCollapse();
    int getHeight();
    void toggleCollapse();
    void toggleCollapse(bool value);
    void updateAmtOfLesson(int amtOfLesson);
    void updateGUI(bool trayOut);
    QString dialogGetString(QString message,QString defaultString);

private:
    int _x;
    int _y;
    int _tone;
    int _amtOfLesson;
    float _height;

    QString _title;
    QLabel *_loadStatus = NULL;
    QLabel *_background = NULL;
    QLabel *_moduleTitle = NULL;
    QLabel *_subHeader = NULL;
    QLabel *_description = NULL;
    bool _collapse;
    QWidget *_parent = NULL;
    QScrollArea *_scrollArea = NULL;
    MedNUSLessonPackageContentPanel *_contentPanel = NULL;
    MedNUSStoryManager *_storyMan = NULL;
    interfaceMode _currentMode;

    QPushButton *_btEditTitle = NULL;
    QPushButton *_btEditSubHeader = NULL;
    QPushButton *_btEditDescription = NULL;

    QPushButton *_btUpload = NULL;
    QPushButton *_btNewQuiz = NULL;
    QPushButton *_btDelete = NULL;

public slots:
    void callOpenFile(QString str, QString title, int type);

protected slots:
    void editTitle();
    void editSubHeader();
    void editDescription();

    void locateNewFile();
    void addNewQuiz();
    void deleteLesson();
    void deleteFile(MedNUSLessonIcon*);

signals:
    void emitOpenFile(QString, QString, int);
    void emitdeleteLesson(MedNUSLessonPackage*);
};


#endif // MEDNUSLESSONPACKAGE_H

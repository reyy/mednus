#ifndef MEDNUSLESSONPACKAGE_H
#define MEDNUSLESSONPACKAGE_H

#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
#include <QScrollArea>

#include "MedNUSLessonIcon.h"
#include "MedNUSAUISettings.h"

class MedNUSLessonPackageContentPanel : public QWidget
{
public:
    explicit MedNUSLessonPackageContentPanel(int x, int y, QWidget *parent = 0);
    ~MedNUSLessonPackageContentPanel();

    void setMode(interfaceMode mode);
    MedNUSLessonIcon* getContentItem(int value);
    int getContentSize();

    MedNUSLessonIcon* addContent(QString filename, fileType filetype);
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
    QLabel *_loadStatus;
    QLabel *_background;
    QLabel *_moduleTitle;
    QLabel *_subHeader;
    QLabel *_description;
    bool _collapse;
    QWidget *_parent;
    QScrollArea *_scrollArea;
    MedNUSLessonPackageContentPanel *_contentPanel;
    interfaceMode _currentMode;

    QPushButton *_btEditTitle;
    QPushButton *_btEditSubHeader;
    QPushButton *_btEditDescription;

    QPushButton *_btUpload;
    QPushButton *_btNewQuiz;
    QPushButton *_btDelete;

public slots:
    void callOpenFile(QString str, QString title, int type);

protected slots:
    void editTitle();
    void editSubHeader();
    void editDescription();

    void locateNewFile();
    void addNewQuiz();
    void deleteLesson();

signals:
    void emitOpenFile(QString, QString, int);
};


#endif // MEDNUSLESSONPACKAGE_H

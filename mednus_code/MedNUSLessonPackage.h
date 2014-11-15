#ifndef MEDNUSLESSONPACKAGE_H
#define MEDNUSLESSONPACKAGE_H


#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
#include <QScrollArea>
#include "MedNUSLessonIcon.h"

class MedNUSLessonPackageContentPanel : public QWidget
{
    QWidget *_parent;
    QVector<MedNUSLessonIcon*> _listOfItems;

public:
    explicit MedNUSLessonPackageContentPanel(int x, int y,QWidget *parent = 0);
    ~MedNUSLessonPackageContentPanel();
    MedNUSLessonIcon* getContentItem(int value);
    int getContentSize();
    MedNUSLessonIcon* addContent(QString filename, QPixmap directory);
    void clearContent();
    void updateGUI(int x, int y, bool collapse);
};

class MedNUSLessonPackage : public QWidget
{
    Q_OBJECT
private:
    int _x;
    int _y;
    int _tone;

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

public:
    explicit MedNUSLessonPackage(QWidget *parent = 0);
    ~MedNUSLessonPackage();
    void setY(int value);
    int getY();

    void addContent(QString filename, QPixmap directory);
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
    void updateGUI();

public slots:
    void callOpenFile(QString str, QString title, int type)
    {emit emitOpenFile(str, title, type);}
signals:
    void emitOpenFile(QString, QString, int);
};

#endif // MEDNUSLESSONPACKAGE_H

#ifndef MEDNUSLESSONPACKAGE_H
#define MEDNUSLESSONPACKAGE_H


#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
#include "MedNUSLessonIcon.h"

class MedNUSLessonPackage : public QWidget
{
    Q_OBJECT
private:
    int _x;
    int _y;
    int _tone;
    int _height;
    int _interactiveHeight;
    int _collapsedHeight;
    QString _title;
    QLabel *_background;
    QLabel *_contentBackground;
    QLabel *_moduleTitle;
    QLabel *_subHeader;
    QLabel *_description;
    bool _collapse;
    QWidget *_parent;
    QVector<MedNUSLessonIcon*> _listOfItems;

public:
    explicit MedNUSLessonPackage(QWidget *parent = 0);
    ~MedNUSLessonPackage();
    void setY(int value);
    int getY();
    MedNUSLessonIcon* getContentItem(int value);
    int getContentSize();
    void addContent(QString filename, QPixmap directory);
    void clearContent();
    void setTitle(QString value);
    QString getTitle();
    void setSubHeader(QString value);
    void setDescription(QString value);
    void setTone(int value);
    int getInteractiveHeight();
    int getHeight();
    int getCollapseHeight();
    void toggleCollapse();
    void toggleCollapse(bool value);
    void updateGUI();

public slots:

};

#endif // MEDNUSLESSONPACKAGE_H

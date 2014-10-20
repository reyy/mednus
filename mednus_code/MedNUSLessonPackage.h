#ifndef MEDNUSLESSONPACKAGE_H
#define MEDNUSLESSONPACKAGE_H


#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>

class MedNUSLessonPackage : public QWidget
{
    Q_OBJECT
private:
    int _y;
    int _tone;
    int _height;
    int _collapsedHeight;
    QString _title;
    QLabel *_background;
    QLabel *_moduleTitle;
    QLabel *_subHeader;
    QLabel *_description;
    bool _collapse;

public:
    explicit MedNUSLessonPackage(QWidget *parent = 0);
    ~MedNUSLessonPackage();
    void setY(int value);
    int getY();
    void setTitle(QString value);
    QString getTitle();
    void setSubHeader(QString value);
    void setDescription(QString value);
    void setTone(int value);
    int getHeight();
    int getCollapseHeight();
    void toggleCollapse();
    void toggleCollapse(bool value);
    void updateGUI();

public slots:

};

#endif // MEDNUSLESSONPACKAGE_H

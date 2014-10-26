#ifndef MedNUSLessonIcon_H
#define MedNUSLessonIcon_H

#include <QLabel>

class MedNUSLessonIcon
{
private:
    bool _selected;
    QLabel *_highlight;
    QLabel *_icon;
    QLabel *_text;
    QPixmap _directory;    
    QString _path;

    float _x;
    float _y;

public:
     MedNUSLessonIcon(QString path, QPixmap directory, QWidget *parent);
    ~MedNUSLessonIcon();
     void updatePosition(float x, float y);
     void setSelected(bool value);
     void setVisible(bool value);
     bool checkMouseClick(float xpos, float ypox);

signals:

public slots:

};

#endif // MedNUSLessonIcon_H

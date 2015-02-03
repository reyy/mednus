#ifndef MedNUSLessonIcon_H
#define MedNUSLessonIcon_H

#include <QLabel>
#include <QMouseEvent>
#include "MedNUSAUISettings.h"

class MedNUSLessonIcon : public QWidget
{
    Q_OBJECT
private:
    bool _scrollBarExist;
    bool _selected;
    QLabel *_highlight;
    QLabel *_icon;
    QLabel *_text;
    QPixmap _directory;    
    QString _path;
    QString _filename;
    QWidget *_parent;

    float _x;
    float _y;

    interfaceMode _currentMode;

public:
     MedNUSLessonIcon(QString path, QPixmap directory, QWidget *parent);
    ~MedNUSLessonIcon();
     void setMode(interfaceMode mode);
     void updatePosition(float packageX, float packageY, float x, float y);
     void setSelected(bool value);
     void setVisible(bool value);
     void setScrollBarSpace(bool value);

protected:
    void setHighlight(bool status);
    void mousePressEvent ( QMouseEvent * event ) ;

signals:
    void emitOpenFile(QString, QString, int);

public slots:
    void tabOpened(QString);
    void tabClosed(QString);
};

#endif // MedNUSLessonIcon_H

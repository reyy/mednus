#ifndef MEDNUSUSERBAR_H
#define MEDNUSUSERBAR_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QPushButton>
#include "MedNUSAUISettings.h"

class MedNUSUserBar : public QWidget
{
    Q_OBJECT
private:
    QLabel *_background;
    QLabel *_backgroundLine;
    QLabel *_avatar;
    QLabel *_cutoutAvatar;
    QLabel *_name;
    bool _trayOut;
    interfaceMode _currentMode;

    QPushButton *_btNewLesson;
    QPushButton *_btViewStats;

public:
    explicit MedNUSUserBar(QWidget *parent = 0);
    ~MedNUSUserBar();

    void setName(QString value);
    void setAvatar(QString path);
    void setTrayOut(bool value);
    void setMode(interfaceMode mode);

protected:
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent ( QMouseEvent * event ) ;

public slots:
    void showContextMenu(const QPoint& pos);
signals:
    void emitLogout();
};


#endif // MEDNUSUSERBAR_H

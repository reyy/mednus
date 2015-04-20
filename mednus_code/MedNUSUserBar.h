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
public:
    explicit MedNUSUserBar(QWidget *parent = 0);
    ~MedNUSUserBar();

    void setName(QString value);
    void setAvatar(QString path);
    void setTrayOut(bool value);
    void setMode(interfaceMode mode);

protected:
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    QLabel *_background;
    QLabel *_backgroundLine;
    QLabel *_avatar;
    QLabel *_cutoutAvatar;
    QLabel *_name;
    bool _trayOut;
    interfaceMode _currentMode;

    QPushButton *_btNewLesson;

public slots:
    void showContextMenu(const QPoint& pos);

protected slots:
    void createNewLesson();

signals:
    void emitLogout();
};


#endif // MEDNUSUSERBAR_H

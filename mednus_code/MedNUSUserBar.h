#ifndef MEDNUSUSERBAR_H
#define MEDNUSUSERBAR_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>

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

public:
    explicit MedNUSUserBar(QWidget *parent = 0);
    ~MedNUSUserBar();

    void setName(QString value);
    void setAvatar(QString path);
    void setTrayOut(bool value);

protected:
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent ( QMouseEvent * event ) ;

public slots:
    void showContextMenu(const QPoint& pos);
signals:
    void emitLogout();
};


#endif // MEDNUSUSERBAR_H

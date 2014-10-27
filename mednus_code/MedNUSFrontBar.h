#ifndef MEDNUSFRONTBAR_H
#define MEDNUSFRONTBAR_H

#include <QWidget>
#include <QLabel>
#include <QDebug>

class MedNUSFrontBar : public QWidget
{
    Q_OBJECT
private:
    QLabel *_background;
    QLabel *_logo;
    QLabel *_avatar;
    QLabel *_cutoutAvatar;
    QLabel *_name;
    bool _trayOut;

public:
    explicit MedNUSFrontBar(QWidget *parent = 0);
    ~MedNUSFrontBar();

    void setName(QString value);
    void setAvatar(QString path);
    void setTrayOut(bool value);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // MEDNUSFRONTBAR_H

#ifndef MEDNUSUSERBAR_H
#define MEDNUSUSERBAR_H

#include <QWidget>
#include <QLabel>
#include <QDebug>

class MedNUSUserBar : public QWidget
{
    Q_OBJECT
private:
    QLabel *_background;
    QLabel *_logo;
    bool _trayOut;

public:
    explicit MedNUSUserBar(QWidget *parent = 0);
    ~MedNUSUserBar();
    void setTrayOut(bool value);

protected:
    void resizeEvent(QResizeEvent* event);

};

#endif // MEDNUSUSERBAR_H

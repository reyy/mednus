#ifndef MEDNUSFRONTBAR_H
#define MEDNUSFRONTBAR_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QPushButton>

class MedNUSFrontBar : public QWidget
{
    Q_OBJECT
private:
    QLabel *_background;
    QLabel *_logo;
    bool _trayOut;

    QPushButton *_btView1;
    QPushButton *_btView2;
    QPushButton *_btView3;

public:
    explicit MedNUSFrontBar(QWidget *parent = 0);
    ~MedNUSFrontBar();

    void setTrayOut(bool value);

signals:

public slots:
    void changeFullScreenView();
    void changeLessonView();
    void changeDefaultView();

protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // MEDNUSFRONTBAR_H

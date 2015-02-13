#ifndef MEDNUSFRONTBAR_H
#define MEDNUSFRONTBAR_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QPushButton>
#include "MedNUSAUISettings.h"

class MedNUSFrontBar : public QWidget
{
    Q_OBJECT
private:
    QLabel *_logo;
    bool _trayOut;
    interfaceMode _currentMode;

    QPushButton *_btView1;
    QPushButton *_btView2;
    QPushButton *_btView3;

public:
    explicit MedNUSFrontBar(QWidget *parent = 0);
    ~MedNUSFrontBar();

    void setTrayOut(bool value);
    void setMode(interfaceMode mode);

signals:
    void toggleLayout(int);

protected slots:
    void changeFullScreenView();
    void changeLessonView();
    void changeDefaultView();

protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // MEDNUSFRONTBAR_H

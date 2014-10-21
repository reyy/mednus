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

public:
    explicit MedNUSFrontBar(QWidget *parent = 0);
    ~MedNUSFrontBar();

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // MEDNUSFRONTBAR_H

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

public:
    explicit MedNUSUserBar(QWidget *parent = 0);
    ~MedNUSUserBar();

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent* event);

};

#endif // MEDNUSUSERBAR_H

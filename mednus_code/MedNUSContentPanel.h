#ifndef MEDNUSCONTENTPANEL_H
#define MEDNUSCONTENTPANEL_H

#include <QTabWidget>

class MedNUSContentPanel : public QTabWidget
{
    Q_OBJECT
public:
    explicit MedNUSContentPanel(QWidget *parent = 0);
    ~MedNUSContentPanel();

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent* event);
};
#endif // MEDNUSCONTENTPANEL_H

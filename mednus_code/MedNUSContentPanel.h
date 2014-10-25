#ifndef MEDNUSCONTENTPANEL_H
#define MEDNUSCONTENTPANEL_H

#include <QTabWidget>
#include <QGridLayout>

class MedNUSContentPanel : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSContentPanel(QWidget *parent = 0);
    ~MedNUSContentPanel();

    QTabWidget tabList[4];
    QGridLayout *layout;
    int counter =2;
    void addTab(QWidget*,QString);
signals:

public slots:

};
#endif // MEDNUSCONTENTPANEL_H

#ifndef MEDNUSCONTENTPANEL_H
#define MEDNUSCONTENTPANEL_H

#include <QTabWidget>
#include <QGridLayout>

class MedNUSTab : public QTabWidget
{
    Q_OBJECT
public:
    explicit MedNUSTab(QWidget *parent = 0)
    {connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));}
    ~MedNUSTab(){}
signals:
    void noMoreTabs(MedNUSTab*);
 public slots:
    void closeTab(int index)
    {
        this->removeTab(index);
        if(this->count()==0)
            emit noMoreTabs(this);
    }

};

class MedNUSContentPanel : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSContentPanel(QWidget *parent = 0);
    ~MedNUSContentPanel();

    MedNUSTab tabList[4];
    QGridLayout *layout;
    int counter =2;
    void addTab(QWidget*,QString);
signals:

public slots:
    void closeTab(MedNUSTab *index);
};


#endif // MEDNUSCONTENTPANEL_H

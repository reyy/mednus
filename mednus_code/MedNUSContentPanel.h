#ifndef MEDNUSCONTENTPANEL_H
#define MEDNUSCONTENTPANEL_H

#include "MedNUSMeshViewer.h"
#include "MedNUSPdfViewer.h"
#include "MedNUSVideoViewer.h"
#include "MedNUSQuiz.h"
#include <QTabWidget>
#include <QGridLayout>

#define VIDEO_INDEX 0
#define PDF_INDEX 1
#define MESH_INDEX 2

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
        delete this->widget(index);
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

    MedNUSTab tabList[3];
    QGridLayout *layout;
    int counter =2;

signals:

public slots:
    void closeTab(MedNUSTab *index);
    void addTab(QWidget*,QString);
};


#endif // MEDNUSCONTENTPANEL_H

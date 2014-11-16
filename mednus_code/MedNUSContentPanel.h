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
    explicit MedNUSTab(QWidget *parent = 0);
    ~MedNUSTab(){}
signals:
    void noMoreTabs(MedNUSTab*);
    void tabClosedSignal(QString);
 public slots:
    void closeTab(int index);

};

class MedNUSContentPanel : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSContentPanel(QWidget *parent = 0);
    ~MedNUSContentPanel();

    MedNUSTab *tabList[3];
    QGridLayout *layout;
    int viewType = 3;
    int rotateCount = 0;

signals:
    void tabClosedSignal(QString);
    void tabOpenedSignal(QString);
public slots:
    void tabClosed(QString t){emit tabClosedSignal(t);}
    void closeTab(MedNUSTab *index);
    void addTab(QWidget*, QString, QString dir);

    void toggleView(int);
};


#endif // MEDNUSCONTENTPANEL_H

#ifndef MEDNUSCONTENTPANEL_H
#define MEDNUSCONTENTPANEL_H

#include <QTabWidget>
#include <QGridLayout>

#include "MedNUSMeshViewer.h"
#include "MedNUSPdfViewer.h"
#include "MedNUSVideoViewer.h"
#include "MedNUSQuiz.h"
#include "MedNUSAUISettings.h"

class MedNUSTab : public QTabWidget
{
    Q_OBJECT
public:
    explicit MedNUSTab(QWidget *parent = 0);
    ~MedNUSTab() {}

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

    void setMode(interfaceMode mode);

protected:
    MedNUSTab *tabList[3];
    QGridLayout *layout;
    int viewType = 0;
    int rotateCount = 0;

private:
    interfaceMode _currentMode;

signals:
    void tabClosedSignal(QString);
    void tabOpenedSignal(QString, QWidget*);

public slots:
    void tabClosed(QString t){emit tabClosedSignal(t);}
    void closeTab(MedNUSTab *index);
    void addTab(QWidget*, QString, QString dir);
    void toggleView(int);
};


#endif // MEDNUSCONTENTPANEL_H

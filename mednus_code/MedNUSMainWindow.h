#ifndef MEDNUSMAINWINDOW_H
#define MEDNUSMAINWINDOW_H

#include <QMainWindow>
#include "MedNUSMeshViewer.h"
#include "MedNUSLessonPanel.h"
#include "MedNUSVideoViewer.h"

class MedNUSMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MedNUSMainWindow(QWidget *parent = 0);
protected:
    QTabWidget *tabs;
    MedNUSVideoViewer *vid;
    void createWidgets();
    void createMenus();

signals:

public slots:

};

#endif // MEDNUSMAINWINDOW_H

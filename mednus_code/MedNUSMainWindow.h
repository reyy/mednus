#ifndef MEDNUSMAINWINDOW_H
#define MEDNUSMAINWINDOW_H

#include <QMainWindow>
#include "MedNUSMeshViewer.h"
#include "MedNUSLessonPanel.h"
#include "MedNUSVideoViewer.h"
#include "MedNUSPdfViewer.h"
#include "MedNUSFrontBar.h"
#include "MedNUSUserBar.h"
#include "MedNUSContentPanel.h"

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

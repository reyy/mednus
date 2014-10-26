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
private:
    bool _trayOut;

public:
    explicit MedNUSMainWindow(QWidget *parent = 0);
protected:
    MedNUSContentPanel *tabs;
    MedNUSVideoViewer *vid;
    MedNUSUserBar *ub;
    MedNUSFrontBar *fb;
    MedNUSLessonPanel *lp;
    void createWidgets();
    void createMenus();

signals:

public slots:

protected:
    void mousePressEvent ( QMouseEvent * event ) ;

};

#endif // MEDNUSMAINWINDOW_H

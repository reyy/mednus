#ifndef MEDNUSMAINWINDOW_H
#define MEDNUSMAINWINDOW_H

#include <QMainWindow>
#include "MedNUSMeshViewer.h"
#include "MedNUSLessonPanel.h"
#include "MedNUSVideoViewer.h"
#include "MedNUSPdfViewer.h"
#include "MedNUSQuiz.h"
#include "MedNUSFrontBar.h"
#include "MedNUSUserBar.h"
#include "MedNUSContentPanel.h"
#include "MedNUSContentManager.h"
#include "MedNUSLogin.h"
#include "MedNUSNetwork.h"

class MedNUSMainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QPixmap _image;
    bool _trayOut;
    bool _widgetsCreated;
    bool _menuCreated;

public:
    explicit MedNUSMainWindow(QWidget *parent = 0);
    ~MedNUSMainWindow();

protected:
    MedNUSContentPanel *tabs;
    MedNUSVideoViewer *vid;
    MedNUSFrontBar *fb;
    MedNUSUserBar *ub;
    MedNUSLessonPanel *lp;
    MedNUSLogin *login;
    MedNUSNetwork *network;

    QString matricNo;
    QString userName;

    void createWidgets();
    void createMenus();
    void deleteWidgets();
    void deleteMenus();

    void paintEvent(QPaintEvent *);

signals:

public slots:
    void loginCompleted(bool,QString,QString);
    void logout();
protected:
    void mousePressEvent ( QMouseEvent * event ) ;

};

#endif // MEDNUSMAINWINDOW_H

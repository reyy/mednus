/* MedNUSLessonPanel.h

   Table widget for lesson plan organization.

   Author: Jay Chua
*/
#ifndef MEDNUSLESSONPANEL_H
#define MEDNUSLESSONPANEL_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QTableWidget>
#include <QItemDelegate>
#include <QColorDialog>
#include <QKeyEvent>
#include <QWidget>
#include <QVector>

#include "MedNUSLessonPackage.h"
#include "MedNUSAUISettings.h"

class MedNUSLessonPanel : public QWidget
{
    Q_OBJECT
public:
    MedNUSLessonPanel(QWidget *parent);
    ~MedNUSLessonPanel();

    void addLesson(MedNUSLessonPackage*);
    bool removeLesson(QString title);
    void clearLesson();

    void updateGUI();
    void setTrayOut(bool value);
    bool checkTray();
    QPixmap getLoadingIcon(int range);
    void setMode(interfaceMode mode);

protected:
    void mousePressEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    interfaceMode _currentMode;
    bool _trayOut;

    QPixmap _button_toopen;
    QPixmap _button_toclose;
    QPixmap _button_toopen2;
    QPixmap _button_toclose2;
    QPixmap _loadingIcon[11];

    QLabel *_background;
    QLabel *_button;
    QVector<MedNUSLessonPackage*> _lessonList;

    void loadPixmap();

public slots:
    void addLesson(QString title, QString subTitle, QString description, QStringList directory, QString storyFile);
    void callOpenFile(QString str, QString title, int type);

    void tabClosed(QString t);
    void tabOpened(QString t);
signals:
    void emitOpenFile(QString, QString, int);
    void tabClosedSignal(QString);
    void tabOpenedSignal(QString);
};


#endif // MEDNUSLESSONPANEL_H

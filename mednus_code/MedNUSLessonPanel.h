/* MedNUSLessonPanel.h

   Table widget for lesson plan organization.

   Author: Jay Chua
*/

#ifndef MEDNUSLESSONPANEL_H
#define MEDNUSLESSONPANEL_H

//class QPushButton;

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

class MedNUSLessonPanel : public QWidget
{
    Q_OBJECT
private:
    QLabel *_background;
    QVector<MedNUSLessonPackage *> _lessonList;

public:
    MedNUSLessonPanel(QWidget *parent);
    ~MedNUSLessonPanel();
    void addLesson(QString title,QString subTitle, QString description);
    bool removeLesson(QString title);
    void clearLesson();
    void updateGUI();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;

public slots:
};

#endif // MEDNUSLESSONPANEL_H

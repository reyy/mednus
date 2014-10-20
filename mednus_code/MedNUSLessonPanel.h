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
#include "MedNUSLessonPackage.h"

class MedNUSLessonPanel : public QWidget
{
    Q_OBJECT

private:
    MedNUSLessonPackage *_package;

public:
    MedNUSLessonPanel(QWidget *parent);
    ~MedNUSLessonPanel();
};

#endif // MEDNUSLESSONPANEL_H

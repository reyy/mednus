/* MedNUSLessonPanel.h

   Table widget for lesson plan organization.

   Author: Jay Chua
*/

#ifndef MEDNUSLESSONPANEL_H
#define MEDNUSLESSONPANEL_H

class QPushButton;

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QTableWidget>
#include <QItemDelegate>
#include <QColorDialog>
#include <QKeyEvent>

class MedNUSLessonPanel : public QTableWidget
{
public:
    MedNUSLessonPanel();
    ~MedNUSLessonPanel();
};

#endif // MEDNUSLESSONPANEL_H

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
#include <QObject>

class MedNUSLessonPanel : public QTableWidget
{
    Q_OBJECT
public:
    MedNUSLessonPanel();
    ~MedNUSLessonPanel();
};

#endif // MEDNUSLESSONPANEL_H

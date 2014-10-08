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
#include <QItemDelegate>
#include <QColorDialog>
#include <QKeyEvent>
#include <QObject>

class MedNUSLessonPanel : public QWidget
{
    Q_OBJECT

public:
    explicit MedNUSLessonPanel(QWidget *parent = 0);
    ~MedNUSLessonPanel();
};

#endif // MEDNUSLESSONPANEL_H

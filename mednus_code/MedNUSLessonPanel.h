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
    QPixmap _icon_3d;
    QPixmap _icon_image;
    QPixmap _icon_pdf;
    QPixmap _icon_quiz;
    QPixmap _icon_video;
    QPixmap _button_toopen;
    QPixmap _button_toclose;

    QLabel *_background;
    QLabel *_dividerBackground;
    QLabel *_button;
    QVector<MedNUSLessonPackage *> _lessonList;

    bool _trayOut;

    void loadPixmap();

public:
    MedNUSLessonPanel(QWidget *parent);
    ~MedNUSLessonPanel();
    void addLesson(MedNUSLessonPackage*);
    void addLesson(QString title, QString subTitle, QString description, QStringList directory);
    bool removeLesson(QString title);
    void clearLesson();
    void updateGUI();
    void setTrayOut(bool value);
    bool checkTrayButton(float xpos, float ypos);

protected:
    void mousePressEvent ( QMouseEvent * event ) ;
    void resizeEvent(QResizeEvent* event);

public slots:
};

#endif // MEDNUSLESSONPANEL_H

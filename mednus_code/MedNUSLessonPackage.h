#ifndef MEDNUSLESSONPACKAGE_H
#define MEDNUSLESSONPACKAGE_H


#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>

class MedNUSLessonPackage : public QWidget
{
    Q_OBJECT
private:
    QLabel *_background;
    QLabel *_moduleTitle;
    QLabel *_subHeader;
    QLabel *_description;

public:
    explicit MedNUSLessonPackage(QWidget *parent = 0);
    ~MedNUSLessonPackage();

protected:
    void mousePressEvent ( QMouseEvent * event ) ;

public slots:

};

#endif // MEDNUSLESSONPACKAGE_H

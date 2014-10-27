#ifndef MEDNUSQUIZ_H
#define MEDNUSQUIZ_H

#include <QWidget>
#include <QLabel>

class MedNUSQuiz : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSQuiz(QString filename, QWidget *parent = 0);
    ~MedNUSQuiz();

protected:
    QLabel *someText;

signals:

public slots:

};

#endif // MEDNUSQUIZ_H

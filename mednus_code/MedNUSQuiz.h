#ifndef MEDNUSQUIZ_H
#define MEDNUSQUIZ_H

#include <QWidget>
#include <QLabel>
#include <QJsonObject>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QVector>
#include <QString>

class MedNUSQuizQuestion
{
public:
    MedNUSQuizQuestion(QWidget *parent, QVBoxLayout *layout);
    ~MedNUSQuizQuestion();

private:
    QLabel _questionTextLabel;
    QRadioButton* _option1;
};

class MedNUSQuiz : public QWidget
{
    Q_OBJECT

public:
    explicit MedNUSQuiz(QString filename, QWidget *parent = 0);
    ~MedNUSQuiz();

protected:
    QVector<MedNUSQuizQuestion*> *_questionList;

    QGroupBox* _content;
    QVBoxLayout* _layout;

signals:

public slots:

};

#endif // MEDNUSQUIZ_H

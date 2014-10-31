#ifndef MEDNUSQUIZ_H
#define MEDNUSQUIZ_H

#include <QWidget>
#include <QLabel>
#include <QJsonObject>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QVector>
#include <QString>
#include <QMessageBox>
// For debugging
#include<QTextStream>

// MedNUSQuizQuestion is not used for now.
class MedNUSQuizQuestion
{
public:
    MedNUSQuizQuestion(QWidget *parent, QGroupBox *box);
    ~MedNUSQuizQuestion();

private:
    QLabel _questionTextLabel;
    QRadioButton* _option1;
    QButtonGroup* _optionButtonGroup;
};

class MedNUSQuiz : public QWidget
{
    Q_OBJECT

public:
    explicit MedNUSQuiz(QString filename, QWidget *parent = 0);
    ~MedNUSQuiz();


protected:
    //QVector<MedNUSQuizQuestion*> *_questionList;

    QGroupBox* _content;
    QVBoxLayout* _layout;

    QVector<QButtonGroup*> _questionOptionGroup;

    QPushButton* _markButton;
    QVector<int> _correctAnswerList;
    int _score;

    QMessageBox* _scoreMsgBox;

signals:

public slots:
    void markQuiz();
};

#endif // MEDNUSQUIZ_H

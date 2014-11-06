#ifndef MEDNUSQUIZ_H
#define MEDNUSQUIZ_H

#include <QWidget>
#include <QLabel>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QVector>
#include <QString>
#include <QScrollArea>
// For debugging
#include<QTextStream>

class MedNUSQuizQuestion : public QWidget
{
public:
    MedNUSQuizQuestion(QWidget *parent, QVBoxLayout *layout);
    ~MedNUSQuizQuestion();

    void myForceResize();

protected:

private:
    QLabel* _questionTextLabel;
    QButtonGroup* _optionButtonGroup;
};

class MedNUSQuiz : public QWidget
{
    Q_OBJECT

public:
    explicit MedNUSQuiz(QString filename, QWidget *parent = 0);
    ~MedNUSQuiz();

protected:
    QWidget* _tempWidget;
    QVBoxLayout* _layout;
    QScrollArea* _scrollArea;

    QVector<QButtonGroup*> _questionOptionGroup;

    QPushButton* _markButton;
    QVector<int> _correctAnswerList;

    void resizeEvent(QResizeEvent *);

private:
    QVector<MedNUSQuizQuestion*>* _questionList;

signals:

public slots:
    void markQuiz();
};

#endif // MEDNUSQUIZ_H

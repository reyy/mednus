#ifndef MEDNUSQUIZ_H
#define MEDNUSQUIZ_H

// General
#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QString>
#include <QMessageBox>
#include <QTimer>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

// For Graphics
#include <QGridLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QResizeEvent>

// For JSON
#include <QFile>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

// For debugging
#include <QTextStream>
#include <QDebug>

#include "MedNUSQuizStylesheet.h"

class MedNUSQuizQuestion
{
public:

    /*  MedNUSQuizQuestion constructor:
     *      The content that is being passed in contains the question text,
     *      options, and the teacher's comment. It is stored in this format:
     *          content[0]              = question text
     *          content[1..noOfOptions] = options
     *          content[noOfOptions+1]  = teacher's comment
     */
    MedNUSQuizQuestion(QWidget *parent, QGridLayout *layout, int& row, QVector<QString> content, int noOfOptions, bool hasImage = false, QString imageUrl = "");
    ~MedNUSQuizQuestion();

    int getSelectedAnswer() const;
    void highlightAnswer(int correctAnswer, bool showCorrectAnswer) const;
    bool oneOptionSelected() const;

    void setNotice(bool value);

    // On Start Quiz
    void showQuestion() const;

private:
    QLabel* _questionImageLabel;
    QLabel* _questionTextLabel;
    QLabel* _teacherCommentLabel;
    QButtonGroup* _optionButtonGroup;
};

class MedNUSQuiz : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSQuiz(QString filename, QWidget *parent = 0);
    ~MedNUSQuiz();

    int getNoOfCorrectAnswers() const;
    void markQuiz(bool byTimer);

protected:
    QWidget* _tempWidget;
    QGridLayout* _layout;
    QScrollArea* _scrollArea;

    QLabel* _titleTextLabel;
    QLabel* _authorTextLabel;
    QLabel* _instructionTextLabel;
    QLabel* _timedQuizWarningTextLabel;
    QLabel* _warning;
    QLabel* _score;
    QVector<QButtonGroup*> _questionOptionGroup;

    QPushButton* _markButton;
    QVector<int> _correctAnswerList;

    bool _showAnswerFlag;
    bool _showTeacherCommentFlag;

    // JSON
    void writeFile();
    void readFile();

    // Start Screen
    QLabel* _startScreenLabel;
    QPushButton* _startQuizButton;

    // Timer
    bool _hasTimeLimit;
    QTimer* _timer;
    int _timerDuration;
    QLabel* _timerLabel;
    QTimer* _labelUpdateTimer;

    // Attempts
    QLabel* _attemptLabel;

    QLabel* _lastModifiedLabel;
    QLabel* _dummySpace1;

    void resizeEvent(QResizeEvent *event);

private:
    QVector<MedNUSQuizQuestion*>* _questionList;

    QString convertTimeToString(int ms);

    int _noOfCorrectAnswers;

signals:
    // No signals as of yet.

public slots:
    void callMarkQuiz_byButton();
    void callMarkQuiz_byTimer();
    void startQuiz();
    void updateTimerLabel();
};


#endif // MEDNUSQUIZ_H

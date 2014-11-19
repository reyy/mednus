#ifndef MEDNUSQUIZ_H
#define MEDNUSQUIZ_H

// General
#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QString>
#include <QMessageBox>
#include <QTimer>

// For Graphics
#include <QVBoxLayout>
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
    MedNUSQuizQuestion(QWidget *parent, QVBoxLayout *layout, QVector<QString> content, int noOfOptions);
    ~MedNUSQuizQuestion();

    int getSelectedAnswer() const;
    void highlightAnswer(int correctAnswer, bool showCorrectAnswer) const;
    bool oneOptionSelected() const;

    void setNotice(bool value);

    // On Start Quiz
    void showQuestion() const;

protected:

private:
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

protected:
    QWidget* _tempWidget;
    QVBoxLayout* _layout;
    QScrollArea* _scrollArea;

    QLabel* _titleTextLabel;
    QLabel* _instructionTextLabel;
    QLabel* _warning;
    QLabel* _score;
    QVector<QButtonGroup*> _questionOptionGroup;

    QPushButton* _markButton;
    QVector<int> _correctAnswerList;

    bool _showAnswerFlag;
    bool _showTeacherCommentFlag;

    void resizeEvent(QResizeEvent *event);

    // JSON
    void writeFile();
    void readFile();

    // Start Screen
    QLabel* _startScreenLabel;
    QPushButton* _startQuizButton;
    bool initStartScreen();

    // Quiz
    QVBoxLayout* _quizLayout;
    bool initQuiz(QString filename);

    // Timer
    bool _hasTimeLimit;
    QTimer* _timer;
    int _timerDuration;
    QLabel* _timerLabel;
    QTimer* _labelUpdateTimer;

private:
    QVector<MedNUSQuizQuestion*>* _questionList;
    void markQuiz(bool byTimer);

signals:

public slots:
    void callMarkQuiz_byButton();
    void callMarkQuiz_byTimer();
    void startQuiz();
    void updateTimerLabel();
};

#endif // MEDNUSQUIZ_H

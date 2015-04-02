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
#include "MedNUSQuizQuestion.h"


class MedNUSQuiz : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSQuiz(QString filename, QWidget *parent = 0);
    ~MedNUSQuiz();

    int getNoOfCorrectAnswers() const;
    void markQuiz(bool calledByTimer);

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
    void loadQuizFile(QString filename, int &row);

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

    void scrollScreenToTop();

signals:
    // No signals as of yet.

public slots:
    void callMarkQuiz_byButton();
    void callMarkQuiz_byTimer();
    void startQuiz();
    void updateTimerLabel();
};


#endif // MEDNUSQUIZ_H

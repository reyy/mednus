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

// For editor
#include <QComboBox>
#include <QListView>

#include "MedNUSQuizStylesheet.h"
#include "MedNUSQuizQuestion.h"


class MedNUSQuiz : public QWidget
{
    Q_OBJECT

public:
    explicit MedNUSQuiz(QString filename, QWidget *parent = 0);
    ~MedNUSQuiz();

protected:
// Widget Stuff
    QWidget* _tempWidget;
    QGridLayout* _layout;
    QScrollArea* _scrollArea;

    QVector<MedNUSQuizQuestion*>* _questionList;
    bool _isEditorView;

    QWidget* _parent;
    QString _filename;


// Quiz Viewer

public:
    int getNoOfCorrectAnswers() const;
    void markQuiz(bool calledByTimer);

private:
    // General
    QVector<int> _correctAnswerList;
    bool _showAnswerFlag;
    bool _showTeacherCommentFlag;

    void loadQuizFileToViewer(QString filename, int &row);
    void resizeEvent(QResizeEvent *event);
    void initViewerView();
    void deinitViewerView();

    // Start Quiz Screen
    QLabel* _titleTextLabel;
    QLabel* _authorTextLabel;
    QLabel* _instructionTextLabel;
    QLabel* _timedQuizWarningTextLabel;
    QLabel* _warning;
    QLabel* _startScreenLabel;
    QLabel* _attemptLabel;
    QLabel* _lastModifiedLabel;
    QLabel* _dummySpace1;
    QPushButton* _startQuizButton;
    QPushButton* _editQuizButton;

    // Quiz Screen
    QPushButton* _markButton;
    QVector<QButtonGroup*> _questionOptionGroup;
    bool _hasTimeLimit;
    QTimer* _timer;
    int _timerDuration;
    QLabel* _timerLabel;
    QTimer* _labelUpdateTimer;

    QString convertTimeToString(int ms);

    // Results Screen
    QLabel* _score;
    int _noOfCorrectAnswers;

    void scrollScreenToTop();

signals:
    // No signals as of yet.

public slots:
    void callMarkQuiz_byButton();
    void callMarkQuiz_byTimer();
    void startQuiz();
    void updateTimerLabel();
    void goToEditorView();
    void goToViewerView();


// Quiz Editor
private:
    QPushButton* _viewQuizButton;
    QComboBox* _noOfQuestionsDropDownBox;
    QListView* _listView;
    int _noOfQuestions;

    void writeFile();
    void initEditorView();
    void deinitEditorView();
    void loadQuizFileToEditor(QString filename, int &row);

signals:
    // No signals as of yet.

public slots:
    void updateNoOfQuestions();
};


#endif // MEDNUSQUIZ_H

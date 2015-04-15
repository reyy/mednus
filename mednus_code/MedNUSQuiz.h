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
#include "MedNUSAUISettings.h"

class MedNUSQuiz : public QWidget {

public:

    explicit MedNUSQuiz(QString filename, interfaceMode currentMode,
                        QWidget *parent = 0);

    ~MedNUSQuiz();


// Quiz Viewer

    // Button Functions
    void markQuiz(bool calledByTimer);

    // Testing
    int getNoOfCorrectAnswers() const;


private:
    Q_OBJECT

// General

    // Determine if student or teacher mode.
    interfaceMode _currentMode;

    // Widget Stuff
    QWidget* _parent;
    QWidget* _tempWidget;
    QGridLayout* _layout;
    QScrollArea* _scrollArea;
    int _lastRow;

    // Quiz File Stuff
    int _noOfQuestions;
    QString _filename;
    QString _title;
    QString _author;
    QString _instructionText;
    QString _lastModifiedDate;
    bool _showCorrectAnswerFlag;
    bool _showTeacherCommentFlag;
    bool _isEditorView;
    QVector<int> _correctAnswerList;
    QVector<MedNUSQuizQuestion*>* _questionList;

    bool loadQuizFile();
    bool loadQuizFile2();
    void createQuizWidgets();
    void resizeEvent(QResizeEvent *event);


// Quiz Viewer

    void initViewerView();
    void deinitViewerView();

    // Start Quiz Screen
    QWidget* _titleTextLabel;
    QWidget* _authorTextLabel;
    QWidget* _instructionTextLabel;
    QWidget* _timedQuizWarningTextLabel;
    QWidget* _warning;
    QWidget* _startScreenLabel;
    QWidget* _attemptLabel;
    QWidget* _lastModifiedLabel;
    QWidget* _dummySpace1;
    QPushButton* _startQuizButton;
    QPushButton* _editQuizButton;

    // Quiz Screen
    bool _hasTimeLimit;
    QTimer* _timer;
    int _timerDuration;
    QLabel* _timerLabel;
    QTimer* _labelUpdateTimer;
    QPushButton* _markButton;
    QVector<QButtonGroup*> _questionOptionGroup;

    QString convertTimeToString(int ms);

    // Results Screen
    QLabel* _score;
    int _noOfCorrectAnswers;

    void scrollScreenToTop();


// Quiz Editor

    // Widget Stuff
    QPushButton* _viewQuizButton;
    QPushButton* _saveButton;
    QComboBox* _noOfQuestionsDropDownBox;
    QListView* _listView;

    void initEditorView();
    void deinitEditorView();
    void writeQuizFile();

signals:
    // No signals as of yet.

public slots:

    // Quiz Viewer
    void callMarkQuiz_byButton();
    void callMarkQuiz_byTimer();
    void startQuiz();
    void updateTimerLabel();
    void goToEditorView();

    // Quiz Editor
    void updateNoOfQuestions();
    void goToViewerView();
    void saveQuiz();
};


#endif // MEDNUSQUIZ_H

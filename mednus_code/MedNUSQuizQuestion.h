#ifndef MEDNUSQUIZQUESTION_H
#define MEDNUSQUIZQUESTION_H

#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QGridLayout>
#include <QDir>
#include <QDebug>
#include <QComboBox>
#include <QListView>
#include <QLineEdit>
#include <QJsonObject>
#include <QJsonArray>

#include "MedNUSQuizStylesheet.h"

class MedNUSQuizQuestion
{
public:

    enum QuestionMode {
        VIEWER = 0,
        EDITOR,
    };

    /*  MedNUSQuizQuestion constructor (viewer view):
     *      The content that is being passed in contains the question text,
     *      options, and the teacher's comment. It is stored in this format:
     *          content[0]              = question text
     *          content[1..noOfOptions] = options
     *          content[noOfOptions+1]  = teacher's comment
     *          content[noOfOptions+2]  = correct answer (int)
     */
   /* MedNUSQuizQuestion(QWidget *parent, QGridLayout *layout, int& row,
                       QVector<QString> content, int noOfOptions,
                       bool hasImage = false, QString imageUrl = "",
                       int questionNum = -1);*/

    MedNUSQuizQuestion(int questionNum, QVector<QString> content,
                       int noOfOptions, bool hasImage = false,
                       QString imageUrl = "");

    MedNUSQuizQuestion(int questionNum);

    /*  MedNUSQuizQuestion constructor (editor view - new question):
     *      This constructor is meant to be used by the quiz editor to
     *      create new (empty) questions.
     */
    //MedNUSQuizQuestion(QWidget *parent, QGridLayout *layout, int &row);

    /*  MedNUSQuizQuestion constructor (editor view - existing question):
     *      This constructor is meant to be used by the quiz editor to
     *      create new (empty) questions.
     */
    //TODO: copy from the viewer view.
    ~MedNUSQuizQuestion();

    int getSelectedAnswer() const;
    void highlightAnswer(bool showCorrectAnswer) const;
    bool oneOptionSelected() const;

    // Load question into the quiz widget.
    void loadQuestion(QuestionMode mode, QWidget *parent, QGridLayout *layout,
                      int &row);
    void unloadQuestion(QuestionMode prevMode);

    // Set a highlight on text label of question left unanswered.
    void setNotice(bool isAnswered);

    // Called on Start Quiz
    void showQuestion() const;

    // Update the storage variables with the data from QLineEdit.
    bool saveChanges();
    bool saveOptions();
    void writeToFile(QJsonObject &json);

    bool markQuestion();

private:
// For storage
    int _questionNum;
    QString _questionImageUrl;
    QString _questionText;
    QString _teacherComment;
    int _noOfOptions;
    int _correctAnswer;
    QVector<QString>* _options;

// For display
    QLabel*  _questionNumLabel;
    QWidget* _questionImageLabel;
    QWidget* _questionImageLabelEdit;           // Edit mode only.
    QWidget* _questionTextLabel;
    QWidget* _teacherCommentLabel;
    //QLabel* _dummySpace;                        // End of question
    QVector<QLabel*>* _dummySpace;               // Start of question
    int _amtOfDummySpace;
    QButtonGroup* _optionButtonGroup;           // Viewing mode only.
    QVector<QLineEdit*>* _optionsLabelEdit;     // Edit mode only.
    QComboBox* _correctAnswerDropDownBox;
    QListView* _listView;

    void loadQuestionNumLabel(QuestionMode mode, QWidget *parent,
                              QGridLayout *layout, int &row);
    void loadQuestionTextLabel(QuestionMode mode, QWidget *parent,
                               QGridLayout *layout, int &row);
    void loadQuestionImageLabel(QuestionMode mode, QWidget *parent,
                                QGridLayout *layout, int &row);
    void loadQuestionOptions(QuestionMode mode, QWidget *parent,
                             QGridLayout *layout, int &row);
    void loadQuestionTeacherCommentLabel(QuestionMode mode, QWidget *parent,
                                         QGridLayout *layout, int &row);
    void loadEndOfQuestionDummySpace(QuestionMode mode, QWidget *parent,
                                     QGridLayout *layout, int &row);
};

#endif // MEDNUSQUIZQUESTION_H

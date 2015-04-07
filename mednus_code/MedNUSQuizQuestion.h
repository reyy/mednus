#ifndef MEDNUSQUIZQUESTION_H
#define MEDNUSQUIZQUESTION_H

#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QGridLayout>
#include <QDir>
#include <QDebug>
#include <QLineEdit>

#include "MedNUSQuizStylesheet.h"

class MedNUSQuizQuestion
{
public:

    /*  MedNUSQuizQuestion constructor (viewer view):
     *      The content that is being passed in contains the question text,
     *      options, and the teacher's comment. It is stored in this format:
     *          content[0]              = question text
     *          content[1..noOfOptions] = options
     *          content[noOfOptions+1]  = teacher's comment
     */
    MedNUSQuizQuestion(QWidget *parent, QGridLayout *layout, int& row,
                       QVector<QString> content, int noOfOptions,
                       bool hasImage = false, QString imageUrl = "");

    /*  MedNUSQuizQuestion constructor (editor view - new question):
     *      This constructor is meant to be used by the quiz editor to
     *      create new (empty) questions.
     */
    MedNUSQuizQuestion(QWidget *parent, QGridLayout *layout, int &row);

    /*  MedNUSQuizQuestion constructor (editor view - existing question):
     *      This constructor is meant to be used by the quiz editor to
     *      create new (empty) questions.
     */
    //TODO: copy from the viewer view.
    ~MedNUSQuizQuestion();

    int getSelectedAnswer() const;
    void highlightAnswer(int correctAnswer, bool showCorrectAnswer) const;
    bool oneOptionSelected() const;

    // Set a highlight on text label of question left unanswered.
    void setNotice(bool isAnswered);

    // Called on Start Quiz
    void showQuestion() const;

private:
// For storage
    QString _questionImageUrl;
    QString _questionText;
    QString _teacherComment;
    int _noOfOptions;
    QVector<QString>* _options;

// For display (viewing)
    QLabel* _questionImageLabel;
    QLabel* _questionTextLabel;
    QLabel* _teacherCommentLabel;
    QButtonGroup* _optionButtonGroup;

// For display (editing)
    QLineEdit* _questionImageLabelEdit;
    QLineEdit* _questionTextLabelEdit;
    QLineEdit* _teacherCommentLabelEdit;
    QVector<QLineEdit*>* _optionsLabelEdit;
};

#endif // MEDNUSQUIZQUESTION_H

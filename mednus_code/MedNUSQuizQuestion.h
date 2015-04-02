#ifndef MEDNUSQUIZQUESTION_H
#define MEDNUSQUIZQUESTION_H

#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QGridLayout>
#include <QDir>

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
    MedNUSQuizQuestion(QWidget *parent, QGridLayout *layout, int& row,
                       QVector<QString> content, int noOfOptions,
                       bool hasImage = false, QString imageUrl = "");
    ~MedNUSQuizQuestion();

    int getSelectedAnswer() const;
    void highlightAnswer(int correctAnswer, bool showCorrectAnswer) const;
    bool oneOptionSelected() const;

    // Set a highlight on text label of question left unanswered.
    void setNotice(bool isAnswered);

    // Called on Start Quiz
    void showQuestion() const;

private:
    QLabel* _questionImageLabel;
    QLabel* _questionTextLabel;
    QLabel* _teacherCommentLabel;
    QButtonGroup* _optionButtonGroup;
};

#endif // MEDNUSQUIZQUESTION_H

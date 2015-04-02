#include "MedNUSQuizQuestion.h"

MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent, QGridLayout *layout,
                                       int& row, QVector<QString> content,
                                       int noOfOptions, bool hasImage,
                                       QString imageUrl)
{
    _optionButtonGroup = new QButtonGroup(parent);

    // Initialize the question's text.
    _questionTextLabel = new QLabel(content[0], parent);
    _questionTextLabel->setWordWrap(true);
    setNotice(true);
    _questionTextLabel->setFont (QFont ("Helvetica", 12));
    _questionTextLabel->setVisible(false);
    layout->addWidget(_questionTextLabel, row++, 0, 1, 2);

    // Initialize the question's image (if any).
    if (hasImage) {
        _questionImageLabel = new QLabel(parent);
        _questionImageLabel->setPixmap(QPixmap(QDir::homePath()+imageUrl));
        _questionImageLabel->setVisible(false);
        layout->addWidget(_questionImageLabel, row++, 0, 1, 2);
    } else {
        _questionImageLabel = NULL;
    }

    // Initialize the buttons
    QRadioButton* tempButton;
    for (int i = 1, j = 0; i <= noOfOptions; i++, j++)
    {
        tempButton = new QRadioButton(content[i], parent);
        tempButton->setStyleSheet(RADIO_BUTTON_STYLESHEET);
        tempButton->setFont (QFont ("Helvetica", 11));
        tempButton->setVisible(false);
        _optionButtonGroup->addButton(tempButton, i);
        layout->addWidget(tempButton, row, j%2, 1, 1);

        j%2 == 1 ? row++ : 0;
    }

    // Quick fix: For the tecaher's comment appearing on the same spot as
    //              the option if there's odd number of options.
    if (noOfOptions % 2 == 1)
        row++;

    _teacherCommentLabel = new QLabel(parent);
    _teacherCommentLabel->setText(content[noOfOptions+1]);
    _teacherCommentLabel->setWordWrap(true);
    _teacherCommentLabel->setStyleSheet(TEACHER_COMMENT_STYLESHEET);
    _teacherCommentLabel->setFont (QFont ("Helvetica", 11,QFont::Bold,true));
    layout->addWidget(_teacherCommentLabel, row++, 0, 1, 2);
    _teacherCommentLabel->setVisible(false);
}


MedNUSQuizQuestion::~MedNUSQuizQuestion()
{
    // Clean up the label
    delete _questionTextLabel;
    _questionTextLabel = NULL;

    // Clean up the buttons
    delete _optionButtonGroup;
    _optionButtonGroup = NULL;
}


int MedNUSQuizQuestion::getSelectedAnswer() const
{
    return _optionButtonGroup->checkedId();
}


void MedNUSQuizQuestion::highlightAnswer(int correctAnswer,
                                         bool showCorrectAnswer) const
{
    QList<QAbstractButton*> buttonList = _optionButtonGroup->buttons();
    for (int i = 0; i < buttonList.size(); i++)
    {
        if (((QRadioButton*)buttonList.at(i))->isChecked()) {
            // Am I the correct answer?
            if (i+1 == correctAnswer) {
                // Set me to green.
                ((QRadioButton*)buttonList.at(i))->
                        setStyleSheet(RADIO_BUTTON_HIGHLIGHT_GREEN);
            } else {
                // Set me to red.
                ((QRadioButton*)buttonList.at(i))->
                        setStyleSheet(RADIO_BUTTON_HIGHLIGHT_RED);
            }
        } else {
            // Check if I should be revealing the correct answer.
            if (showCorrectAnswer && i+1 == correctAnswer) {
                // Set me to green.
                ((QRadioButton*)buttonList.at(i))->
                        setStyleSheet(RADIO_BUTTON_HIGHLIGHT_GREEN);
            } else {
                // Set me to grey.
                ((QRadioButton*)buttonList.at(i))->
                        setStyleSheet(RADIO_BUTTON_HIGHLIGHT_GREY);
            }
        }
    }
    _teacherCommentLabel->setVisible(true);
}


bool MedNUSQuizQuestion::oneOptionSelected() const
{
    if (_optionButtonGroup->checkedId() == -1)
        return false;

    return true;
}


void MedNUSQuizQuestion::setNotice(bool isAnswered) {
    if (isAnswered) {
        _questionTextLabel->setStyleSheet(QUESTION_ANSWERED_NOTICE_STYLESHEET);
    } else {
        _questionTextLabel->setStyleSheet(QUESTION_UNANSWERED_NOTICE_STYLESHEET);
    }
}


void MedNUSQuizQuestion::showQuestion() const
{
    _questionTextLabel->setVisible(true);

    if (_questionImageLabel != NULL) {
        _questionImageLabel->setVisible(true);
    }

    QList<QAbstractButton*> buttonList = _optionButtonGroup->buttons();
    for (int i = 0; i < buttonList.size(); i++)
    {
        ((QRadioButton*)buttonList.at(i))->setVisible(true);
    }
}

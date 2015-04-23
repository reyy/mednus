#include "MedNUSQuizQuestion.h"

const int MAX_NO_OF_OPTIONS = 4;

// Offsets determine the position of the object in the content QVector that
//      is passed into the constructor.
const int CONTENT_TEACHER_COMMENT_OFFSET = 1;
const int CONTENT_CORRECT_ANSWER_OFFSET = 2;

MedNUSQuizQuestion::MedNUSQuizQuestion(int questionNum, QVector<QString> content,
                                       int noOfOptions, bool hasImage,
                                       QString imageUrl) {

    qDebug() << "qNum: " << questionNum;
    qDebug() << "noOfOptions = " << noOfOptions;
    qDebug() << "content.length = " << content.size();

    _questionNum = questionNum;
    _questionText = content[0];

    if (hasImage)
        _questionImageUrl = QDir::homePath() + imageUrl;
    else
        _questionImageUrl = "";

    _noOfOptions = noOfOptions;
    _dummySpace = new QVector<QLabel*>();
    _amtOfDummySpace = 3;
    _options = new QVector<QString>();
    _optionsLabelEdit = new QVector<QLineEdit*>();
    for (int i = 1; i <= noOfOptions; i++)
        _options->append(content[i]);

    _teacherComment = content[noOfOptions+CONTENT_TEACHER_COMMENT_OFFSET];
    _correctAnswer = content[noOfOptions+CONTENT_CORRECT_ANSWER_OFFSET].toInt();
}

MedNUSQuizQuestion::MedNUSQuizQuestion(int questionNum) {

    _questionNum = questionNum;
    _questionText = "";
    _questionImageUrl = "";
    _noOfOptions = 0;
    _dummySpace = new QVector<QLabel*>();
    _amtOfDummySpace = 3;
    _options = new QVector<QString>();
    _optionsLabelEdit = new QVector<QLineEdit*>();
    _teacherComment = "";
    _correctAnswer = 0;
}

/*
MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent, QGridLayout *layout,
                                       int& row, QVector<QString> content,
                                       int noOfOptions, bool hasImage,
                                       QString imageUrl, int questionNum)
{
    _optionButtonGroup = new QButtonGroup(this);

    // Initialize the question number label.
    _questionNum = questionNum;
    _questionNumLabel = new QLabel("Question " + QString::number(_questionNum),
                                   parent);
    _questionNumLabel->setWordWrap(true);
    _questionNumLabel->setFont (QFont ("Helvetica", 12, QFont::Bold));
    _questionNumLabel->setVisible(false);
    layout->addWidget(_questionNumLabel, row++, 0, 1, 2);

    // Initialize the question's text.
    _questionText = content[0];
    _questionTextLabel = new QLabel(_questionText, this);
    ((QLabel*)_questionTextLabel)->setWordWrap(true);
    setNotice(true);
    _questionTextLabel->setFont (QFont ("Helvetica", 12));
    _questionTextLabel->setVisible(false);
    layout->addWidget(_questionTextLabel, row++, 0, 1, 2);

    // Initialize the question's image (if any).
    if (hasImage) {
        _questionImageUrl = QDir::homePath() + imageUrl;
        _questionImageLabel = new QLabel(this);
        ((QLabel*)_questionImageLabel)->setPixmap(QPixmap(_questionImageUrl));
        _questionImageLabel->setVisible(false);
        layout->addWidget(_questionImageLabel, row++, 0, 1, 2);
    } else {
        _questionImageLabel = NULL;
    }

    // Initialize the buttons
    QRadioButton* tempButton;
    _options = new QVector<QString>();
    for (int i = 1, j = 0; i <= noOfOptions; i++, j++)
    {
        _options->append(content[i]);
        tempButton = new QRadioButton(_options->at(i-1), this);
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

    _teacherComment = content[noOfOptions+1];
    _teacherCommentLabel = new QLabel(this);
    ((QLabel*)_teacherCommentLabel)->setText(_teacherComment);
    ((QLabel*)_teacherCommentLabel)->setWordWrap(true);
    _teacherCommentLabel->setStyleSheet(TEACHER_COMMENT_STYLESHEET);
    _teacherCommentLabel->setFont (QFont ("Helvetica", 11,QFont::Bold,true));
    layout->addWidget(_teacherCommentLabel, row++, 0, 1, 2);
    _teacherCommentLabel->setVisible(false);
}
*/

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


void MedNUSQuizQuestion::highlightAnswer(bool showCorrectAnswer) const
{
    QList<QAbstractButton*> buttonList = _optionButtonGroup->buttons();
    for (int i = 0; i < buttonList.size(); i++)
    {
        if (((QRadioButton*)buttonList.at(i))->isChecked()) {
            // Am I the correct answer?
            if (i+1 == _correctAnswer) {
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
            if (showCorrectAnswer && i+1 == _correctAnswer) {
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


void MedNUSQuizQuestion::loadQuestion(QuestionMode mode, QWidget *parent,
                                      QGridLayout *layout, int &row) {


    // Load some dummy space at the start of the question to seperate the questions.
    loadEndOfQuestionDummySpace(mode, parent, layout, row);

    qDebug()<<"num="<<_questionNum;
    qDebug()<<"1";
    loadQuestionNumLabel(mode, parent, layout, row);
    qDebug()<<"2";

    loadQuestionTextLabel(mode, parent, layout, row);
    qDebug()<<"3";

    loadQuestionImageLabel(mode, parent, layout, row);
    qDebug()<<"4";

    loadQuestionOptions(mode, parent, layout, row);
    qDebug()<<"5";

    // Quick fix: For the tecaher's comment appearing on the same spot as
    //              the option if there's odd number of options.
    if (_noOfOptions % 2 == 1)
        row++;

    loadQuestionTeacherCommentLabel(mode, parent, layout, row);
    qDebug()<<"6";
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
    _questionNumLabel->setVisible(true);
    _questionTextLabel->setVisible(true);

    if (_questionImageLabel != NULL)
        _questionImageLabel->setVisible(true);

    QList<QAbstractButton*> buttonList = _optionButtonGroup->buttons();
    for (int i = 0; i < buttonList.size(); i++)
        ((QRadioButton*)buttonList.at(i))->setVisible(true);

    for (int i = 0; i < _amtOfDummySpace; i++)
        _dummySpace->at(i)->setVisible(true);
}


bool MedNUSQuizQuestion::saveChanges() {

    qDebug()<<"saving num="<<_questionNum;
    _questionText = ((QLineEdit*)_questionTextLabel)->displayText();
    qDebug()<<"done qtext";
    _teacherComment = ((QLineEdit*)_teacherCommentLabel)->displayText();
    qDebug()<<"done tc";


    if (!saveOptions())
        return false;

    _correctAnswer = _correctAnswerDropDownBox->currentIndex()+1;

    return true;
}

bool MedNUSQuizQuestion::saveOptions() {
    //TODO: Change it such that is push a button to add options.
    _options->clear();
    for (int i = 0; i < MAX_NO_OF_OPTIONS; i++) {
        qDebug()<<"option"<<i;
        QLineEdit* lineEdit = _optionsLabelEdit->at(i);

        if (lineEdit->text().length() > 0)
            _options->append(lineEdit->text());
    }

    qDebug() << "new noOfOptions" << _options->size();
    _noOfOptions = _options->size();

    return true;
}

void MedNUSQuizQuestion::writeToFile(QJsonObject &json) {

    json["noOfOptions"] = _noOfOptions;
    json["question"] = _questionText;
    json["correctAnswer"] = _correctAnswer;
    json["teacherComment"] = _teacherComment;

    QJsonArray optionArray;

    for (int i = 0; i < _noOfOptions; i++) {
        optionArray.append(_options->at(i));
    }

    json["options"] = optionArray;
}

bool MedNUSQuizQuestion::markQuestion()
{
    if (getSelectedAnswer() == _correctAnswer)
        return true;

    return false;
}


void MedNUSQuizQuestion::loadQuestionNumLabel(QuestionMode mode, QWidget *parent,
                                              QGridLayout *layout, int &row)
{
    _questionNumLabel = new QLabel("Question " + QString::number(_questionNum),
                                   parent);
    _questionNumLabel->setWordWrap(true);
    _questionNumLabel->setAlignment(Qt::AlignLeft);
    _questionNumLabel->setFont (QFont ("Helvetica", 24, QFont::Bold));

    if (mode == VIEWER)
        _questionNumLabel->setVisible(false);

    layout->addWidget(_questionNumLabel, row++, 0, 1, 2);
}


void MedNUSQuizQuestion::loadQuestionTextLabel(QuestionMode mode, QWidget *parent,
                                               QGridLayout *layout, int &row)
{
    if (mode == VIEWER) {

        _questionTextLabel = new QLabel(_questionText, parent);
        ((QLabel*)_questionTextLabel)->setWordWrap(true);
        setNotice(true);
        _questionTextLabel->setFont (QFont ("Helvetica", 12));
        _questionTextLabel->setVisible(false);
        layout->addWidget(_questionTextLabel, row++, 0, 1, 2);

    } else if (mode == EDITOR) {

        _questionTextLabel = new QLineEdit(_questionText, parent);
        ((QLineEdit*)_questionTextLabel)->setPlaceholderText("Question Text");
        _questionTextLabel->setFont (QFont ("Helvetica", 12));
        _questionTextLabel->setVisible(true);
        layout->addWidget(_questionTextLabel, row++, 0, 1, 2);

    } else {
        qWarning() << "MedNUSQuizQuestion::loadQuestionTextLabel():  Invalid mode set.";
    }
}


void MedNUSQuizQuestion::loadQuestionImageLabel(QuestionMode mode, QWidget *parent,
                                                QGridLayout *layout, int &row)
{
    if (_questionImageUrl.length() > 0) {

        _questionImageLabel = new QLabel(parent);
        ((QLabel*)_questionImageLabel)->setPixmap(QPixmap(_questionImageUrl));

        if (mode == VIEWER)
            _questionImageLabel->setVisible(false);

        layout->addWidget(_questionImageLabel, row++, 0, 1, 2);

    } else {
        _questionImageLabel = NULL;
    }
}


void MedNUSQuizQuestion::loadQuestionOptions(QuestionMode mode, QWidget *parent,
                                             QGridLayout *layout, int &row)
{
    if (mode == VIEWER) {

        _optionButtonGroup = new QButtonGroup(parent);

        // Create and load the radio buttons.
        for (int i = 1, j = 0; i <= _noOfOptions; i++, j++) {
            QRadioButton* tempButton = new QRadioButton(_options->at(i-1), parent);
            tempButton->setStyleSheet(RADIO_BUTTON_STYLESHEET);
            tempButton->setFont (QFont ("Helvetica", 11));
            tempButton->setVisible(false);
            _optionButtonGroup->addButton(tempButton, i);
            layout->addWidget(tempButton, row, j%2, 1, 1);

            j%2 == 1 ? row++ : 0;
        }

    } else if (mode == EDITOR) {

        //TODO: Add a "Options" label here to inform the user that these are options.

        // Existing options.
        for (int i = 1; i <= _noOfOptions; i++) {
            QLabel* optionNumLabel = new QLabel("Option "+QString::number(i), parent);
            optionNumLabel->setVisible(true);
            layout->addWidget(optionNumLabel, row, 0, 1, 1);
            QLineEdit* lineEdit = new QLineEdit(_options->at(i-1), parent);
            lineEdit->setFont (QFont ("Helvetica", 12));
            _optionsLabelEdit->append(lineEdit);
            layout->addWidget(lineEdit, row++, 1, 1, 1);
        }

        // Unused options.
        if (_noOfOptions < MAX_NO_OF_OPTIONS) {

            for (int i = _noOfOptions; i < MAX_NO_OF_OPTIONS; i++) {
                QLabel* optionNumLabel = new QLabel("Option "+QString::number(i+1), parent);
                optionNumLabel->setVisible(true);
                layout->addWidget(optionNumLabel, row, 0, 1, 1);
                QLineEdit* lineEdit = new QLineEdit("", parent);
                lineEdit->setPlaceholderText("Option " + QString::number(i+1));
                lineEdit->setFont (QFont ("Helvetica", 12));
                _optionsLabelEdit->append(lineEdit);
                layout->addWidget(lineEdit, row++, 1, 1, 1);
            }

        }

        QLabel* correctAnswerLabel = new QLabel("Correct Option:", parent);
        correctAnswerLabel->setVisible(true);
        layout->addWidget(correctAnswerLabel, row, 0, 1, 1);

        _correctAnswerDropDownBox = new QComboBox(parent);
        for (int i = 1; i <= MAX_NO_OF_OPTIONS; i++) {
            _correctAnswerDropDownBox->addItem(QString::number(i));
        }
        _correctAnswerDropDownBox->setVisible(true);
        _listView = new QListView(_correctAnswerDropDownBox);
        _listView->setStyleSheet(DROPDOWN_LIST_VIEW_STYLESHEET);
        _correctAnswerDropDownBox->setView(_listView);
        _correctAnswerDropDownBox->setCurrentIndex(_correctAnswer-1);
        layout->addWidget(_correctAnswerDropDownBox, row++, 1, 1, 1);

    } else {
        qWarning() << "MedNUSQuizQuestion::loadQuestionOptions():  Invalid mode set.";
    }
}

void MedNUSQuizQuestion::loadQuestionTeacherCommentLabel(QuestionMode mode,
                                                         QWidget *parent,
                                                         QGridLayout *layout,
                                                         int &row)
{
    if (mode == VIEWER) {

        _teacherCommentLabel = new QLabel(_teacherComment, parent);
        ((QLabel*)_teacherCommentLabel)->setWordWrap(true);
        _teacherCommentLabel->setStyleSheet(TEACHER_COMMENT_STYLESHEET);
        _teacherCommentLabel->setFont (QFont ("Helvetica", 11,QFont::Bold,true));
        layout->addWidget(_teacherCommentLabel, row++, 0, 1, 2);
        _teacherCommentLabel->setVisible(false);

    } else if (mode == EDITOR) {

        _teacherCommentLabel = new QLineEdit(_teacherComment, parent);
        ((QLineEdit*)_teacherCommentLabel)->setPlaceholderText("Comment for student.");
        _teacherCommentLabel->setFont (QFont ("Helvetica", 12));
        layout->addWidget(_teacherCommentLabel, row++, 0, 1, 2);

    } else {
        qWarning() << "MedNUSQuizQuestion::loadQuestionTeacherCommentLabel():  Invalid mode set.";
    }
}

void MedNUSQuizQuestion::loadEndOfQuestionDummySpace(QuestionMode mode,
                                                     QWidget *parent,
                                                     QGridLayout *layout,
                                                     int &row) {
    for (int i = 0; i < _amtOfDummySpace; i++) {
        QLabel* dummySpace = new QLabel(" ", parent);
        dummySpace->setFont(QFont("Helvetica", 12, QFont::Bold));
        dummySpace->setAlignment(Qt::AlignCenter);

        if (mode == VIEWER)
            dummySpace->setVisible(false);

        layout->addWidget(dummySpace, row++, 0, 1, 2);

        _dummySpace->append(dummySpace);
    }
}

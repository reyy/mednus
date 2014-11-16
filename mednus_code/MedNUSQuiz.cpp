#include "MedNUSQuiz.h"

const QString __startScreenText = "Read the instructions before taking the Quiz. When you have finished with the Quiz, click the Submit button.";

MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent, QVBoxLayout *layout, QVector<QString> content, int noOfOptions)
{
    _optionButtonGroup = new QButtonGroup(parent);

    // Initialize the QLabel
    _questionTextLabel = new QLabel(content[0], parent);
    _questionTextLabel->setWordWrap(true);
    //_questionTextLabel->setStyleSheet("QLabel { color : black; }");
    //_questionTextLabel->setGeometry(parent->geometry());
    //_questionTextLabel->setWordWrap(true);
    _questionTextLabel->setContentsMargins(20,10,10,0);
    //_questionTextLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _questionTextLabel->setVisible(false);
    layout->addWidget(_questionTextLabel);

    // Initialize the buttons
    QRadioButton* tempButton;
    for (int i = 1; i <= noOfOptions; i++)
    {
        tempButton = new QRadioButton(content[i], parent);
        //tempButton->setStyleSheet("QRadioButton { color : black;}");
        //tempButton->setContentsMargins(5,0,5,10);
        tempButton->setVisible(false);
        _optionButtonGroup->addButton(tempButton, i);
        layout->addWidget(tempButton);
    }
    _teacherCommentLabel = new QLabel(parent);
    _teacherCommentLabel->setText(content[noOfOptions+1]);
    _teacherCommentLabel->setWordWrap(true);
    _teacherCommentLabel->setStyleSheet("QLabel {font:italic}");
    //_teacherCommentLabel->setStyleSheet("QLabel { color : black; }");
    //_teacherCommentLabel->setGeometry(parent->geometry());
    //_teacherCommentLabel->setWordWrap(true);
    //_teacherCommentLabel->setContentsMargins(5,5,5,5);
    //_teacherCommentLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(_teacherCommentLabel);
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

void MedNUSQuizQuestion::highlightAnswer(int correctAnswer, bool showCorrectAnswer) const
{
    QList<QAbstractButton*> buttonList = _optionButtonGroup->buttons();
    for (int i = 0; i < buttonList.size(); i++)
    {
        if (showCorrectAnswer)
        {
            if (i+1 == correctAnswer)
            {
                // Set the correct answer green.
                ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {font: bold;color: rgba(50, 255, 50, 255);}");
            }
            else if (((QRadioButton*)buttonList.at(i))->isChecked())
            {
                // Set the wrong answer red.
                ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {color: rgba(255, 0, 0, 255);}");
            }
            else
            {
                // Set the others opacity 50%.
                ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {color: rgba(0, 0, 0, 50);}");
            }
        }
        else
        {
            if (((QRadioButton*)buttonList.at(i))->isChecked() && i+1 == correctAnswer)
            {
                // Set the correct answer green.
                ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {font: bold;color: rgba(50, 255, 50, 255);}");
            }
            else if (((QRadioButton*)buttonList.at(i))->isChecked())
            {
                // Bold the selected answer.
                ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {font: bold;}");
            }
            else
            {
                // Set the others opacity 50%.
                ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {color: rgba(0, 0, 0, 50);}");
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

void MedNUSQuizQuestion::myForceResize(QRect geometry)
{
    //TODO: Fix this
    //_questionTextLabel->setGeometry(QRect(_questionTextLabel->geometry().left(), _questionTextLabel->geometry().top(), geometry.width(), _questionTextLabel->geometry().height()));
}

void MedNUSQuizQuestion::showQuestion() const
{
    _questionTextLabel->setVisible(true);

    QList<QAbstractButton*> buttonList = _optionButtonGroup->buttons();
    for (int i = 0; i < buttonList.size(); i++)
    {
        ((QRadioButton*)buttonList.at(i))->setVisible(true);
    }
}

MedNUSQuiz::MedNUSQuiz(QString filename, QWidget *parent) :
    QWidget(parent)
{
    this->setAccessibleName(filename);
    _tempWidget = new QWidget(parent);
    _layout = new QVBoxLayout(_tempWidget);

    initQuiz(filename);
    initStartScreen();

    _tempWidget->setLayout(_layout);
    _tempWidget->setStyleSheet("background-color: #6894ad");
    _tempWidget->setContentsMargins(10,10,10,10);
    _tempWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _scrollArea = new QScrollArea(this);
    _scrollArea->setContentsMargins(20,20,20,20);
    _scrollArea->setWidgetResizable(true);
    _scrollArea->setWidget(_tempWidget);
    //_scrollArea->setGeometry(this->geometry());
    _scrollArea->setAutoFillBackground(true);
    _scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //Load scrollbar style.
    QFile file2(":/images/scrollbar.css");
    if(file2.open(QIODevice::ReadOnly|QIODevice::Text)) {
        _scrollArea->setStyleSheet(file2.readAll());
        file2.close();
    }
}

MedNUSQuiz::~MedNUSQuiz()
{
    //TODO: Clean up
}

void MedNUSQuiz::markQuiz()
{
    // Check if all the questions have been answered.
    for (int i = 0; i < _questionList->size(); i++)
    {
        if (((MedNUSQuizQuestion*)_questionList->at(i))->oneOptionSelected() == false)
        {
            // Create a message box;
            QMessageBox *warningMessageBox = new QMessageBox();
            warningMessageBox->setText("Please answer all the questions.");
            warningMessageBox->exec();
            return;
        }
    }

    // Start the marking of the quiz.
    int score = 0;
    for (int i = 0; i < _questionList->size(); i++)
    {
        if (((MedNUSQuizQuestion*)_questionList->at(i))->getSelectedAnswer() == _correctAnswerList[i])
        {
            score++;
            QTextStream(stdout) << "Q" << i+1 << "=CORRECT";
        }
        else
        {
            QTextStream(stdout) << "Q" << i+1 << "=WRONG";
        }
        // Show the correct answers only if the teacher wants to.
        ((MedNUSQuizQuestion*)_questionList->at(i))->highlightAnswer(_correctAnswerList[i], _showAnswerFlag);
    }
    QTextStream(stdout) << "score=" << score;
}

void MedNUSQuiz::startQuiz()
{
    // Remove all of the start screen elements.
    _startScreenLabel->setVisible(false);
    _startQuizButton->setVisible(false);
    _instructionTextLabel->setVisible(false);

    // Show all the quiz elements.
    for (int i = 0; i < _questionList->size(); i++)
    {
       ((MedNUSQuizQuestion*)_questionList->at(i))->showQuestion();
    }
    _markButton->setVisible(true);
}

void MedNUSQuiz::resizeEvent(QResizeEvent *event)
{
    _scrollArea->setGeometry(this->geometry());
    //_tempWidget->setGeometry(this->geometry());

    // Go through every QLabel in _layout and setGeomtry accordingly
    for (int i = 0; i < _questionList->size(); i++)
    {
       ((MedNUSQuizQuestion*)_questionList->at(i))->myForceResize(this->geometry());
    }
}

bool MedNUSQuiz::initStartScreen()
{
    //_startScreenLayout = new QVBoxLayout(_tempWidget);
    _startScreenLabel = new QLabel(__startScreenText, _tempWidget);
    _startScreenLabel->setWordWrap(true);
    _layout->addWidget(_startScreenLabel);

    // Start Quiz button
    _startQuizButton = new QPushButton("Start Quiz", _tempWidget);
    _startQuizButton->setGeometry(QRect(QPoint(100, 100), QSize(50, 50)));
    //_markButton->setStyleSheet("QPushButton { background-color : rgba(255, 255, 255, 255) }");
    connect(_startQuizButton, SIGNAL(released()), this, SLOT(startQuiz()));
    _layout->addWidget(_startQuizButton);
    _layout->setAlignment(Qt::AlignTop);
    //_layout->setContentsMargins(5,5,5,5);
}

bool MedNUSQuiz::initQuiz(QString filename)
{
    // Add questions here //
    _questionList = new QVector<MedNUSQuizQuestion*>();

    // Read Json file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file.";

        // TODO: Add warning message box.
    }
    QByteArray fileValue = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(fileValue);
    QJsonObject obj = doc.object();
    QJsonValue v = obj.value(QString("details"));
    QJsonObject quizDetails = v.toObject();

    // Quiz title text label
    _titleTextLabel = new QLabel(quizDetails["title"].toString(), _tempWidget);
    _titleTextLabel->setStyleSheet("QLabel {font: bold; font-size: 18px}");
    _layout->addWidget(_titleTextLabel);

    // Quiz flags
    _showAnswerFlag = quizDetails["showAnswer"].toBool();
    _showTeacherCommentFlag = quizDetails["showTeacherComment"].toBool();

    int noOfQuestions = quizDetails["noOfQuestions"].toInt();

    // The instruction text at the top of the quiz.
    _instructionTextLabel = new QLabel(quizDetails["introductionText"].toString(), _tempWidget);
    _instructionTextLabel->setWordWrap(true);
    _instructionTextLabel->setStyleSheet("QLabel {font: italic}");
    //_instructionTextLabel->setVisible(false);
    _layout->addWidget(_instructionTextLabel);

    // Load the quiz questions.
    QString questionLabelString = "question_";
    QVector<QString> content;
    for (int i = 1; i <= noOfQuestions; i++)
    {
        // Construct the QJsonObject for the particular question first.
        QJsonObject jsonQuestion = obj.value(questionLabelString+QString::number(i)).toObject();

        // Check the number of options for the question.
        int noOfOptions = jsonQuestion["noOfOptions"].toInt();

        // Load the question.
        content.append(QString(jsonQuestion["question"].toString()));

        // Load the options.
        QJsonArray optionArray = jsonQuestion["options"].toArray();

        for (int j = 0; j < optionArray.size(); j++)
            content.append(QString(optionArray[j].toString()));

        // Load the teacher comment.
        content.append(QString(jsonQuestion["teacherComment"].toString()));

        // Load the correct answers.
        _correctAnswerList.append(jsonQuestion["correctAnswer"].toInt());

        // Create the question.
        MedNUSQuizQuestion *question = new MedNUSQuizQuestion(_tempWidget, _layout, content, noOfOptions);

        // Push the question into the question vector.
        _questionList->append(question);
        content.clear();
    }

    // Marking button
    _markButton = new QPushButton("Submit", _tempWidget);
    _markButton->setGeometry(QRect(QPoint(100, 100), QSize(50, 50)));
    _markButton->setVisible(false);
    _markButton->setContentsMargins(0, 30, 0, 0);
    //_markButton->setStyleSheet("QPushButton { background-color : rgba(255, 255, 255, 255) }");
    connect(_markButton, SIGNAL(released()), this, SLOT(markQuiz()));
    _layout->addWidget(_markButton);
    _layout->setAlignment(Qt::AlignTop);
    //_layout->setContentsMargins(5,5,5,5);

    return true;
}

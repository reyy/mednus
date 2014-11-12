#include "MedNUSQuiz.h"

MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent, QVBoxLayout *layout, QVector<QString> content, int noOfOptions) :
    QWidget(parent)
{
    this->setVisible(false);
    _optionButtonGroup = new QButtonGroup(this);

    // Initialize the QLabel
    _questionTextLabel = new QLabel(content[0], this);
    _questionTextLabel->setStyleSheet("QLabel { color : black; }");
    _questionTextLabel->setGeometry(parent->geometry());
    _questionTextLabel->setWordWrap(true);
    _questionTextLabel->setContentsMargins(0,0,0,0);
    _questionTextLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(_questionTextLabel);

    // Initialize the buttons
    QRadioButton* tempButton;
    for (int i = 1; i <= noOfOptions; i++)
    {
        tempButton = new QRadioButton(content[i], this);
        tempButton->setStyleSheet("QRadioButton { color : black;}");
        tempButton->setContentsMargins(5,5,5,5);
        _optionButtonGroup->addButton(tempButton, i);
        layout->addWidget(tempButton);
    }
    _teacherCommentLabel = new QLabel(this);
    _teacherCommentLabel->setText(content[noOfOptions+1]);
    _teacherCommentLabel->setStyleSheet("QLabel { color : black; }");
    _teacherCommentLabel->setGeometry(parent->geometry());
    _teacherCommentLabel->setWordWrap(true);
    _teacherCommentLabel->setContentsMargins(0,0,0,0);
    _teacherCommentLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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

void MedNUSQuizQuestion::showCorrectAnswer(int correctAnswer) const
{
    QList<QAbstractButton*> buttonList = _optionButtonGroup->buttons();
    for (int i = 0; i < buttonList.size(); i++)
    {
        if (i+1 == correctAnswer)
        {
            // Set the correct answer green.
            ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {color: rgba(50, 255, 50, 255);}");
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
    _questionTextLabel->setGeometry(QRect(_questionTextLabel->geometry().left(), _questionTextLabel->geometry().top(), geometry.width(), _questionTextLabel->geometry().height()));
}

MedNUSQuiz::MedNUSQuiz(QString filename, QWidget *parent) :
    QWidget(parent)
{
    _tempWidget = new QWidget(parent);
    _layout = new QVBoxLayout(_tempWidget);

    // Add questions here
    _questionList = new QVector<MedNUSQuizQuestion*>();
    // Read Json file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file.";
    }
    QByteArray fileValue = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(fileValue);
    QJsonObject obj = doc.object();
    QJsonValue v = obj.value(QString("details"));
    QJsonObject quizDetails = v.toObject();

    int noOfQuestions = quizDetails["noOfQuestions"].toInt();

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
    _markButton = new QPushButton("Check Answers", this);
    _markButton->setGeometry(QRect(QPoint(100, 100), QSize(50, 50)));
    _markButton->setStyleSheet("QPushButton { background-color : rgba(255, 255, 255, 255) }");
    connect(_markButton, SIGNAL(released()), this, SLOT(markQuiz()));
    _layout->addWidget(_markButton);
    _layout->setAlignment(Qt::AlignTop);
    _layout->setContentsMargins(5,5,5,5);

    _tempWidget->setLayout(_layout);
    _tempWidget->setStyleSheet("QWidget { background-color : rgba(255, 255, 255, 255) }");
    _tempWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _scrollArea = new QScrollArea(this);
    //_scrollArea->setObjectName(QStringLiteral("_scrollArea"));
    //_scrollArea->setWidgetResizable(true);
    _scrollArea->setContentsMargins(0,0,0,0);
    _scrollArea->setWidget(_tempWidget);
    _scrollArea->setGeometry(this->geometry());
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
    //TODO: Fix clean up.
    //      Currently, clicking the close quiz icon crashes the program.

    /*// Clean up the question list.
    _questionList->clear();
    delete _questionList; _questionList = NULL;

    // Clean up the answer list.
    _correctAnswerList.clear();

    // Clean up the other QT stuff.
    delete _layout; _layout = NULL;
    delete _scrollArea; _scrollArea = NULL;
    delete _markButton; _markButton = NULL;
    delete _tempWidget; _tempWidget = NULL;*/
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
        ((MedNUSQuizQuestion*)_questionList->at(i))->showCorrectAnswer(_correctAnswerList[i]);
    }
    QTextStream(stdout) << "score=" << score;
}

void MedNUSQuiz::resizeEvent(QResizeEvent *event)
{
    _scrollArea->setGeometry(this->geometry());
    _tempWidget->setGeometry(this->geometry());

    // Go through every QLabel in _layout and setGeomtry accordingly
    for (int i = 0; i < _questionList->size(); i++)
    {
       //((MedNUSQuizQuestion*)_questionList->at(i))->myForceResize(this->geometry());
    }
}

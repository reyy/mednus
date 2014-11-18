#include "MedNUSQuiz.h"
#include "MedNUSAUISettings.h"

MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent, QGridLayout *layout, int& row, QVector<QString> content, int noOfOptions, bool hasImage, QString imageUrl)
{
    _optionButtonGroup = new QButtonGroup(parent);
    
    // Initialize the question's text.
    _questionTextLabel = new QLabel(content[0], parent);
    _questionTextLabel->setWordWrap(true);
    setNotice(false);
    _questionTextLabel->setFont (QFont ("Helvetica", 12));
    _questionTextLabel->setVisible(false);
    layout->addWidget(_questionTextLabel, row++, 0, 1, 2);

    // Initialize the question's image (if any).
    if (hasImage) {
        qDebug() << imageUrl;
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
        tempButton->setStyleSheet("QRadioButton { margin-left:10px;margin-right:10px;margin-top:2px;margin-bottom:2px;} QRadioButton::indicator{margin-right:10px;}QRadioButton::indicator::unchecked{image:url(:/images/check_default.png)} QRadioButton::indicator::unchecked:pressed{image:url(:/images/check_default.png)} QRadioButton::indicator::unchecked:hover{image:url(:/images/check_default_hover.png)} QRadioButton::indicator::checked{image:url(:/images/check_filled.png)} QRadioButton::indicator::checked:pressed{image:url(:/images/check_filled.png)} QRadioButton::indicator::checked:hover{image:url(:/images/check_filled_hover.png)}");
        tempButton->setFont (QFont ("Helvetica", 11));
        tempButton->setVisible(false);
        _optionButtonGroup->addButton(tempButton, i);
        layout->addWidget(tempButton, j%2==1?row++:row, j%2, 1, 1);
    }
    _teacherCommentLabel = new QLabel(parent);
    _teacherCommentLabel->setText(content[noOfOptions+1]);
    _teacherCommentLabel->setWordWrap(true);
    _teacherCommentLabel->setStyleSheet("QLabel{color:#97d5ee;margin-top:5px;margin-bottom:5px;}");
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

void MedNUSQuizQuestion::highlightAnswer(int correctAnswer, bool showCorrectAnswer) const
{
    QList<QAbstractButton*> buttonList = _optionButtonGroup->buttons();
    for (int i = 0; i < buttonList.size(); i++)
    {
        if (((QRadioButton*)buttonList.at(i))->isChecked()) {
            // Am I the correct answer?
            if (i+1 == correctAnswer) {
                // Set me to green.
                ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {font: bold;color: rgba(50, 255, 50, 255);}");
            } else {
                // Set me to red.
                ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {font: bold;color: rgba(255, 0, 0, 255);}");
            }
        } else {
            // Check if I should be revealing the correct answer.
            if (showCorrectAnswer && i+1 == correctAnswer) {
                // Set me to green.
                ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {font: bold;color: rgba(50, 255, 50, 255);}");
            } else {
                // Set me to translucent.
                ((QRadioButton*)buttonList.at(i))->setStyleSheet("QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {color: rgba(128, 128, 128, 255);}");
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

void MedNUSQuizQuestion::setNotice(bool value) {
    if (value) {
        _questionTextLabel->setStyleSheet("QLabel{margin-top:8px;margin-bottom:4px;padding:10px;background:rgba(229,165,57,255);border-top-left-radius: 16px;border-bottom-right-radius: 16px;}");
    } else {
        _questionTextLabel->setStyleSheet("QLabel{margin-top:8px;margin-bottom:4px;padding:10px;background:rgba(0,0,0,50);border-top-left-radius: 16px;border-bottom-right-radius: 16px;}");
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

MedNUSQuiz::MedNUSQuiz(QString filename, QWidget *parent) :
    QWidget(parent)
{
    this->setAccessibleName(filename);
    _tempWidget = new QWidget(parent);
    _layout = new QGridLayout(_tempWidget);
    _layout->setContentsMargins(0,0,0,0);

    int row = 0;
    // Add questions here //
    _questionList = new QVector<MedNUSQuizQuestion*>();

    // Read Json file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file.";

        // TODO: Add warning message box.
        //To Miki, Check with Rey to standardise file not found output.
    }
    QByteArray fileValue = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(fileValue);
    QJsonObject obj = doc.object();
    QJsonValue v = obj.value(QString("details"));
    QJsonObject quizDetails = v.toObject();

    // Quiz title text label
    _titleTextLabel = new QLabel(quizDetails["title"].toString(), _tempWidget);
    _titleTextLabel->setFont (QFont ("Helvetica", 18,QFont::Bold,false));
    _layout->addWidget(_titleTextLabel, row++, 0, 1, 2);

    _authorTextLabel = new QLabel("by "+quizDetails["author"].toString(), _tempWidget);
    _authorTextLabel->setFont(QFont("Helvetica", 9, QFont::Normal, true));
    _layout->addWidget(_authorTextLabel, row++, 0, 1, 1);

    // The instruction text at the top of the quiz.
    _instructionTextLabel = new QLabel(quizDetails["introductionText"].toString(), _tempWidget);
    _instructionTextLabel->setWordWrap(true);
    _instructionTextLabel->setStyleSheet("QLabel {color:#adbfc6;}");
    _instructionTextLabel->setFont (QFont ("Helvetica", 11,QFont::Normal,true));
    _layout->addWidget(_instructionTextLabel, row++, 0, 1, 2);

    _score = new QLabel(TEXT_QUIZ_WARNING, _tempWidget);
    _score->setStyleSheet("QLabel{color:#e5a539;margin-top:10px;margin-bottom:10px;padding:5px;background:rgba(0,0,0,100);border-top-left-radius: 30px;border-bottom-right-radius: 30px;}");
    _score->setFont (QFont ("Helvetica", 90,QFont::Bold,true));
    _score->setAlignment(Qt::AlignCenter);
    _score->setVisible(false);
    _layout->addWidget(_score, row++, 0, 1, 2);

    //_startScreenLayout = new QVBoxLayout(_tempWidget);
    _startScreenLabel = new QLabel(TEXT_QUIZ_INTRO, _tempWidget);
    _startScreenLabel->setWordWrap(true);
    _startScreenLabel->setStyleSheet("text-align:justify;");
    _startScreenLabel->setFont (QFont ("Helvetica", 12));
    _layout->addWidget(_startScreenLabel, row++, 0, 1, 2);

    // Quiz flags
    _showAnswerFlag = quizDetails["showAnswer"].toBool();
    _showTeacherCommentFlag = quizDetails["showTeacherComment"].toBool();

    int noOfQuestions = quizDetails["noOfQuestions"].toInt();

    // Load the timer
    _hasTimeLimit = quizDetails["hasTimeLimit"].toBool();
    if (_hasTimeLimit) {
        _timer = new QTimer(_tempWidget);
        connect(_timer, SIGNAL(timeout()), this, SLOT(callMarkQuiz_byTimer()));
        _timerDuration = quizDetails["timerDuration"].toInt();
        _timer->setSingleShot(true);

        _timerLabel = new QLabel(QString::number(_timer->remainingTime()), _tempWidget);
        _timerLabel->setWordWrap(true);
        _timerLabel->setStyleSheet("QLabel {color:rgba(229,164,57);margin-top:10px}");
        _timerLabel->setFont (QFont ("Helvetica", 14,QFont::Bold,false));
        _timerLabel->setVisible(false);
        _layout->addWidget(_timerLabel, row++, 0, 1, 2);

        _labelUpdateTimer = new QTimer(_tempWidget);
        connect(_labelUpdateTimer, SIGNAL(timeout()), this, SLOT(updateTimerLabel()));

        _timedQuizWarningTextLabel = new QLabel(TEXT_QUIZ_TIMED_QUIZ_WARNING, _tempWidget);
        _timedQuizWarningTextLabel->setStyleSheet("QLabel{color:#e5a539;margin-top:10px;margin-bottom:10px;padding:5px;background:rgba(0,0,0,100);border-top-left-radius: 30px;border-bottom-right-radius: 30px;}");
        _timedQuizWarningTextLabel->setFont (QFont ("Helvetica", 18,QFont::Bold,true));
        _timedQuizWarningTextLabel->setWordWrap(true);
        _layout->addWidget(_timedQuizWarningTextLabel, row++, 0, 1, 2);

        // Alternative way:
        //QTimer::singleShot(quizDetails["timerDuration"].toInt(), this, SLOT(callMarkQuiz_byTimer()));
    }

    // Start Quiz button
    _startQuizButton = new QPushButton("Start Quiz", _tempWidget);
    _startQuizButton->setStyleSheet("");
    _startQuizButton->setStyleSheet("QPushButton {margin-top:20px;background:rgba(0,0,0,255);border: 5px solid #e5a539;padding:10px;} QPushButton::pressed {background:rgba(30,30,30,255);}");
    _startQuizButton->setFont (QFont ("Helvetica", 14));
    connect(_startQuizButton, SIGNAL(released()), this, SLOT(startQuiz()));
    _layout->addWidget(_startQuizButton, row++, 0, 1, 2);
    _layout->setAlignment(Qt::AlignTop);

    _warning = new QLabel(TEXT_QUIZ_WARNING, _tempWidget);
    _warning->setStyleSheet("QLabel { color:#e5a539;margin-top:20px;margin-left:10px;margin-right:10px;}");
    _warning->setFont (QFont ("Helvetica", 11,QFont::Bold,false));
    _warning->setVisible(false);
    _layout->addWidget(_warning, row++, 0, 1, 2);

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
        MedNUSQuizQuestion *question = new MedNUSQuizQuestion(_tempWidget, _layout, row, content, noOfOptions, jsonQuestion["hasImage"].toBool(), QString(jsonQuestion["imageUrl"].toString()));

        // Push the question into the question vector.
        _questionList->append(question);
        content.clear();
    }

    // Marking button
    _markButton = new QPushButton("Submit", _tempWidget);
    _markButton->setVisible(false);
    _markButton->setStyleSheet("QPushButton {margin-top:20px;background:rgba(0,0,0,255);border: 5px solid #e5a539;padding:10px;} QPushButton::pressed {background:rgba(30,30,30,255);}");
    _markButton->setFont (QFont ("Helvetica", 14));
    connect(_markButton, SIGNAL(released()), this, SLOT(callMarkQuiz_byButton()));
    _layout->addWidget(_markButton, row++, 0, 1, 2);
    //_layout->setAlignment(Qt::AlignTop);

    _tempWidget->setLayout(_layout);
    _tempWidget->setContentsMargins(10,10,10,10);
    _tempWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _scrollArea = new QScrollArea(this);
    _scrollArea->setContentsMargins(20,20,20,20);
    _scrollArea->setWidgetResizable(true);
    _scrollArea->setWidget(_tempWidget);
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

void MedNUSQuiz::markQuiz(bool byTimer)
{
    if (_timer) {
        _timer->stop();
        _labelUpdateTimer->stop();
        _timerLabel->setVisible(false);
    }
    bool questionAnswered=false;
    bool isAnswered=true;

    _warning->setVisible(false);

    // Check if all the questions have been answered.
    if (!byTimer) {
        for (int i = 0; i < _questionList->size(); i++) {
            questionAnswered=((MedNUSQuizQuestion*)_questionList->at(i))->oneOptionSelected();
            ((MedNUSQuizQuestion*)_questionList->at(i))->setNotice(!questionAnswered);
            if (!questionAnswered) {
               isAnswered=false;
            }
        }
        if(!isAnswered) {
            _warning->setVisible(true);
            return;
        }
    }

    // Start the marking of the quiz.
    int score = 0;
    for (int i = 0; i < _questionList->size(); i++)
    {
        if (((MedNUSQuizQuestion*)_questionList->at(i))->getSelectedAnswer() == _correctAnswerList[i])
            score++;

        // Show the correct answers only if the teacher wants to.
        ((MedNUSQuizQuestion*)_questionList->at(i))->highlightAnswer(_correctAnswerList[i], _showAnswerFlag);
    }
    _score->setText(QString::number(score)+"/"+QString::number(_questionList->size()));
    _score->setVisible(true);
    _markButton->setVisible(false);

    // Make the quiz scroll to the top once the quiz has been marked.
    _scrollArea->verticalScrollBar()->setSliderPosition(0);
}

void MedNUSQuiz::callMarkQuiz_byButton()
{
    markQuiz(false);
}

void MedNUSQuiz::callMarkQuiz_byTimer()
{
    markQuiz(true);
}

void MedNUSQuiz::startQuiz()
{
    // Remove all of the start screen elements.
    _startScreenLabel->setVisible(false);
    _startQuizButton->setVisible(false);
    _instructionTextLabel->setVisible(false);
    _timedQuizWarningTextLabel->setVisible(false);

    // Show all the quiz elements.
    for (int i = 0; i < _questionList->size(); i++)
    {
       ((MedNUSQuizQuestion*)_questionList->at(i))->showQuestion();
    }
    _markButton->setVisible(true);
    _timer->start(_timerDuration);
    _timerLabel->setVisible(true);
    _labelUpdateTimer->start(500);
    // hack: to force it to start showing from the actual duration instead of -1
    updateTimerLabel();
}

void MedNUSQuiz::updateTimerLabel()
{
    if (_timer->isActive()) {
        // Update the timer label.
        int time = _timer->remainingTime()/1000;
        int min = time/60;
        int sec = time%60;
        QString timeText = "Time Left: ";
        if (min < 10) {
            timeText += "0" + QString::number(min) +":";
        } else {
            timeText += QString::number(min) + ":";
        }
        if (sec < 10) {
            timeText += "0" + QString::number(sec);
        } else {
            timeText += QString::number(sec);
        }
        _timerLabel->setText(timeText);
    } else {
        // Stop the label update timer.
        _labelUpdateTimer->stop();
    }
}

void MedNUSQuiz::resizeEvent(QResizeEvent *event)
{
    _scrollArea->setGeometry(this->geometry());
}

#include "MedNUSQuiz.h"
#include "MedNUSAUISettings.h"

const int MAX_NO_OF_QUIZ_QUESTIONS = 100;

MedNUSQuiz::MedNUSQuiz(QString filename, QWidget *parent) :
    QWidget(parent)
{
    this->setAccessibleName(_filename);
    _parent = parent;
    _filename = filename;

    try
    {
        initViewerView();
    }
    catch(std::string errorMsg)
    {
        qWarning() << errorMsg.c_str();
        return;
    }

    //TODO: REMOVE Debug stuff
    if (_parent == NULL)
        qDebug() << "no parent";
    else
        qDebug() << "yes parent";
    qDebug() << "noOfElements = " << _tempWidget->children().size();

    //Loaded sucessfully!
    this->setProperty("Loaded",true);
}


MedNUSQuiz::~MedNUSQuiz()
{
    //TODO: Clean up
}

void MedNUSQuiz::initViewerView() {

    _tempWidget = new QWidget(_parent);
    _layout = new QGridLayout(_tempWidget);
    _layout->setContentsMargins(0,0,0,0);

    int row = 0;
    // Add questions here //
    _questionList = new QVector<MedNUSQuizQuestion*>();

    // Read Json file
    if(!loadQuizFileToViewer(_filename, row))
        throw "Could not load file.";

    // Marking button
    _markButton = new QPushButton("Submit", _tempWidget);
    _markButton->setVisible(false);
    _markButton->setStyleSheet(MARK_BUTTON_STYLESHEET);
    _markButton->setFont (QFont ("Helvetica", 14));
    connect(_markButton, SIGNAL(released()), this, SLOT(callMarkQuiz_byButton()));
    _layout->addWidget(_markButton, row++, 0, 1, 2);

    // Edit Quiz button
    _editQuizButton = new QPushButton("Edit Quiz", _tempWidget);
    _editQuizButton->setVisible(true);
    _editQuizButton->setStyleSheet(EDIT_BUTTON_STYLESHEET);
    _editQuizButton->setFont(QFont("Helvetica", 14));
    connect(_editQuizButton, SIGNAL(released()), this, SLOT(goToEditorView()));
    _layout->addWidget(_editQuizButton, row++, 0, 1, 2);

    _tempWidget->setLayout(_layout);
    _tempWidget->setContentsMargins(10,10,10,10);
    _tempWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _scrollArea = new QScrollArea(this);
    _scrollArea->setContentsMargins(20,20,20,20);
    _scrollArea->setWidgetResizable(true);
    _scrollArea->setWidget(_tempWidget);
    _scrollArea->setAutoFillBackground(true);
    _scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _scrollArea->setVisible(true);
    //Load scrollbar style.
    QFile file2(":/images/scrollbar.css");
    if(file2.open(QIODevice::ReadOnly|QIODevice::Text)) {
        _scrollArea->setStyleSheet(file2.readAll());
        file2.close();
    }

    _noOfCorrectAnswers = 0;
    _isEditorView = false;
}


void MedNUSQuiz::deinitViewerView() {
    delete _tempWidget;
    delete _scrollArea;
}


void MedNUSQuiz::initEditorView() {
    _tempWidget = new QWidget(_parent);
    _layout = new QGridLayout(_tempWidget);
    _layout->setContentsMargins(0,0,0,0);

    int row = 0;

    // View Quiz button
    _viewQuizButton = new QPushButton("View Quiz", _tempWidget);
    _viewQuizButton->setVisible(true);
    _viewQuizButton->setStyleSheet(EDIT_BUTTON_STYLESHEET);
    _viewQuizButton->setFont(QFont("Helvetica", 14));
    connect(_viewQuizButton, SIGNAL(released()), this, SLOT(goToViewerView()));
    _layout->addWidget(_viewQuizButton, row++, 0, 1, 2);

    // Number of Questions for quiz.
    _noOfQuestionsDropDownBox = new QComboBox(_tempWidget);
    for (int i = 1; i <= MAX_NO_OF_QUIZ_QUESTIONS; i++) {
        _noOfQuestionsDropDownBox->addItem(QString::number(i));
    }
    _noOfQuestionsDropDownBox->setMaxVisibleItems(MAX_NO_OF_QUIZ_QUESTIONS/2);
    _noOfQuestionsDropDownBox->setVisible(true);
    _listView = new QListView(_noOfQuestionsDropDownBox);
    _noOfQuestionsDropDownBox->setView(_listView);
    connect(_noOfQuestionsDropDownBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(updateNoOfQuestions()));

    // Load the quiz into the editor.
    loadQuizFileToEditor(_filename, row);

    _tempWidget->setLayout(_layout);
    _tempWidget->setContentsMargins(10,10,10,10);
    _tempWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _tempWidget->setVisible(true);
    _scrollArea = new QScrollArea(this);
    _scrollArea->setContentsMargins(20,20,20,20);
    _scrollArea->setWidgetResizable(true);
    _scrollArea->setWidget(_tempWidget);
    _scrollArea->setAutoFillBackground(true);
    _scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _scrollArea->setVisible(true);

    _isEditorView = true;
}


void MedNUSQuiz::deinitEditorView() {
    delete _tempWidget;
    delete _scrollArea;
}


int MedNUSQuiz::getNoOfCorrectAnswers() const {
    return _noOfCorrectAnswers;
}


void MedNUSQuiz::markQuiz(bool calledByTimer) {

// Stop the timer if we have one.

    if (_timer) {
        _timer->stop();
        _labelUpdateTimer->stop();
        _timerLabel->setVisible(false);
    }


// Check if all the questions have been answered.

    bool areAllQuestionsAnswered = true;
    _warning->setVisible(false);

    if (!calledByTimer) {
        for (int i = 0; i < _questionList->size(); i++) {
            MedNUSQuizQuestion* qn = (MedNUSQuizQuestion*)_questionList->at(i);
            if (!qn->oneOptionSelected()) {
                qn->setNotice(false);
                areAllQuestionsAnswered = false;
            } else {
                qn->setNotice(true);
            }
        }

        // If not all questions are answered, set a notice.
        if(!areAllQuestionsAnswered) {
            _warning->setVisible(true);
            scrollScreenToTop();
            return;
        }
    }


// Start marking.

    for (int i = 0; i < _questionList->size(); i++) {

        MedNUSQuizQuestion* qn = (MedNUSQuizQuestion*)_questionList->at(i);

        if (qn->getSelectedAnswer() == _correctAnswerList[i])
            _noOfCorrectAnswers++;

        // Show the correct answers only if the teacher wants to.
        qn->highlightAnswer(_correctAnswerList[i], _showAnswerFlag);
    }


// Show the score.

    QString scoreText = "";
    scoreText += QString::number(_noOfCorrectAnswers) + "/";
    scoreText += QString::number(_questionList->size());

    _score->setText(scoreText);
    _score->setVisible(true);
    _markButton->setVisible(false);


// Make the quiz scroll to the top once the quiz has been marked.

    scrollScreenToTop();
}

bool MedNUSQuiz::loadQuizFileToViewer(QString filename, int &row) {

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file.";
        return false;
    }
    QString lastModifiedDate = QFileInfo(file).lastModified().toString();
    //qDebug() << lastModifiedDate;
    QByteArray fileValue = file.readAll();
    file.close();


// Start parsing the quiz file data.

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

    _lastModifiedLabel = new QLabel("last modified on: "+lastModifiedDate, _tempWidget);
    _lastModifiedLabel->setFont(QFont("Helvetica", 9, QFont::Normal, true));
    _lastModifiedLabel->setAlignment(Qt::AlignRight);
    _layout->addWidget(_lastModifiedLabel, row-1, 1, 1, 1);

    // The instruction text at the top of the quiz.
    _instructionTextLabel = new QLabel(quizDetails["introductionText"].toString(), _tempWidget);
    _instructionTextLabel->setWordWrap(true);
    _instructionTextLabel->setStyleSheet(INSTRUCTION_TEXT_LABEL_STYLESHEET);
    _instructionTextLabel->setFont (QFont ("Helvetica", 11,QFont::Normal,true));
    _layout->addWidget(_instructionTextLabel, row++, 0, 1, 2);

    _dummySpace1 = new QLabel("-", _tempWidget);
    _dummySpace1->setStyleSheet(DUMMY_SPACE_STYLESHEET);
    _dummySpace1->setFont (QFont ("Helvetica", 14, QFont::Bold));
    _layout->addWidget(_dummySpace1, row++, 0, 1, 2, Qt::AlignCenter);

    _score = new QLabel(TEXT_QUIZ_WARNING, _tempWidget);
    _score->setStyleSheet(SCORE_TEXT_LABEL_STYLESHEET);
    _score->setFont (QFont ("Helvetica", 90,QFont::Bold,true));
    _score->setAlignment(Qt::AlignCenter);
    _score->setVisible(false);
    _layout->addWidget(_score, row++, 0, 1, 2);

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
        _timerLabel->setStyleSheet(TIMER_LABEL_STYLESHEET);
        _timerLabel->setFont (QFont ("Helvetica", 14,QFont::Bold,false));
        _timerLabel->setVisible(false);
        _layout->addWidget(_timerLabel, row++, 0, 1, 2);

        _labelUpdateTimer = new QTimer(_tempWidget);
        connect(_labelUpdateTimer, SIGNAL(timeout()), this, SLOT(updateTimerLabel()));

        _timedQuizWarningTextLabel = new QLabel(TEXT_QUIZ_TIMED_QUIZ_WARNING + QString("\n") + convertTimeToString(quizDetails["timerDuration"].toInt()), _tempWidget);
        _timedQuizWarningTextLabel->setStyleSheet(TIMED_QUIZ_WARNING_STYLESHEET);
        _timedQuizWarningTextLabel->setFont (QFont ("Helvetica", 12));
        _timedQuizWarningTextLabel->setWordWrap(true);
        _layout->addWidget(_timedQuizWarningTextLabel, row++, 0, 1, 1);
    }

    // Attempts
    _attemptLabel = new QLabel("Attempts Left:\n3/5", _tempWidget);
    _attemptLabel->setStyleSheet(ATTEMPT_LABEL_STYLESHEET);
    _attemptLabel->setAlignment(Qt::AlignRight);
    _attemptLabel->setFont (QFont ("Helvetica", 12));
    _attemptLabel->setWordWrap(true);
    _layout->addWidget(_attemptLabel, _hasTimeLimit?row-1:row, 1, 1, 1);

    // Start Quiz button
    _startQuizButton = new QPushButton("Start Quiz", _tempWidget);
    _startQuizButton->setStyleSheet(START_QUIZ_BUTTON_STYLESHEET);
    _startQuizButton->setFont (QFont ("Helvetica", 14));
    connect(_startQuizButton, SIGNAL(released()), this, SLOT(startQuiz()));
    _layout->addWidget(_startQuizButton, row++, 0, 1, 2);
    _layout->setAlignment(Qt::AlignTop);

    _warning = new QLabel(TEXT_QUIZ_WARNING, _tempWidget);
    _warning->setStyleSheet(WARNING_LABEL_STYLESHEET);
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
        MedNUSQuizQuestion *question =
                new MedNUSQuizQuestion(_tempWidget, _layout, row, content,
                                       noOfOptions,
                                       jsonQuestion["hasImage"].toBool(),
                QString(jsonQuestion["imageUrl"].toString()));

        // Push the question into the question vector.
        _questionList->append(question);
        content.clear();
    }
    return true;
}

void MedNUSQuiz::loadQuizFileToEditor(QString filename, int &row) {

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file.";

        // TODO: Add warning message box.
        //To Miki, Check with Rey to standardise file not found output.
    }
    QString lastModifiedDate = QFileInfo(file).lastModified().toString();
    //qDebug() << lastModifiedDate;
    QByteArray fileValue = file.readAll();
    file.close();


// Start parsing the quiz file data.

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

    _lastModifiedLabel = new QLabel("last modified on: "+lastModifiedDate, _tempWidget);
    _lastModifiedLabel->setFont(QFont("Helvetica", 9, QFont::Normal, true));
    _lastModifiedLabel->setAlignment(Qt::AlignRight);
    _layout->addWidget(_lastModifiedLabel, row-1, 1, 1, 1);

    // The instruction text at the top of the quiz.
    _instructionTextLabel = new QLabel(quizDetails["introductionText"].toString(), _tempWidget);
    _instructionTextLabel->setWordWrap(true);
    _instructionTextLabel->setStyleSheet(INSTRUCTION_TEXT_LABEL_STYLESHEET);
    _instructionTextLabel->setFont (QFont ("Helvetica", 11,QFont::Normal,true));
    _layout->addWidget(_instructionTextLabel, row++, 0, 1, 2);

    _dummySpace1 = new QLabel("-", _tempWidget);
    _dummySpace1->setStyleSheet(DUMMY_SPACE_STYLESHEET);
    _dummySpace1->setFont (QFont ("Helvetica", 14, QFont::Bold));
    _layout->addWidget(_dummySpace1, row++, 0, 1, 2, Qt::AlignCenter);

    _score = new QLabel(TEXT_QUIZ_WARNING, _tempWidget);
    _score->setStyleSheet(SCORE_TEXT_LABEL_STYLESHEET);
    _score->setFont (QFont ("Helvetica", 90,QFont::Bold,true));
    _score->setAlignment(Qt::AlignCenter);
    _score->setVisible(false);
    _layout->addWidget(_score, row++, 0, 1, 2);

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
        _timerLabel->setStyleSheet(TIMER_LABEL_STYLESHEET);
        _timerLabel->setFont (QFont ("Helvetica", 14,QFont::Bold,false));
        _timerLabel->setVisible(false);
        _layout->addWidget(_timerLabel, row++, 0, 1, 2);

        _labelUpdateTimer = new QTimer(_tempWidget);
        connect(_labelUpdateTimer, SIGNAL(timeout()), this, SLOT(updateTimerLabel()));

        _timedQuizWarningTextLabel = new QLabel(TEXT_QUIZ_TIMED_QUIZ_WARNING + QString("\n") + convertTimeToString(quizDetails["timerDuration"].toInt()), _tempWidget);
        _timedQuizWarningTextLabel->setStyleSheet(TIMED_QUIZ_WARNING_STYLESHEET);
        _timedQuizWarningTextLabel->setFont (QFont ("Helvetica", 12));
        _timedQuizWarningTextLabel->setWordWrap(true);
        _layout->addWidget(_timedQuizWarningTextLabel, row++, 0, 1, 1);
    }

    // Attempts
    _attemptLabel = new QLabel("Attempts Left:\n3/5", _tempWidget);
    _attemptLabel->setStyleSheet(ATTEMPT_LABEL_STYLESHEET);
    _attemptLabel->setAlignment(Qt::AlignRight);
    _attemptLabel->setFont (QFont ("Helvetica", 12));
    _attemptLabel->setWordWrap(true);
    _layout->addWidget(_attemptLabel, _hasTimeLimit?row-1:row, 1, 1, 1);

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
        MedNUSQuizQuestion *question =
                new MedNUSQuizQuestion(_tempWidget, _layout, row, content,
                                       noOfOptions,
                                       jsonQuestion["hasImage"].toBool(),
                QString(jsonQuestion["imageUrl"].toString()));

        // Force the question to be shown.
        question->showQuestion();

        // Push the question into the question vector.
        _questionList->append(question);
        content.clear();
    }
}

void MedNUSQuiz::updateNoOfQuestions()
{
    qDebug() << _noOfQuestionsDropDownBox->currentIndex() + 1;

    _noOfQuestions = _noOfQuestionsDropDownBox->currentIndex() + 1;
}



QString MedNUSQuiz::convertTimeToString(int ms)
{
    QString timeString = "";

    int time = ms/1000;
    int hr = time/3600;
    int min = time/60;
    int sec = time%60;

    if (hr < 10) {
        timeString += "0" + QString::number(hr) + ":";
    } else {
        timeString += QString::number(hr) + ":";
    }

    if (min < 10) {
        timeString += "0" + QString::number(min) + ":";
    } else {
        timeString += QString::number(min) + ":";
    }

    if (sec < 10) {
        timeString += "0" + QString::number(sec);
    } else {
        timeString += QString::number(sec);
    }


    return timeString;
}

void MedNUSQuiz::scrollScreenToTop()
{
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
    _attemptLabel->setVisible(false);
    _dummySpace1->setVisible(false);

    // Show all the quiz elements.
    for (int i = 0; i < _questionList->size(); i++)
       ((MedNUSQuizQuestion*)_questionList->at(i))->showQuestion();

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
        int hr = time/3600;
        int min = time/60;
        int sec = time%60;
        QString timeText = "Time Left: ";
        if (hr < 10) {
            timeText += "0" + QString::number(hr) + ":";
        } else {
            timeText += QString::number(hr) + ":";
        }
        if (min < 10) {
            timeText += "0" + QString::number(min) + ":";
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

void MedNUSQuiz::goToEditorView() {

    deinitViewerView();
    initEditorView();

    _isEditorView = true;
}

void MedNUSQuiz::goToViewerView() {

    deinitEditorView();
    initViewerView();

    _isEditorView = false;
}


void MedNUSQuiz::resizeEvent(QResizeEvent *event)
{
    _scrollArea->setGeometry(this->geometry());
}

#include "MedNUSQuiz.h"

MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent, QVBoxLayout *layout, QVector<QString> content, int noOfOptions)
{
    _optionButtonGroup = new QButtonGroup(parent);

    // Initialize the QLabel
    _questionTextLabel = new QLabel(content[0]);
    _questionTextLabel->setStyleSheet("QLabel { color : white; }");
    _questionTextLabel->setGeometry(parent->geometry());
    _questionTextLabel->setWordWrap(true);
    layout->addWidget(_questionTextLabel);

    // Initialize the buttons
    QRadioButton* tempButton;
    for (int i = 1; i <= noOfOptions; i++)
    {
        tempButton = new QRadioButton(content[i]);
        tempButton->setStyleSheet("QRadioButton { color : white; }");
        _optionButtonGroup->addButton(tempButton, i);
        layout->addWidget(tempButton);
    }
}

MedNUSQuizQuestion::~MedNUSQuizQuestion()
{
    //TODO: Do a proper clean up
    //QTextStream(stdout) << "HELLO I'm being destroyed! D=";
    // Clean up the label
    delete _questionTextLabel;
    _questionTextLabel = NULL;

    // Clean up the buttons
    delete _optionButtonGroup;
    _optionButtonGroup = NULL;
}

void MedNUSQuizQuestion::myForceResize()
{
    QTextStream(stdout) << "resize";
    // Resize the QLabel geometry as well as the radio buttons
    //TODO: Fix this
    //_questionTextLabel->setGeometry(((QWidget*)_questionTextLabel->parent())->geometry());
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
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString fileValue = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(fileValue.toUtf8());
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

        // Create the question.
        MedNUSQuizQuestion *question = new MedNUSQuizQuestion(_tempWidget, _layout, content, noOfOptions);

        // Push the question into the question vector.
        _questionList->append(question);
        content.clear();
    }

    // Marking button
    _markButton = new QPushButton("Check Answers");
    _markButton->setGeometry(QRect(QPoint(100, 100), QSize(50, 50)));
    _markButton->setStyleSheet("QPushButton { color : white; }");
    connect(_markButton, SIGNAL(released()), this, SLOT(markQuiz()));
    _layout->addWidget(_markButton);
    _layout->setAlignment(Qt::AlignTop);

    _tempWidget->setLayout(_layout);
    _scrollArea = new QScrollArea(this);
    _scrollArea->setObjectName(QStringLiteral("_scrollArea"));
    _scrollArea->setWidgetResizable(true);
    _scrollArea->setContentsMargins(0,0,0,0);
    _scrollArea->setWidget(_tempWidget);
    _scrollArea->setGeometry(_tempWidget->geometry());
    _scrollArea->setAutoFillBackground(true);
}

MedNUSQuiz::~MedNUSQuiz()
{
    delete _layout;

    // NEW
    _questionList->clear();
    delete _questionList;
    _questionList = NULL;
    // END NEW
}

void MedNUSQuiz::markQuiz()
{
    QTextStream(stdout) << "no more quiz marking for now.";
}

void MedNUSQuiz::resizeEvent(QResizeEvent *event)
{
    QTextStream(stdout) << "resize quiz";
    _scrollArea->setGeometry(this->geometry());

    // Go through every QLabel in _layout and setGeomtry accordingly
    for (int i = 0; i < _questionList->size(); i++)
    {
        ((MedNUSQuizQuestion*)_questionList->at(i))->myForceResize();
    }
}

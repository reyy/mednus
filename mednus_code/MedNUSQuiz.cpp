#include "MedNUSQuiz.h"

MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent, QVBoxLayout *layout)
{
    _optionButtonGroup = new QButtonGroup(parent);

    // Initialize the QLabel
    _questionTextLabel = new QLabel("Q1. There are _____ cranial bones and _____ facial bones in the adult skull.");
    _questionTextLabel->setStyleSheet("QLabel { color : white; }");
    _questionTextLabel->setGeometry(parent->geometry());
    _questionTextLabel->setWordWrap(true);
    layout->addWidget(_questionTextLabel);

    // Initialize the buttons
    QRadioButton* tempButton;
    tempButton = new QRadioButton("A) 6; 10");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    _optionButtonGroup->addButton(tempButton, 1);
    layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) 8; 14");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    _optionButtonGroup->addButton(tempButton, 2);
    layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) 12; 12");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    _optionButtonGroup->addButton(tempButton, 3);
    layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) 5; 9");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    _optionButtonGroup->addButton(tempButton, 4);
    layout->addWidget(tempButton);
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
    // NEW
    for (int i = 0; i < 10; i++)
    {
        _questionList = new QVector<MedNUSQuizQuestion*>();
        _questionList->append(new MedNUSQuizQuestion(_tempWidget, _layout));
    }
    // END NEW

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

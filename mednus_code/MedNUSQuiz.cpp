#include "MedNUSQuiz.h"

MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent, QGroupBox *box)
{
    //box->addWidget(new QLabel("This is a question that has to be answered."));

    _optionButtonGroup = new QButtonGroup(box);
    _optionButtonGroup->addButton(new QRadioButton("option 1", parent));
    _optionButtonGroup->addButton(new QRadioButton("option 2", parent));
    _optionButtonGroup->addButton(new QRadioButton("option 3", parent));
    _optionButtonGroup->addButton(new QRadioButton("option 4", parent));
}

MedNUSQuizQuestion::~MedNUSQuizQuestion()
{
    delete _option1;
}

MedNUSQuiz::MedNUSQuiz(QString filename, QWidget *parent) :
    QWidget(parent)
{
    _tempWidget = new QWidget(parent);
    _layout = new QVBoxLayout(_tempWidget);

    // Add questions here
    QButtonGroup *tempButtonGroup;
    QRadioButton *tempButton;
    QLabel *tempLabel;

    for (int i = 0; i < 8; i++)
    {
        tempButtonGroup = new QButtonGroup(_tempWidget);
        tempLabel = new QLabel("Q1. There are _____ cranial bones and _____ facial bones in the adult skull.");
        tempLabel->setStyleSheet("QLabel { color : white; }");
        tempLabel->setGeometry(_tempWidget->geometry());
        tempLabel->setWordWrap(true);
        _layout->addWidget(tempLabel);
        tempButton = new QRadioButton("A) 6; 10");
        tempButton->setStyleSheet("QRadioButton { color : white; }");
        tempButtonGroup->addButton(tempButton, 1);
        _layout->addWidget(tempButton);
        tempButton = new QRadioButton("B) 8; 14");
        tempButton->setStyleSheet("QRadioButton { color : white; }");
        tempButtonGroup->addButton(tempButton, 2);
        _layout->addWidget(tempButton);
        tempButton = new QRadioButton("C) 12; 12");
        tempButton->setStyleSheet("QRadioButton { color : white; }");
        tempButtonGroup->addButton(tempButton, 3);
        _layout->addWidget(tempButton);
        tempButton = new QRadioButton("D) 5; 9");
        tempButton->setStyleSheet("QRadioButton { color : white; }");
        tempButtonGroup->addButton(tempButton, 4);
        _layout->addWidget(tempButton);
        _questionOptionGroup.append(tempButtonGroup);
        _correctAnswerList.append(3);
    }

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
}

void MedNUSQuiz::markQuiz()
{
    _score = 0;
    // go to each button group and see
    // what is the correct answer.
    // add up the score and report to the user.
    for (int i = 0; i < 2; i++)
    {
        QTextStream(stdout) << "Question " << i+1 << ": ";
        //QTextStream(stdout) << _questionOptionGroup[i]->checkedId() << endl;
        if (_questionOptionGroup[i]->checkedId() == _correctAnswerList[i])
        {
            _score++;
            QTextStream(stdout) << "CORRECT\n";
        }
        else
        {
            QTextStream(stdout) << "WRONG\n";
        }
    }

    QTextStream(stdout) << "SCORE: " << _score << endl;

    _scoreMsgBox = new QMessageBox();
    QString text = "SCORE: ";
    text += QString::number(_score);
    _scoreMsgBox->setText(text);
    _scoreMsgBox->exec();
}

void MedNUSQuiz::resizeEvent(QResizeEvent *event)
{
    _scrollArea->setGeometry(this->geometry());

    // Go through every QLabel in _layout and setGeomtry accordingly
}

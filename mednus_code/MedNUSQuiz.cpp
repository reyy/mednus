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
    _content = new QGroupBox(this);
    _layout = new QVBoxLayout(this);

    // Add questions here
    QButtonGroup *temp;
    QRadioButton *tempButton;

    temp = new QButtonGroup(_content);
    _layout->addWidget(new QLabel("The exchange of oxygen and carbon dioxide between air, blood and body tisses is known as:"));
    tempButton = new QRadioButton("Inspiration");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("Expiration");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("Respiration");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("Perspiration");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(3);

    temp = new QButtonGroup(_content);
    _layout->addWidget(new QLabel("Breathing is an automatic process controlled by the "));
    tempButton = new QRadioButton("Medulla");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("Hypothalmus");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("Lymph Nodes");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("Hypocampus");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    _markButton = new QPushButton("Check Answers");
    _markButton->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
    connect(_markButton, SIGNAL(released()), this, SLOT(markQuiz()));
    _layout->addWidget(_markButton);

    _content->setLayout(_layout);
}

MedNUSQuiz::~MedNUSQuiz()
{
    delete _layout;
    delete _content;

    //_questionList->clear();
    //delete _questionList;
    //_questionList = NULL;
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
}

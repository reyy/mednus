#include "MedNUSQuiz.h"

MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent, QVBoxLayout *layout)
{
    layout->addWidget(new QLabel("This is a question that has to be answered."));

    for (int i = 0; i < 5; i++)
    {
        layout->addWidget(new QRadioButton(QString("option "+i), parent));
    }
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

    _questionList = new QVector<MedNUSQuizQuestion*>();
    _questionList->append(new MedNUSQuizQuestion(this, _layout));
    _questionList->append(new MedNUSQuizQuestion(this, _layout));
    _questionList->append(new MedNUSQuizQuestion(this, _layout));

    _content->setLayout(_layout);
}

MedNUSQuiz::~MedNUSQuiz()
{
    delete _layout;
    delete _content;

    _questionList->clear();
    delete _questionList;
    _questionList = NULL;
}

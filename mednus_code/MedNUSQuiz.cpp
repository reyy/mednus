#include "MedNUSQuiz.h"

MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent)
{
    //_option1 = new QRadioButton("option 1", parent);

    _content = new QGroupBox(parent);

    QVBoxLayout* layout = new QVBoxLayout(parent);

    layout->addWidget(new QLabel("This is a question that has to be answered."));

    for (int i = 0; i < 5; i++)
    {
        layout->addWidget(new QRadioButton(QString("option "+i), parent));
    }

    //layout->addWidget(_option1);
    _content->setLayout(layout);
}

MedNUSQuizQuestion::~MedNUSQuizQuestion()
{
    delete _option1;
    delete _content;
}

MedNUSQuiz::MedNUSQuiz(QString filename, QWidget *parent) :
    QWidget(parent)
{
    _questionList = new QVector<MedNUSQuizQuestion*>();
    _questionList->append(new MedNUSQuizQuestion(this));
}

MedNUSQuiz::~MedNUSQuiz()
{
    _questionList->clear();
    delete _questionList;
    _questionList = NULL;
}

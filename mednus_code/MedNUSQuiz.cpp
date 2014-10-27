#include "MedNUSQuiz.h"

MedNUSQuiz::MedNUSQuiz(QString filename, QWidget *parent) :
    QWidget(parent)
{
    // Hello
    someText = new QLabel("Hello", this);
}

MedNUSQuiz::~MedNUSQuiz()
{
    delete someText;
}

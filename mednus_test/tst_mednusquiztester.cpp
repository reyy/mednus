#include "tst_mednusquiztester.h"

MedNUSQuizTester::MedNUSQuizTester() {
    //quiz = new MedNUSQuiz("~mednus/lesson1/quiz/Quiz - The Skull.qiz");
}


MedNUSQuizTester::~MedNUSQuizTester() {
    //delete quiz;
    //quiz = NULL;
}


void MedNUSQuizTester::initTestCase() {

}


void MedNUSQuizTester::cleanupTestCase() {

}


void MedNUSQuizTester::markQuizTestCase() {
    quiz->markQuiz(true);

    QVERIFY(quiz->getNoOfCorrectAnswers() == 0);
}


void MedNUSQuizTester::helloTestCase() {

}


void MedNUSQuizTester::editQuizTitleNormalTestCase() {
    MedNUSQuiz* tempQuiz = new MedNUSQuiz(QDir::homePath()+"mednus/lesson1/quiz/Quiz - The Skull.qiz", LECTURER);

    const QString data = "A new quiz title.";
    int dataLength = data.length();

    tempQuiz->_titleTextLabel = new QLineEdit(data);

    int extractedLength = ((QLineEdit*)tempQuiz->_titleTextLabel)->text().length();

    QVERIFY(dataLength == extractedLength);
}


void MedNUSQuizTester::editQuizTitleZeroTestCase() {
    MedNUSQuiz* tempQuiz = new MedNUSQuiz(QDir::homePath()+"mednus/lesson1/quiz/Quiz - The Skull.qiz", LECTURER);

    QString data = "";
    int dataLength = data.length();

    tempQuiz->_titleTextLabel = new QLineEdit(data);

    int extractedLength = ((QLineEdit*)tempQuiz->_titleTextLabel)->text().length();

    QVERIFY(dataLength == extractedLength);
}


void MedNUSQuizTester::editQuizTitleOverBoundaryTestCase() {
    MedNUSQuiz* tempQuiz = new MedNUSQuiz(QDir::homePath()+"mednus/lesson1/quiz/Quiz - The Skull.qiz", LECTURER);

    QString data = "";

    for (int i = 0; i < 32768; i++) {
        data.append("a");
    }

    int dataLength = data.length();

    tempQuiz->_titleTextLabel = new QLineEdit(data);

    int extractedLength = ((QLineEdit*)tempQuiz->_titleTextLabel)->text().length();

    QVERIFY(dataLength != extractedLength);
}

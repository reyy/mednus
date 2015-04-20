#include "tst_mednusquiztester.h"

MedNUSQuizTester::MedNUSQuizTester() {
    quiz = new MedNUSQuiz("~mednus/lesson1/quiz/Quiz - The Skull.qiz");
}


MedNUSQuizTester::~MedNUSQuizTester() {
    delete quiz;
    quiz = NULL;
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
    MedNUSQuiz* tempQuiz = new MedNUSQuiz("~mednus/lesson1/quiz/Quiz - The Skull.qiz");
}


void MedNUSQuizTester::editQuizTitleZeroTestCase() {

}


void MedNUSQuizTester::editQuizTitleOverBoundaryTestCase() {

}

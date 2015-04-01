#include <QString>
#include <QtTest>
#include <QCoreApplication>
#define protected public
#include "MedNUSQuiz.h"

class MedNUSQuizTester : public QObject
{
public:
    MedNUSQuizTester();
    ~MedNUSQuizTester();

private:
    Q_OBJECT
    MedNUSQuiz *quiz;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void markQuizTestCase();
    void helloTestCase();
};

MedNUSQuizTester::MedNUSQuizTester()
{
    quiz = new MedNUSQuiz("~mednus/lesson1/quiz/Quiz - The Skull.qiz");
}
MedNUSQuizTester::~MedNUSQuizTester()
{
    delete quiz;
}

void MedNUSQuizTester::initTestCase() {

}

void MedNUSQuizTester::cleanupTestCase() {

}

void MedNUSQuizTester::markQuizTestCase()
{
    quiz->markQuiz(true);

    QVERIFY(quiz->getNoOfCorrectAnswers() == 0);
}

void MedNUSQuizTester::helloTestCase() {

}

//QTEST_MAIN(MedNUSQuizTester)

#include "tst_mednusquiztester.moc"

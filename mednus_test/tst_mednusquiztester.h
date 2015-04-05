#ifndef _TST_MEDNUSQUIZTESTER_H_
#define _TST_MEDNUSQUIZTESTER_H_

#include <QString>
#include <QtTest>
#include <QCoreApplication>
#define protected public
#include "MedNUSQuiz.h"

class MedNUSQuizTester : public QObject {
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

//#include "tst_mednusquiztester.moc"

#endif

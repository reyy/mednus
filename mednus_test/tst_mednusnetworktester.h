#ifndef _TST_MEDNUSNETWORKTESTER_H_
#define _TST_MEDNUSNETWORKTESTER_H_

#include <QString>
#include <QtTest>
#include <QCoreApplication>
#define protected public
#include "MedNUSNetwork.h"

class MedNUSNetworkTester : public QObject
{
public:
    MedNUSNetworkTester();
    ~MedNUSNetworkTester();

private:
    Q_OBJECT
    MedNUSNetwork *network;

private Q_SLOTS:
    void initTestCase();
    void loginTest();
    void downloadLessonListTest();
    void BluebellHTTPSTest();
    void cleanupTestCase();
};

//QTEST_MAIN(MedNUSNetworkTester)

//#include "tst_mednusnetworktester.moc"

#endif

#include <QString>
#include <QtTest>
#include <QCoreApplication>
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
    void cleanupTestCase();
};

MedNUSNetworkTester::MedNUSNetworkTester()
{
    network = new MedNUSNetwork();
}
MedNUSNetworkTester::~MedNUSNetworkTester()
{
    delete network;
}

void MedNUSNetworkTester::initTestCase()
{

}

void MedNUSNetworkTester::cleanupTestCase()
{

}

void MedNUSNetworkTester::loginTest()
{

    QSignalSpy spy(network, SIGNAL(loginResults(bool,QString,QString)));
//    //QVERIFY2(false, "Failure");
    network->login("NUSSTU\a0111823","123", true);
    spy.wait();
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).type() == QVariant::Bool);
    QVERIFY(arguments.at(1).type() == QVariant::String);
    QVERIFY(arguments.at(2).type() == QVariant::String);
}

QTEST_MAIN(MedNUSNetworkTester)

#include "tst_mednusnetworktester.moc"

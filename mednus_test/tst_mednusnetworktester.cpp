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
    void BluebellHTTPSTest();
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

void MedNUSNetworkTester::BluebellHTTPSTest()
{
    QSignalSpy m_pSignalSpy(&(network->mgr), SIGNAL(finished(QNetworkReply*)));
    network->fileDownload();
    m_pSignalSpy.wait();

    for(int index = 0; index<m_pSignalSpy.size(); index++)
          {
        QList<QVariant> listItem = m_pSignalSpy.value(index);

        for(int index2 = 0;index2 < listItem.size();index2++)
        {
          qDebug(listItem[index2].toString().toStdString().c_str());
        }
          }

//    QNetworkReply* temp  = static_cast<QNetworkReply*>(&(spy.takeFirst()[0]));
//    char *c;
//    temp->getChar(c);
//    qDebug(c);
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

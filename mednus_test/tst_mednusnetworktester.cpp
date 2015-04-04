#include "tst_mednusnetworktester.h"

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
    //Initialize SSL Certs
    QFile file(":/ssl/bluebell.crt");
    file.open(QIODevice::ReadOnly);
    const QByteArray bytes = file.readAll();

    //Add as default crt
    const QSslCertificate certificate(bytes);
    QSslSocket::addDefaultCaCertificate(certificate);
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

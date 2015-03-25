#ifndef MEDNUSNETWORK_H
#define MEDNUSNETWORK_H

#include <QSettings>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QVariant>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QFile>
#include <QSslCertificate>
#include <QSslSocket>

class MedNUSNetwork : public QObject
{
    Q_OBJECT
public:
    explicit MedNUSNetwork(QObject *parent = 0);
    void tryAutoLogin();

protected:
    const static QString _baseURL;
    QString token;
    QNetworkAccessManager mgr;

    bool isLocked = false;
    bool rememberMe = false;

    void verifyToken();
    void getProfile();
    void checkTokenReply(QJsonObject);
    void loginReply(QJsonObject);
    void profileReply(QJsonObject);

    void fileDownload();

signals:
    void showLoadingScreen(bool);
    void loginResults(bool, QString, QString);

public slots:
    void login(QString matric, QString password, bool r);
    void logout();

protected slots:
    void getReply(QNetworkReply*);
    void handleSslErrors(QNetworkReply* reply, const QList<QSslError> &errors);
};


#endif // MEDNUSNETWORK_H

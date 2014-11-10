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

class MedNUSNetwork : public QObject
{
    Q_OBJECT
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
public:
    explicit MedNUSNetwork(QObject *parent = 0);

signals:
    void loginResults(bool,QString,QString);
public slots:
    void login(QString matric, QString password, bool r);
    void logout();
protected slots:
    void getReply(QNetworkReply*);
};

#endif // MEDNUSNETWORK_H

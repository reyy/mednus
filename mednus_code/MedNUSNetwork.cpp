#include "MedNUSNetwork.h"


MedNUSNetwork::MedNUSNetwork(QObject *parent) :
    QObject(parent)
{
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(getReply(QNetworkReply*)));
    QSettings settings("nus.edu", "MedNUS");
    QSettings::setDefaultFormat(QSettings::NativeFormat);
    if(settings.contains("Token"))
    {
        qDebug() << settings.value("Token","").toString();
        this->token = settings.value("Token","").toString();
        verifyToken();
        //todo: call loading page slots
    }
}

void MedNUSNetwork::verifyToken()
{
    QUrl temp = QUrl( QString("https://ivle.nus.edu.sg/api/Lapi.svc/Validate"));
    //https://ivle.nus.edu.sg/api/login/?apikey=9fPC9MsT0DEWtHOk0iW7H&url=http://localhost") ) );
    QUrlQuery postData;
    postData.addQueryItem("APIKey",/*"9fPC9MsT0DEWtHOk0iW7H"*/"lAY3TAAcAGYcokEEqKNCt");
    postData.addQueryItem("Token",token);
    temp.setQuery(postData);

    QNetworkRequest req(temp);

    mgr.get(req);
}

void MedNUSNetwork::getProfile()
{

    //QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(profileReply(QNetworkReply*)));

    // the HTTP request
    QUrl temp = QUrl( QString("https://ivle.nus.edu.sg/api/Lapi.svc/Profile_View"));
    //https://ivle.nus.edu.sg/api/login/?apikey=9fPC9MsT0DEWtHOk0iW7H&url=http://localhost") ) );
    QUrlQuery postData;
    postData.addQueryItem("APIKey",/*"9fPC9MsT0DEWtHOk0iW7H"*/"lAY3TAAcAGYcokEEqKNCt");
    postData.addQueryItem("AuthToken",token);
    temp.setQuery(postData);

    QNetworkRequest req(temp);

    mgr.get(req);
}


void MedNUSNetwork::login(QString matric, QString password, bool remember)
{
    this->rememberMe = remember;
    //    token = "CABDF0D0EB3AFCBA292BB9127F23BB1C3F4D6AC1B2317CB3D96AD7E81606E7446CD96EA511E71E97F66ED642E8DB8CB626C61D7E63F8826533658BE71CC5FA9F617566D2703F722609793E439A837418EE999356C2E8DAA00A851CCDA1D41095DB66EF55083A9E8790ED47DC3A58197453EC9688DB22553B22B28F70D5C3966F13C747FAFF8F1FFBB17559619D8A19C0A5F3399B820650A76F9519D71EB5D7C349969A896C9D8FF919608884169504F3F204D8EE7287B66D89F1DEDE1E16A3BABBBB4C2168316704DEE4090B7647F67D";
    //    getProfile();
    // the HTTP request
    if(isLocked)
        return;
    isLocked = true;
    QNetworkRequest req( QUrl( QString("https://ivle.nus.edu.sg/api/Lapi.svc/Login_JSON")));//https://ivle.nus.edu.sg/api/login/?apikey=9fPC9MsT0DEWtHOk0iW7H&url=http://localhost") ) );
    QUrlQuery postData;
    postData.addQueryItem("APIKey",/*"9fPC9MsT0DEWtHOk0iW7H"*/"lAY3TAAcAGYcokEEqKNCt");
    postData.addQueryItem("UserID", matric);
    postData.addQueryItem("Password", password);
    postData.addQueryItem("Domain","NUSSTU");

    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    mgr.post(req, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void MedNUSNetwork::getReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {

        QString strReply = (QString)reply->readAll();

        //parse json
        qDebug() << "Response:" << strReply;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

        QJsonObject jsonObj = jsonResponse.object();
        if(jsonObj["Login_JSONResult"].isObject())
            loginReply(jsonObj["Login_JSONResult"].toObject());
        else if(jsonObj["Token"].isString()) //todo: MUST CHANGE THIS!
            checkTokenReply(jsonObj);
        else if(jsonObj["Results"].isArray()) //todo: MUST CHANGE THIS!
            profileReply(jsonObj["Results"].toArray()[0].toObject());

    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }
    isLocked = false;
}


void MedNUSNetwork::checkTokenReply(QJsonObject jsonObj)
{
    bool success = jsonObj["Success"].toBool();

    if(success)
    {
        getProfile();
    }
    else
        emit loginResults(false,"","");
}

void MedNUSNetwork::loginReply(QJsonObject jsonObj)
{
    this->token = jsonObj["Token"].toString();
    bool success = jsonObj["Success"].toBool();

    if(success)
    {
        getProfile();
        if(rememberMe)
        {
            QSettings settings("nus.edu", "MedNUS");
            QSettings::setDefaultFormat(QSettings::NativeFormat);
            settings.setValue("Token",token);
            qDebug() << settings.value("Token","").toString();
            //QStringList keys = settings.allKeys();
        }
    }
    else
        emit loginResults(false,"","");
}

void MedNUSNetwork::profileReply(QJsonObject jsonObj)
{
    emit loginResults(true,jsonObj["UserID"].toString(),jsonObj["Name"].toString());
}

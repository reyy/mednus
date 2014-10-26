#include "MedNUSLogin.h"
#include "MedNUSAUISettings.h"

MedNUSLogin::MedNUSLogin(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(800,600);

    _background = new QLabel(parent);
    _background->setGeometry(QRect(0, 0, this->geometry().width(), this->geometry().height()));
    _background->setStyleSheet("background-color: #1a394a;");

    _logo = new QLabel(parent);
    _logo->setGeometry(QRect(this->geometry().width()*0.25-LOGO_WIDTH*0.5, this->geometry().height()*0.2, LOGO_WIDTH/40*76, LOGO_WIDTH/40*21));
    _logo->setPixmap(QPixmap(QString::fromStdString(":/images/logo.png")));
    _logo->setScaledContents(true);

    _username = new QLineEdit(parent);
    _username->setFocus();
    _username->setStyleSheet("border-width: 0px;font-size:20px;");
    _username->setGeometry(QRect(this->geometry().width()*0.5-100, this->geometry().height()*0.5-16, 200,24));
    _username->setPlaceholderText("A0123456A");
    _username->setFont (QFont ("Arial", 20));

    _password = new QLineEdit(parent);
    _password->setStyleSheet("border-width: 0px;font-size:20px;");
    _password->setGeometry(QRect(this->geometry().width()*0.5-100, this->geometry().height()*0.5+16, 200-24-5,24));
    _password->setEchoMode(QLineEdit::Password);
    _password->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    _password->setFont (QFont ("Arial", 20));

    _loginButton = new QPushButton(parent);
    _loginButton->setIcon(QIcon(":/images/icon_login.png"));
    _loginButton->setIconSize(QSize(24,24));
    _loginButton->setGeometry(QRect(this->geometry().width()*0.5+100-24, this->geometry().height()*0.5+16, 24,24));
    connect(_loginButton,SIGNAL(clicked()),this,SLOT(loginButtonPress()));
}

MedNUSLogin::~MedNUSLogin() {
    delete _background;
    delete _logo;
    delete _username;
    delete _password;
}

void MedNUSLogin::loginButtonPress()
{
    emit callLogin(_username->text(),_password->text());
}

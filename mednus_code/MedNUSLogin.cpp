#include "MedNUSLogin.h"
#include "MedNUSAUISettings.h"
#include <QFileDialog>

MedNUSLogin::MedNUSLogin(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(800,600);

    _background = new QLabel(this);
    _background->setPixmap(QPixmap(QString::fromStdString(":/images/login_background.jpg")));
    _background->setScaledContents(true);

    _background2 = new QLabel(this);
    _background2->setPixmap(QPixmap(QString::fromStdString(":/images/login_overlay.png")));
    _background2->setScaledContents(true);

    _backgroundLine = new QLabel(this);
    _backgroundLine->setStyleSheet("QLabel {background-image: url(:/images/login_lines.png);background-repeat: repeat-xy;}");

    _backgroundRect = new QLabel(this);
    _backgroundRect->setStyleSheet("QLabel {background-image: url(:/images/login_rect.png);background-repeat: repeat-xy;}");

    //_background->setStyleSheet("background-color: #1a394a;");
    _logo = new QLabel((this));
    _logo->setPixmap(QPixmap(QString::fromStdString(":/images/logo.png")));
    _logo->setScaledContents(true);

    _description = new QLabel((this));
    _description->setText("Connect. Access. Learn.");
    _description->setStyleSheet("color:#a5e4f4;");
    _description->setFont (QFont ("Geneva", 24));

    _error = new QLabel((this));
    _error->setText("");
    _error->setStyleSheet("color:#c3f0fb;");
    _error->setFont (QFont ("Geneva", 12));

    _title = new QLabel((this));
    _title->setText("MedNUS");
    _title->setStyleSheet("color:#a5e4f4;font-size:15px;font-weight:bold;");

    _subTitle = new QLabel((this));
    _subTitle->setText("Version 1.0");
    _subTitle->setStyleSheet("color:#a5e4f4;font-size:10px;");

    _bottomText = new QLabel((this));
    _bottomText->setText("© 2014 Rey Neo, Jay Chua and Melvyn Qwek.\nAll rights reserved.\n\nProject Supervisors:\n\nAssociate Professor:\nLeow Wee Kheng\nSchool of Computing (Department of Computer Science)\nNational University of Singapore\n\nConsulting Professor:\nGopalakrishnakone P\nDepartment of Anatomy\nYong Loo Lin School of Medicine\nNational University Health System\n\nDeveloped for CS3283 Media Technological Project\n\nNational University of Singapore\n21 Lower Kent Ridge Road\nSingapore 119077\n");
    _bottomText->setStyleSheet("color:#c3f0fb;font-size:9px;");

    _message = new QLabel((this));
    _message->setText("Welcome");
    _message->setStyleSheet("color:#FFFFFF;");
    _message->setFont (QFont ("Helvetica", 40));

    _sideText = new QLabel((this));
    _sideText->setText("Login");
    _sideText->setStyleSheet("color:#FFFFFF;");
    _sideText->setFont (QFont ("Helvetica", 30));

    _username = new QLineEdit((this));
    _username->setFocus();
    _username->setStyleSheet("QLineEdit {vertical-align: middle;border-width: 2px; border-style: inset; border-color: #FFFFFF; color:#FFFFFF; border-radius:5px;background-color:rgba(0,0,0,0);}");
    _username->setPlaceholderText("Matriculation Number");
    _username->setFont (QFont ("Helvetica", 15));

    _password = new QLineEdit((this));
    _password->setStyleSheet("QLineEdit {vertical-align: middle;border-width: 2px; border-style: inset; border-color: #FFFFFF; color:#FFFFFF; border-radius:5px;background-color:rgba(0,0,0,0);}");
    _password->setEchoMode(QLineEdit::Password);
    _password->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    _password->setPlaceholderText("Password");
    _password->setFont (QFont ("Helvetica", 15));

    _loginButton = new QPushButton((this));
    _loginButton->setIconSize(QSize(64,64));
    _loginButton->setFlat(true);
    _loginButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/icon_login.png);} QPushButton::pressed {background-image: url(:/images/icon_login_pressed.png);}");

    connect(_loginButton,SIGNAL(clicked()),this,SLOT(loginButtonPress()));
}

MedNUSLogin::~MedNUSLogin() {
    delete _background;
    delete _background2;
    delete _backgroundLine;
    delete _backgroundRect;
    delete _logo;
    delete _description;
    delete _error;
    delete _title;
    delete _subTitle;
    delete _sideText;
    delete _message;
    delete _bottomText;
    delete _loginButton;
    delete _username;
    delete _password;
}

void MedNUSLogin::loginButtonPress() {
    setLoading(true);
    emit callLogin(_username->text(),_password->text());
}

void MedNUSLogin::resizeEvent(QResizeEvent* event)
{
    _background->setGeometry(QRect(0, 0, this->geometry().width(), this->geometry().height()));
    _background2->setGeometry(QRect(this->geometry().width()*0.6, 0, this->geometry().width()*0.4, this->geometry().height()));
    _backgroundRect->setGeometry(QRect(0, 0, this->geometry().width()*0.6, this->geometry().height()));
    _backgroundLine->setGeometry(QRect(this->geometry().width()*0.6, 0, this->geometry().width()*0.4, this->geometry().height()));
    _logo->setGeometry(QRect(this->geometry().width()*0.04, this->geometry().height()*0.05, LOGO_WIDTH/40*76, LOGO_WIDTH/40*21));
    _description->setGeometry(QRect(this->geometry().width()*0.04+40, this->geometry().height()*0.05+LOGO_WIDTH/40*15, 300,50));
    _title->setGeometry(QRect(this->geometry().width()*0.62, this->geometry().height()*0.1, 100,32));_username->setGeometry(QRect(this->geometry().width()*0.11, this->geometry().height()*0.5-16, 200,24));
    _subTitle->setGeometry(QRect(this->geometry().width()*0.62, this->geometry().height()*0.1+12, 100,32));
    _bottomText->setGeometry(QRect(this->geometry().width()*0.62, this->geometry().height()*0.1+12+30, 300,250));
    _message->setGeometry(QRect(this->geometry().width()*0.08, this->geometry().height()*0.3, 200,42));
    _sideText->setGeometry(QRect(this->geometry().width()*0.08+210+64+10, this->geometry().height()*0.5, 100,64));
    _username->setGeometry(QRect(this->geometry().width()*0.08, this->geometry().height()*0.5, 200,24));
    _password->setGeometry(QRect(this->geometry().width()*0.08, this->geometry().height()*0.5+34, 200,24));
    _error->setGeometry(QRect(this->geometry().width()*0.08+10, this->geometry().height()*0.5+34+22, 300,30));
    _loginButton->setGeometry(QRect(this->geometry().width()*0.08+210, this->geometry().height()*0.5, 64,64));
}

void MedNUSLogin::setLoading(bool value) {
    if(value) {
        _message->setText("Loading");
        _sideText->setText("");
        _loginButton->setVisible(false);
        _username->setVisible(false);
        _password->setVisible(false);
        _error->setText("");
    } else  {
        _message->setText("Welcome");
        _sideText->setText("Login");
        _loginButton->setVisible(true);
        _username->setVisible(true);
        _password->setVisible(true);
    }
}

void MedNUSLogin::setErrorMessage(QString value) {
    _error->setText(value);
}

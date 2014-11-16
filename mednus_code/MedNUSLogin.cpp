#include "MedNUSLogin.h"
#include "MedNUSAUISettings.h"
#include <QFileDialog>
#include <QDebug>
#include <QKeyEvent>

MedNUSLogin::MedNUSLogin(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(800,600);

    _background = new QLabel(this);
    _background->setPixmap(QPixmap(QString::fromStdString(":/images/login_overlay.png")));
    _background->setScaledContents(true);

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
    _error->setVisible(true);

    _subTitle = new QLabel((this));
    _subTitle->setText("Version 1.0");
    _subTitle->setStyleSheet("color:#a5e4f4;font-size:10px;");
    _subTitle->setVisible(false);

    _aboutText = new QLabel((this));
    _aboutText->setPixmap(QPixmap(QString::fromStdString(":/images/about.png")));
    _aboutText->setScaledContents(true);

    _message = new QLabel((this));
    _message->setText("Welcome");
    _message->setStyleSheet("color:#FFFFFF;vertical-align: middle;");
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

    _remember = new QCheckBox(this);
    _remember->setText("Keep me signed in");
    _remember->setStyleSheet(" QCheckBox {spacing: 5px;color:#c3f0fb;} QCheckBox::indicator:unchecked {image: url(:/images/checkbox_unchecked.png);} QCheckBox::indicator:checked {image: url(:/images/checkbox_checked.png);}");
    _remember->setFont (QFont ("Geneva", 12));
    _remember->setCheckState(Qt::Checked);

    _loginButton = new QPushButton((this));
    _loginButton->setIconSize(QSize(64,64));
    _loginButton->setFlat(true);
    _loginButton->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;background-image: url(:/images/icon_login.png);} QPushButton::pressed {background-image: url(:/images/icon_login_pressed.png);}");

    connect(_loginButton,SIGNAL(clicked()),this,SLOT(loginButtonPress()));

    _contactUs = new QLabel(this);
    _contactUs->setText("<html> <a style='text-decoration:none;color:#c4d6db' href='mailto:mednus@hotmail.com'>Contact Us</a>");
    _contactUs->setFont (QFont ("Geneva", 10));
    _contactUs->setOpenExternalLinks(true);
    _contactUs->setAlignment(Qt::AlignRight);
}

MedNUSLogin::~MedNUSLogin() {
}

void MedNUSLogin::loginButtonPress() {
    setLoading(true);
    emit callLogin(_username->text(),_password->text(),_remember->isChecked());
}

void MedNUSLogin::resizeEvent(QResizeEvent* event)
{
    int scale=MIN(1050,this->geometry().width());
    _background->setGeometry(QRect(this->geometry().width()*0.6, 0, this->geometry().width()*0.4, this->geometry().height()));
    _backgroundRect->setGeometry(QRect(0, 0, this->geometry().width()*0.6, this->geometry().height()));
    _backgroundLine->setGeometry(QRect(this->geometry().width()*0.6, 0, this->geometry().width()*0.4, this->geometry().height()));
    _logo->setGeometry(QRect(this->geometry().width()*0.04, this->geometry().height()*0.05, LOGO_WIDTH/40*76, LOGO_WIDTH/40*21));
    _description->setGeometry(QRect(this->geometry().width()*0.04+40, this->geometry().height()*0.05+LOGO_WIDTH/40*15, 300,50));
    _subTitle->setGeometry(QRect(this->geometry().width()*0.62, this->geometry().height()*0.1+12, 100,32));
    _aboutText->setGeometry(QRect(this->geometry().width()*0.61, this->geometry().height()*0.1, scale*0.4*0.9, scale*0.56285*0.9));
    _message->setGeometry(QRect(this->geometry().width()*0.08, this->geometry().height()*0.25, 500,100));
    _sideText->setGeometry(QRect(this->geometry().width()*0.08+210+64+10, this->geometry().height()*0.5, 100,64));
    _username->setGeometry(QRect(this->geometry().width()*0.08, this->geometry().height()*0.5, 200,24));
    _password->setGeometry(QRect(this->geometry().width()*0.08, this->geometry().height()*0.5+34, 200,24));
    _error->setGeometry(QRect(this->geometry().width()*0.08, this->geometry().height()*0.5+34+32, 300,30));
    _remember->setGeometry(QRect(this->geometry().width()*0.08, this->geometry().height()*0.5+34+22, 300,30));
    _loginButton->setGeometry(QRect(this->geometry().width()*0.08+210, this->geometry().height()*0.5, 64,64));
    _contactUs->setGeometry(QRect(this->geometry().width()-95, this->geometry().height()-30, 80,20));
}

void MedNUSLogin::setLoading(bool value) {
    if(value) {
        _message->setText("Loading");
        _sideText->setText("");
        _loginButton->setVisible(false);
        _username->setVisible(false);
        _password->setVisible(false);
        _remember->setVisible(false);
        //_error->setText("");
    } else  {
        _message->setText("Welcome");
        _sideText->setText("Login");
        _loginButton->setVisible(true);
        _username->setVisible(true);
        _password->setVisible(true);
        _remember->setVisible(true);
    }
}

void MedNUSLogin::setErrorMessage(QString value) {
   // _error->setText(value);
    _message->setText(value);
}

void MedNUSLogin::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        loginButtonPress();
    }
    event->ignore();
}

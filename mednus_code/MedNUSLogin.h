#ifndef MEDNUSLOGIN_H
#define MEDNUSLOGIN_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

class MedNUSLogin : public QWidget
{
    Q_OBJECT
private:
    QLabel *_background;
    QLabel *_background2;
    QLabel *_backgroundLine;
    QLabel *_backgroundRect;
    QLabel *_logo;
    QLabel *_contactUs;
    QLabel *_description;
    QLabel *_error;
    QLabel *_subTitle;
    QLabel *_sideText;
    QLabel *_message;
    QLabel *_aboutText;
    QPushButton *_loginButton;
    QLineEdit *_username;
    QLineEdit *_password;
    QCheckBox *_remember;

protected:
    void resizeEvent(QResizeEvent* event);
    void keyReleaseEvent(QKeyEvent *event);

public:
    explicit MedNUSLogin(QWidget *parent = 0);
    ~MedNUSLogin();


    void setErrorMessage(QString value);
signals:
    void callLogin(QString,QString,bool);
public slots:
    void loginButtonPress();
    void setLoading(bool value);
};

#endif // MEDNUSLOGIN_H

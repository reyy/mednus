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
public:
    explicit MedNUSLogin(QWidget *parent = 0);
    ~MedNUSLogin();

    void setErrorMessage(QString value);

protected:
    void resizeEvent(QResizeEvent* event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QLabel *_background;
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

signals:
    void callLogin(QString, QString, bool);

public slots:
    void loginButtonPress();
    void setLoading(bool value);
};


#endif // MEDNUSLOGIN_H

#ifndef MEDNUSLOGIN_H
#define MEDNUSLOGIN_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class MedNUSLogin : public QWidget
{
    Q_OBJECT
private:
    QLabel *_background;
    QLabel *_background2;
    QLabel *_backgroundLine;
    QLabel *_backgroundRect;
    QLabel *_logo;
    QLabel *_description;
    QLabel *_error;
    QLabel *_title;
    QLabel *_subTitle;
    QLabel *_sideText;
    QLabel *_message;
    QLabel *_bottomText;
    QPushButton *_loginButton;
    QLineEdit *_username;
    QLineEdit *_password;

protected:
    void resizeEvent(QResizeEvent* event);
    void keyReleaseEvent(QKeyEvent *event);

public:
    explicit MedNUSLogin(QWidget *parent = 0);
    ~MedNUSLogin();

    void setLoading(bool value);
    void setErrorMessage(QString value);
signals:
    void callLogin(QString,QString);
public slots:
    void loginButtonPress();
};

#endif // MEDNUSLOGIN_H

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
    QLabel *_logo;
    QPushButton *_loginButton;
    QLineEdit *_username;
    QLineEdit *_password;

public:
    explicit MedNUSLogin(QWidget *parent = 0);
    ~MedNUSLogin();

signals:

public slots:

};

#endif // MEDNUSLOGIN_H

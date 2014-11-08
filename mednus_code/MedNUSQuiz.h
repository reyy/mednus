#ifndef MEDNUSQUIZ_H
#define MEDNUSQUIZ_H

// General
#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QString>

// For Graphics
#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QScrollArea>

// For JSON
#include <QFile>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

// For debugging
#include <QTextStream>
#include <QDebug>

class MedNUSQuizQuestion : public QWidget
{
public:
    MedNUSQuizQuestion(QWidget *parent, QVBoxLayout *layout, QVector<QString> content, int noOfOptions);
    ~MedNUSQuizQuestion();

    void myForceResize();

protected:

private:
    QLabel* _questionTextLabel;
    QButtonGroup* _optionButtonGroup;
};

class MedNUSQuiz : public QWidget
{
    Q_OBJECT

public:
    explicit MedNUSQuiz(QString filename, QWidget *parent = 0);
    ~MedNUSQuiz();

protected:
    QWidget* _tempWidget;
    QVBoxLayout* _layout;
    QScrollArea* _scrollArea;

    QVector<QButtonGroup*> _questionOptionGroup;

    QPushButton* _markButton;
    QVector<int> _correctAnswerList;

    void resizeEvent(QResizeEvent *);

    // JSON
    void writeFile();
    void readFile();

private:
    QVector<MedNUSQuizQuestion*>* _questionList;

signals:

public slots:
    void markQuiz();
};

#endif // MEDNUSQUIZ_H

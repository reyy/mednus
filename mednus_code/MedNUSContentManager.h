#ifndef MEDNUSCONTENTMANAGER_H
#define MEDNUSCONTENTMANAGER_H

#include <QObject>
#include <QStringList>
#include <QWidget>
#include <QDir>

#include "MedNUSPdfViewer.h"
#include "MedNUSMeshViewer.h"
#include "MedNUSVideoViewer.h"
#include "MedNUSQuiz.h"

class MedNUSContentManager : public QObject
{
    Q_OBJECT
public:
    explicit MedNUSContentManager(QObject *parent = 0);

    void openLastView(QStringList);

private:
    bool isFileExist(QString);

signals:
    void callAddTab(QWidget*,QString,QString);
    void callAddLesson(QString title, QString subTitle, QString description, QStringList directory);

public slots:
    void openFile(QString fileDir, QString title, int type);
    void initLessonList(QJsonDocument jsonResponse);
};


#endif // MEDNUSCONTENTMANAGER_H

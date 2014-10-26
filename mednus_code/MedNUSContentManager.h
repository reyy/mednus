#ifndef MEDNUSCONTENTMANAGER_H
#define MEDNUSCONTENTMANAGER_H
#include <QObject>
#include <QStringList>
#include <QWidget>

class MedNUSContentManager : public QObject
{
    Q_OBJECT
public:
    explicit MedNUSContentManager(QObject *parent = 0);
    void initLessonList(QStringList);

public slots:
    void openFile(QString);
signals:
    //void callAddTab(QWidget);
    void callAddLesson(QString title, QString subTitle, QString description, QStringList directory);

private:
    bool isFileExist();
};


#endif // MEDNUSCONTENTMANAGER_H

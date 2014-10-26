#include "MedNUSContentManager.h"

MedNUSContentManager::MedNUSContentManager(QObject *parent)
{
}


void MedNUSContentManager::initLessonList(QStringList)
{
    //STUB For future Network I/O connection!
    QStringList content;
    content.push_back(":/content/test.pdf");
    content.push_back(":/content/samplevideo.mp4");

    emit callAddLesson("Skull Biology","Professor Gopal","Module about biology.",content);
}

void MedNUSContentManager::openFile(QString)
{
    if(isFileExist())
    {
        //Create MedNUSVideo/Mesh/PDF instance
        //emit callAddTab();
    }
    else
    {
        //Future Call Network to D/L
    }
}

bool MedNUSContentManager::isFileExist()
{

}

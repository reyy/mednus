#include "MedNUSContentManager.h"

MedNUSContentManager::MedNUSContentManager(QObject *parent)
{
}


void MedNUSContentManager::initLessonList(QStringList)
{
    //STUB For future Network I/O connection!
    QStringList content;
    //content.push_back(":/content/test.pdf");
    //content.push_back(":/content/samplevideo.mp4");
    content.push_back("/mednus/lesson1/pdf/thorax.ppt.pdf");
    content.push_back("/mednus/lesson1/videos/Untitled.mp4");
    content.push_back("/mednus/lesson1/models/craniofacial.ply");

    emit callAddLesson("Skull Biology","Professor Gopal","Module about biology.",content);

    content.clear();
    content.push_back("/mednus/lesson1/pdf/thorax.ppt.pdf");
    content.push_back("/mednus/lesson1/videos/Untitled.mp4");
    content.push_back("/mednus/lesson1/models/craniofacial.ply");

    emit callAddLesson("Amazing Lesson 2","Professor RMJ","Module about stuffs.",content);
}

void MedNUSContentManager::openFile(QString fileDir, QString title, int type)
{
    QString dir = QDir::homePath();
    dir.append(fileDir);

    if(isFileExist(dir))
    {
        //Create MedNUSVideo/Mesh/PDF instance
        QWidget *toAdd;
        //todo: change to type
        if(fileDir.contains(".pdf", Qt::CaseInsensitive))
            toAdd = new MedNUSPdfViewer(dir);
        else if(fileDir.contains(".ply", Qt::CaseInsensitive))
            toAdd = new MedNUSMeshViewer(dir,false);
        else if(fileDir.contains(".mp4", Qt::CaseInsensitive) || fileDir.contains(".mov", Qt::CaseInsensitive))
            toAdd = new MedNUSVideoViewer(dir);
        else if(fileDir.contains(".qiz", Qt::CaseInsensitive))
            toAdd = new MedNUSQuiz(dir);

        emit callAddTab(toAdd,title);
    }
    else
    {
        //Future Call Network to D/L
    }
}

bool MedNUSContentManager::isFileExist(QString)
{
    return true;
}

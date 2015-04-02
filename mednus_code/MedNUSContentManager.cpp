#include "MedNUSContentManager.h"

MedNUSContentManager::MedNUSContentManager(QObject *parent)
{
}


void MedNUSContentManager::initLessonList(QJsonDocument jsonResponse)
{
    //STUB For future Network I/O connection!
    QJsonArray *jsonObj2 = new QJsonArray(jsonResponse.array());
    //
    QJsonArray::ConstIterator i = jsonObj2->begin();
    while(i!=jsonObj2->end()) {
        QStringList content;
        QJsonObject o = (*i).toObject();
        content.push_back("abc.pdf");
        emit callAddLesson(o["lesson_title"].toString(),"Const","",content);

        qDebug() << o["lesson_id"].toString().toInt();
        qDebug() << o["file_list"].toString();
        i++;
    }

    QStringList content;
    content.push_back("/mednus/lesson1/videos/Skull Anatomy (1 of 5)- Superior, Posterior and Lateral Views -- Head and Neck Anatomy 101.mp4");
    content.push_back("/mednus/lesson1/videos/Osteology of the Skull- 12 Newborn Skull.mp4");
    content.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content.push_back("/mednus/lesson1/models/craniofacial.ply");
    content.push_back("/mednus/lesson1/quiz/Quiz - The Skull.qiz");
    content.push_back("/mednus/lesson1/models/face.ply");
    content.push_back("/mednus/lesson1/models/skull.ply");
    emit callAddLesson("Functional Anatomy of the Skull","Professor Gopal","Anatomy Department",content);

    QStringList content2;
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson2/videos/Osteology of the Skull- 7 The Face (1).mp4");
    content2.push_back("/mednus/lesson1/models/craniofacial.ply");
    emit callAddLesson("Osteology of the Skull","A/Professor Tan","Anatomy Department",content2);

    content2.clear();
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson2/videos/Osteology of the Skull- 7 The Face (1).mp4");
    emit callAddLesson("Skull Osteology I","A/Professor Tan","Anatomy Department",content2);

    content2.clear();
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson1/videos/Osteology of the Skull- 12 Newborn Skull.mp4");
    emit callAddLesson("Skull Osteology II","A/Professor Tan","Anatomy Department",content2);
    //openLastView(content);
}


void MedNUSContentManager::openLastView(QStringList content)
{
    //Stub: Future use of QSettings to determine last open tabs
    for (int i=content.size()-1; i>=0; i--)
    {
        //DUPLICATED EFFORT FOR FILE NAME! MUST REFACTOR!
        int startIndex=content[i].lastIndexOf("/");
        int nameLength=content[i].size()-startIndex;
        QString title = content[i].mid(startIndex+1,nameLength-1);

        openFile(content[i],title,0);
    }
}


void MedNUSContentManager::openFile(QString fileDir, QString title, int type)
{
    QString dir = QDir::homePath();
    dir.append(fileDir);

    qDebug() << dir;
    qDebug() << "fileType = " << type;

    if(isFileExist(dir))
    {
        //Create MedNUSVideo/Mesh/PDF instance
        QWidget *toAdd;

        switch (type) {
        case PDF:
            toAdd = new MedNUSPdfViewer(dir);
            break;

        case VIDEO:
            toAdd = new MedNUSVideoViewer(dir);
            break;

        case MODEL:
            toAdd = new MedNUSMeshViewer(dir, true);
            break;

        case QUIZ:
            toAdd = new MedNUSQuiz(dir);
            break;
        }

        //Todo: Temp fix for too long titles
        if (title.length()>10)
        {
            title.truncate(10);
            title.append("...");
        }
        emit callAddTab(toAdd,title,dir);
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

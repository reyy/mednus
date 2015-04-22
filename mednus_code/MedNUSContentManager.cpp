#include "MedNUSContentManager.h"
using namespace std;

MedNUSContentManager::MedNUSContentManager(interfaceMode currentMode,
                                           QObject *parent)
{
    _currentMode = currentMode;
}


void MedNUSContentManager::initLessonList(QJsonDocument jsonResponse)
{
    //Receive Stuff from Network I/O
    QJsonArray *lessonArray = new QJsonArray(jsonResponse.array());
    QJsonArray::ConstIterator curLesson = lessonArray->begin();

    while(curLesson!=lessonArray->end()) {
        QJsonObject fileItem = (*curLesson).toObject();
        QString desc = fileItem["lesson_desc"].toString();
        QString owner = fileItem["lesson_owner_domain"].toString() + "\\" + fileItem["lesson_owner"].toString();
        QStringList content = fileItem["file_list"].toString().split(QRegExp("[{}\",]"),
                                                                     QString::SplitBehavior::SkipEmptyParts);
        //Account for NULL cases
        if(content.size() == 1 && content[0] =="NULL")
            content.clear();

        emit callAddLesson(fileItem["lesson_title"].toString(), desc, owner, content, "");

        curLesson++;
    }

    QStringList content;
    content.push_back("/mednus/lesson1/videos/Skull Anatomy (1 of 5)- Superior, Posterior and Lateral Views -- Head and Neck Anatomy 101.mp4");
    content.push_back("/mednus/lesson1/videos/Osteology of the Skull- 12 Newborn Skull.mp4");
    content.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content.push_back("/mednus/lesson1/models/model2");
    content.push_back("/mednus/lesson1/quiz/Quiz - The Skull.qiz");
    emit callAddLesson("Functional Anatomy of the Skull","Professor Gopal","Anatomy Department",content, "/mednus/lesson1/story");

    QStringList content2;
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content2.push_back("/mednus/lesson2/videos/Osteology of the Skull- 7 The Face (1).mp4");
    content2.push_back("/mednus/lesson1/models/model1");
    emit callAddLesson("Osteology of the Skull","A/Professor Tan","Anatomy Department",content2, "");

    content2.clear();
    content2.push_back("/mednus/lesson2/pdf/axial_lecture_2.pdf");
    content.push_back("/mednus/lesson1/pdf/Functional anatomy of skull.pdf");
    content2.push_back("/mednus/lesson1/videos/Osteology of the Skull- 12 Newborn Skull.mp4");
    emit callAddLesson("Skull Osteology II","A/Professor Tan","Anatomy Department",content2, "");
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
            toAdd = new MedNUSQuiz(dir, _currentMode);
            break;
        }

        //Todo: Temp fix for too long titles
        if (title.length()>10)
        {
            title.truncate(10);
            title.append("...");
        }

        //Catch Corrupted File
        if(toAdd->property("Loaded").isValid())
            emit callAddTab(toAdd,title,dir);
        else
            QMessageBox::information(NULL, "", "File could not be opened. It might be corrupted or be used by another process.");
    }
    else
    {
        //Future Call Network to D/L
        QMessageBox::information(NULL, "File Not Downloaded", "File not found! Network implementation to download missing file is not completed yet.");
    }
}


bool MedNUSContentManager::isFileExist(QString path)
{
    QFileInfo checkFile(path);

    if (checkFile.exists() /*&& checkFile.isFile()*/)
        return true;
    else
        return false;
}

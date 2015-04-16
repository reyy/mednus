#include "MedNUSStoryManager.h"

MedNUSStoryManager::MedNUSStoryManager(QString storyFile)
{
    storyPointList = new vector<StoryPoint>();
    videoFile = NULL;
    modelFile = NULL;
    pdfFile = NULL;

    if(!loadStoryFile(storyFile))
        throw false;

}

MedNUSStoryManager::~MedNUSStoryManager()
{
    delete storyPointList;
    storyPointList = NULL;
}

void MedNUSStoryManager::checkAddedItem(QString fileName, fileType type, MedNUSLessonIcon *icon)
{
    if(type == fileType::MODEL && fileName == modelFileName)
        modelFile = icon;
    else if (type == fileType::VIDEO && fileName == videoFileName)
        videoFile = icon;
    else if (type == fileType::PDF && fileName == pdfFileName)
        pdfFile = icon;

    //TODO: Account for NULL cases
    if(modelFile && videoFile && pdfFile)
        isReady = true;
}

void MedNUSStoryManager::playStory()
{
    //TODO: Account for NULL
    if(!videoFile->isOpened())
        videoFile->forceOpenFile();
    if(!pdfFile->isOpened())
        pdfFile->forceOpenFile();
    if(!modelFile->isOpened())
        modelFile->forceOpenFile();

    //TODO: Set a timeout!
    while(!videoFile->isOpened() || !pdfFile->isOpened() || !modelFile->isOpened()) {}

    MedNUSVideoViewer* videoWidget = dynamic_cast<MedNUSVideoViewer*>(videoFile->getContentWidget());
    connect(videoWidget, SIGNAL(positionChanged(qint64)), this, SLOT(videoPositionChanged(qint64)));
    //if(!isReady)

}

bool MedNUSStoryManager::loadStoryFile(QString storyFile)
{
    if(storyFile == "")
        return false;
    //TEMP
    this->videoFileName = "/mednus/lesson1/videos/Osteology of the Skull- 12 Newborn Skull.mp4";
    this->modelFileName = "/mednus/lesson1/models/model2";
    this->pdfFileName = "/mednus/lesson1/pdf/Functional anatomy of skull.pdf";
    return true;
}

void MedNUSStoryManager::videoPositionChanged(qint64 pos)
{
    qDebug() << pos;
}

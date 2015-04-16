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

    if(modelFile && videoFile && pdfFile)
        isReady = true;
}

void MedNUSStoryManager::playStory()
{

}

bool MedNUSStoryManager::loadStoryFile(QString storyFile)
{
    if(storyFile == "")
        return false;

    return true;
}

#include "MedNUSStoryManager.h"

MedNUSStoryManager::MedNUSStoryManager(QString storyFile)
{
    storyPointList = new vector<StoryPoint>();
    if(!loadStoryFile(storyFile))
        throw false;

}

MedNUSStoryManager::~MedNUSStoryManager()
{
    delete storyPointList;
    storyPointList = NULL;
}

void MedNUSStoryManager::playStory()
{

}

bool MedNUSStoryManager::loadStoryFile(QString storyFile)
{
    return true;
}

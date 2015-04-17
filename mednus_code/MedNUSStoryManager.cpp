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

    StoryPoint temp;
    temp.timestamp = 0;
    temp.slideNum = 1;
    temp.cameraPosition[0] =  -120.327; temp.cameraPosition[1] = -118.244; temp.cameraPosition[2] =754.268;
    temp.cameraFocalPoint[0] = -120.327; temp.cameraFocalPoint[1] = -118.244; temp.cameraFocalPoint[2] = 108.92;
    temp.cameraViewUp[0] = 0; temp.cameraViewUp[1] = 1; temp.cameraViewUp[2] =0;
    temp.cameraViewAngle = 30;
    storyPointList->push_back(temp);

    temp.timestamp = 20000;
    temp.slideNum = 2;
    temp.cameraPosition[0] =  365.956; temp.cameraPosition[1] = 106.795; temp.cameraPosition[2] = 468.587;
    temp.cameraFocalPoint[0] = -120.327; temp.cameraFocalPoint[1] = -118.244; temp.cameraFocalPoint[2] = 108.92;
    temp.cameraViewUp[0] = -0.106299; temp.cameraViewUp[1] = 0.901206; temp.cameraViewUp[2] =-0.420153;
    temp.cameraViewAngle = 30;
    storyPointList->push_back(temp);

    temp.timestamp = 50000;
    temp.slideNum = 3;
    temp.cameraPosition[0] =  -631.827; temp.cameraPosition[1] = 118.549; temp.cameraPosition[2] =423.198;
    temp.cameraFocalPoint[0] = -120.327; temp.cameraFocalPoint[1] = -118.244; temp.cameraFocalPoint[2] = 108.92;
    temp.cameraViewUp[0] = 0.0842606; temp.cameraViewUp[1] = 0.856923; temp.cameraViewUp[2] =-0.508511;
    temp.cameraViewAngle = 30;
    storyPointList->push_back(temp);
    return true;
}

void MedNUSStoryManager::goToStoryPoint(MedNUSStoryManager::StoryPoint curStoryPoint)
{
    if(++nextStoryPoint >= storyPointList->size())
        nextStoryPoint = END_OF_STORY;
    //
    qDebug() <<"HHHHH";
    if(pdfFile->isOpened())
    {
        MedNUSPdfViewer* pdfWidget = dynamic_cast<MedNUSPdfViewer*>(pdfFile->getContentWidget());
        pdfWidget->setPage(curStoryPoint.slideNum);
    }

    if(modelFile->isOpened())
    {
        MedNUSMeshViewer* modelWidget = dynamic_cast<MedNUSMeshViewer*>(modelFile->getContentWidget());
        modelWidget->setCameraView(curStoryPoint.cameraViewAngle,
                                   curStoryPoint.cameraPosition,
                                   curStoryPoint.cameraFocalPoint,
                                   curStoryPoint.cameraViewUp);
    }

}

void MedNUSStoryManager::videoPositionChanged(qint64 pos)
{
    qint64 posDelta = pos - prevPos;
    if(/*posDelta > POS_DELTA_LIMIT ||*/ posDelta < 0)
    {
        //Recheck everything
        for(int i = 0; i < storyPointList->size(); i++)
            if((*storyPointList)[i].timestamp <= pos)
                nextStoryPoint = i;
            else
                break;

        //Get back on track
        goToStoryPoint((*storyPointList)[nextStoryPoint]);
    }
    else if(nextStoryPoint != END_OF_STORY)
    {
        StoryPoint next = (*storyPointList)[nextStoryPoint];
        if(next.timestamp >= pos)
            goToStoryPoint(next);
        qDebug() << next.timestamp << pos;
    }
}

#include "MedNUSStoryManager.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MedNUSStoryManager::MedNUSStoryManager(QString storyFile)
{
    storyPointList = new QVector<StoryPoint>();
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
    {
        videoFile = icon;
        connect(videoFile,SIGNAL(emitTabIsOpen()), this, SLOT(initStoryPoints()));
    }
    else if (type == fileType::PDF && fileName == pdfFileName)
        pdfFile = icon;


    //TODO: Account for NULL cases
    if(modelFile && videoFile && pdfFile)
        isReady = true;
}

void MedNUSStoryManager::playStory()
{
    //TODO: Account for NULL
    if(videoFile && !videoFile->isOpened())
        videoFile->forceOpenFile();
    if(pdfFile && !pdfFile->isOpened())
        pdfFile->forceOpenFile();
    if(modelFile && !modelFile->isOpened())
        modelFile->forceOpenFile();

    //TODO: Set a timeout!
    while((!videoFile || !videoFile->isOpened()) ||
          (!pdfFile || !pdfFile->isOpened()) ||
          (!modelFile || !modelFile->isOpened())) {}

    MedNUSVideoViewer* videoWidget = dynamic_cast<MedNUSVideoViewer*>(videoFile->getContentWidget());
    connect(videoWidget, SIGNAL(positionChanged(qint64)), this, SLOT(videoPositionChanged(qint64)));
    //if(!isReady)

}

bool MedNUSStoryManager::loadStoryFile(QString storyFile)
{
    QString dir = QDir::homePath();
    dir.append(storyFile);

    QFile file(dir);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Unable to open file.";
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();
    QJsonValue jsonValue = obj.value(QString("videoFile"));
    videoFileName = jsonValue.toString();

    jsonValue = obj.value(QString("3DFile"));
    modelFileName = jsonValue.toString();

    jsonValue = obj.value(QString("PDFFile"));
    pdfFileName = jsonValue.toString();

    jsonValue = obj.value(QString("timeline"));
    QJsonArray timelineDetails = jsonValue.toArray();

    for(int i = 0; i < timelineDetails.size(); i++)
    {
        obj = timelineDetails[i].toObject();

        StoryPoint temp;
        temp.timestamp = qint64(obj.value(QString("timestamp")).toString().toLongLong());
        temp.slideNum = obj.value(QString("slide")).toInt();

        QJsonArray posArray = obj.value(QString("cameraPosition")).toArray();
        QJsonArray focArray = obj.value(QString("cameraFocalPoint")).toArray();
        QJsonArray vupArray = obj.value(QString("cameraViewUp")).toArray();
        for(int i = 0; i<3; i++)
        {
            temp.cameraPosition[i] =  posArray[i].toDouble();
            temp.cameraFocalPoint[i] = focArray[i].toDouble();
            temp.cameraViewUp[i] = vupArray[i].toDouble();
        }

        temp.cameraViewAngle = obj.value(QString("cameraViewAngle")).toInt();
        storyPointList->push_back(temp);
    }

    return true;
}

void MedNUSStoryManager::goToStoryPoint(MedNUSStoryManager::StoryPoint curStoryPoint)
{
    if(++nextStoryPoint >= storyPointList->size())
        nextStoryPoint = END_OF_STORY;
    //
    //qDebug() <<"HHHHH";
    if(pdfFile && pdfFile->isOpened() && pdfFile->getContentWidget())
    {
        MedNUSPdfViewer* pdfWidget;
        if(pdfWidget = dynamic_cast<MedNUSPdfViewer*>(pdfFile->getContentWidget()))
            pdfWidget->setPage(curStoryPoint.slideNum);
    }

    if(modelFile && modelFile->isOpened() && modelFile->getContentWidget())
    {
        MedNUSMeshViewer* modelWidget;
        if (modelWidget = dynamic_cast<MedNUSMeshViewer*>(modelFile->getContentWidget()));
            modelWidget->setCameraView(curStoryPoint.cameraViewAngle,
                                       curStoryPoint.cameraPosition,
                                       curStoryPoint.cameraFocalPoint,
                                       curStoryPoint.cameraViewUp);
    }

}

bool MedNUSStoryManager::fillInStoryPointData(qint64 pos, StoryPoint *toAdd)
{
    toAdd->timestamp = pos;
    if(modelFile && modelFile->getContentWidget())
    {
        MedNUSMeshViewer* modelWidget = dynamic_cast<MedNUSMeshViewer*>(modelFile->getContentWidget());
        //toAdd.cameraFocalPoint = modelWidget->
        for(int i=0; i<3;i++)
        {
            toAdd->cameraPosition[i] = modelWidget->getCamerPosition()[i];
            toAdd->cameraFocalPoint[i] = modelWidget->getCameraFocalPoint()[i];
            toAdd->cameraViewUp[i] = modelWidget->getCameraViewUp()[i];
        }
        toAdd->cameraViewAngle = modelWidget->getViewAngle();
    }
    else
        return false;

    if(pdfFile && pdfFile->getContentWidget())
    {
        MedNUSPdfViewer* modelWidget = dynamic_cast<MedNUSPdfViewer*>(pdfFile->getContentWidget());
        toAdd->slideNum = modelWidget->getPage();
    }
    else
        return false;

    return true;
}

void MedNUSStoryManager::videoPositionChanged(qint64 pos)
{
    qint64 posDelta = pos - prevPos;
    if(posDelta > POS_DELTA_LIMIT || posDelta < 0)
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
        qDebug() << next.timestamp << pos;
        if(next.timestamp <= pos + 999)
            goToStoryPoint(next);
    }
    prevPos = pos;
}

void MedNUSStoryManager::initStoryPoints()
{
    if(!videoFile || !videoFile->getContentWidget())
        qWarning() << "Unable to init story points";
    else
    {
        QList<qint64> spList;
        foreach(StoryPoint point, *storyPointList)
            spList.push_back(point.timestamp);
        MedNUSVideoViewer* videoWidget = dynamic_cast<MedNUSVideoViewer*>(videoFile->getContentWidget());
        videoWidget->initStoryPoints(spList);

        connect(videoWidget, SIGNAL(addEditStoryPoint(qint64)), this, SLOT(addEditStoryPoints(qint64)));
    }
}

void MedNUSStoryManager::addEditStoryPoints(qint64 pos)
{
    qDebug() << pos;
    //TODO: if empty

    for(int i = 0; i<storyPointList->size(); i++)//StoryPoint point = (*storyPointList)[i]
        if((*storyPointList)[i].timestamp == pos)
        {
            fillInStoryPointData(pos, &(*storyPointList)[i]);
            initStoryPoints();
            return;
        }
        else if((*storyPointList)[i].timestamp > pos)
        {
            StoryPoint toAdd;
            fillInStoryPointData(pos, &toAdd);
            storyPointList->insert(i, toAdd);
            initStoryPoints();
            return;
        }
    //Add to first
    if((*storyPointList)[0].timestamp > pos)
    {
        StoryPoint toAdd;
        fillInStoryPointData(pos, &toAdd);
        storyPointList->push_front(toAdd);
        initStoryPoints();
        return;
    }
    else if((*storyPointList)[storyPointList->size() - 1].timestamp < pos)
    {
        StoryPoint toAdd;
        fillInStoryPointData(pos, &toAdd);
        storyPointList->push_back(toAdd);
        initStoryPoints();
        return;
    }
    //Add to last
}

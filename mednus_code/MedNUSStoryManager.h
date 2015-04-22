#ifndef MEDNUSSTORYMANAGER_H
#define MEDNUSSTORYMANAGER_H
#include "MedNUSLessonIcon.h"
#include "MedNUSVideoViewer.h"
#include "MedNUSPdfViewer.h"
#include "MedNUSMeshViewer.h"
using namespace std;

class MedNUSStoryManager : public QObject
{
    Q_OBJECT
public:
    MedNUSStoryManager(QString);
    ~MedNUSStoryManager();

    void checkAddedItem(QString, fileType, MedNUSLessonIcon*);

public slots:
    void playStory();

protected:
    static const short END_OF_STORY = -1;
    static const short POS_DELTA_LIMIT = 1500;

    struct StoryPoint
    {
        qint64 timestamp = 0;
        int slideNum = 0;
        int cameraViewAngle = 0;
        double cameraPosition[3] = {0,0,0};
        double cameraFocalPoint[3] = {0,0,0};
        double cameraViewUp[3] = {0,0,0};
    };

    QVector<StoryPoint> *storyPointList;
    MedNUSLessonIcon *videoFile;
    MedNUSLessonIcon *modelFile;
    MedNUSLessonIcon *pdfFile;

    QString videoFileName;
    QString modelFileName;
    QString pdfFileName;

    bool isReady = false;
    qint64 prevPos = 1;
    int nextStoryPoint = 0;

//resume from last?
    bool loadStoryFile(QString storyFile);
    void goToStoryPoint(StoryPoint);

    bool fillInStoryPointData(qint64,StoryPoint*);

protected slots:
    void videoPositionChanged(qint64);
    void initStoryPoints();
    void addEditStoryPoints(qint64);
};

#endif // MEDNUSSTORYMANAGER_H

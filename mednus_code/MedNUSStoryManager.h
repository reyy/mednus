#ifndef MEDNUSSTORYMANAGER_H
#define MEDNUSSTORYMANAGER_H
#include "MedNUSLessonIcon.h"
using namespace std;

class MedNUSStoryManager : public QObject
{
    Q_OBJECT
public:
    MedNUSStoryManager(QString);
    ~MedNUSStoryManager();

public slots:
    void playStory();

protected:
    struct StoryPoint
    {
        int timestamp;
        int slideNum;
        int cameraViewAngle;
        float cameraPosition[3];
        float cameraFocalPoint[3];
        float cameraViewUp[3];
    };

    vector<StoryPoint> *storyPointList;
    MedNUSLessonIcon *videoFile;
    MedNUSLessonIcon *modelFile;
    MedNUSLessonIcon *pdfFile;
//resume from last?
    bool loadStoryFile(QString storyFile);
};

#endif // MEDNUSSTORYMANAGER_H

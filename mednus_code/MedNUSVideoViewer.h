#ifndef MEDNUSVIDEOVIEWER_H
#define MEDNUSVIDEOVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QMediaPlayer>
#include <QSlider>
#include <QStyle>
#include <QPushButton>
#include <QGridLayout>
#include "MedNUSAUISettings.h"

class MedNUSVideoControl;

class MedNUSVideoViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSVideoViewer(QString filename, interfaceMode currentMode = interfaceMode::STUDENT, QWidget *parent = 0);
    ~MedNUSVideoViewer();
    void initStoryPoints(QList<qint64>);

signals:
    void positionChanged(qint64);
    void addEditStoryPoint(qint64);
    void deleteStoryPoint(qint64);

protected:
    QMediaPlayer mediaPlayer;
    QGraphicsView *videoView;
    QGraphicsVideoItem *videoItem;
    QGraphicsScene *scene;
    MedNUSVideoControl *control;
    QString fi;
    interfaceMode currentMode;

    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject* obj, QEvent* e);

protected slots:
    void togglePlay();
    void setPosition(int position);
    void setPosition(qint64 position);
    void setVolume(int volume);
    void addEditStoryPoint() {emit addEditStoryPoint(mediaPlayer.position());}
    void deleteStoryPoint() {emit deleteStoryPoint(mediaPlayer.position());}
};

class MedNUSVideoControl : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSVideoControl(interfaceMode currentMode, QWidget *parent = 0);
    ~MedNUSVideoControl();

    void updateUI();
    void setStoryPoints(QList<qint64> sp);

protected:
    QPushButton *_playButton;
    QPushButton *_volumeButton;
    QSlider *_positionSlider;
    QLabel *_videoTimer;
    QString _durationText;
    int _volume;
    qint64 _duration = 0;
    qint64 _position = 0;

    QPushButton *_nextButton;
    QPushButton *_prevButton;
    QPushButton *_delButton;
    QPushButton *_addEditButton;
    QList <qint64> _storyPoints;
    QWidget *_storyPointContainer;
    QMediaPlayer::State _state = QMediaPlayer::PausedState;
    interfaceMode _currentMode;

    QString timeConvert(qint64);

signals:
    void seekTo(int millisecond);
    void changeVolume(int);
    void changePosition(qint64);

protected slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void volumeClicked();

    void goToNextStoryPoint();
    void goToPrevStoryPoint();
};


#endif // MEDNUSVIDEOVIEWER_H

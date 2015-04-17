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

class MedNUSVideoControl;

class MedNUSVideoViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSVideoViewer(QString filename, QWidget *parent = 0);
    ~MedNUSVideoViewer();

signals:
    void positionChanged(qint64);

protected:
    QMediaPlayer mediaPlayer;
    QGraphicsView *videoView;
    QGraphicsVideoItem *videoItem;
    QGraphicsScene *scene;
    MedNUSVideoControl *control;
    QString fi;

    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject* obj, QEvent* e);

protected slots:
    void togglePlay();
    void setPosition(int position);
    void setVolume(int volume);
};

class MedNUSVideoControl : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSVideoControl(QWidget *parent = 0);
    ~MedNUSVideoControl();

    void updateUI();

protected:
    QPushButton *_playButton;
    QPushButton *_volumeButton;
    QSlider *_positionSlider;
    QLabel *_videoTimer;
    QString _durationText;
    int _volume;

    QString timeConvert(qint64);

signals:
    void seekTo(int millisecond);
    void changeVolume(int);

protected slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void volumeClicked();
};


#endif // MEDNUSVIDEOVIEWER_H

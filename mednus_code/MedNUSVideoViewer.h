#ifndef MEDNUSVIDEOVIEWER_H
#define MEDNUSVIDEOVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QMediaPlayer>
#include <QSlider>
#include <QStyle>
#include <QPushButton>

class MedNUSVideoControl;

class MedNUSVideoViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSVideoViewer(QString filename, QWidget *parent = 0);
    ~MedNUSVideoViewer();
protected:
    QMediaPlayer *player;
    QVideoWidget *videoWidget;
    MedNUSVideoControl *control;
    QMediaPlayer mediaPlayer;
    QSize videoSize;
    //QVideoWidget *videoWidget;
    // test
    QGraphicsView *videoView;
    QGraphicsVideoItem *videoItem;
    QGraphicsScene *scene;

    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *);
    bool eventFilter(QObject* obj, QEvent* e);
signals:

protected slots:
    void togglePlay();
    void setPosition(int position);
};

class MedNUSVideoControl : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSVideoControl(QWidget *parent = 0);
    ~MedNUSVideoControl();
protected:
    QPushButton *play;
    QSlider *positionSlider;
signals:
    void seekTo(int millisecond);
protected slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
};

#endif // MEDNUSVIDEOVIEWER_H

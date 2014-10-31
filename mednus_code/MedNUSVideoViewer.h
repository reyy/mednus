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
#include <QMouseEvent>

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

public slots:

};

class MedNUSVideoControl : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSVideoControl(QWidget *parent = 0);
    ~MedNUSVideoControl();
protected:
    QLabel *pause;
    QLabel *play;
    QSlider *positionSlider;
};

#endif // MEDNUSVIDEOVIEWER_H

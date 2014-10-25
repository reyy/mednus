#ifndef MEDNUSVIDEOVIEWER_H
#define MEDNUSVIDEOVIEWER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QKeyEvent>

class MedNUSVideoViewer : public QWidget
{
    Q_OBJECT
public:
    explicit MedNUSVideoViewer(QString filename, QWidget *parent = 0);
    ~MedNUSVideoViewer();
protected:
    QMediaPlayer *player;
    void keyPressEvent(QKeyEvent *event);
signals:

public slots:

};

#endif // MEDNUSVIDEOVIEWER_H

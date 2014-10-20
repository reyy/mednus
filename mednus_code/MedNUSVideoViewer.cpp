#include "MedNUSVideoViewer.h"
#include <QGridLayout>

MedNUSVideoViewer::MedNUSVideoViewer(QWidget *parent) :
    QWidget(parent)
{
    player = new QMediaPlayer();

    QMediaPlaylist *playlist = new QMediaPlaylist(player);
    //playlist->addMedia(QUrl("mms://live-vip-49.nus.edu.sg/INV_CSEWARE/anatomy/respiratory/landmarks_patient.wmv"));
    playlist->addMedia(QUrl::fromLocalFile("/Users/rey/Desktop/Untitled.mov"));

    QVideoWidget *videoWidget = new QVideoWidget();
    player->setVideoOutput(videoWidget);

    QGridLayout *grid = new QGridLayout;
    this->setLayout(grid);
    this->layout()->addWidget(videoWidget);
    player->setPlaylist(playlist);
    playlist->setCurrentIndex(0);
    player->play();
}

MedNUSVideoViewer::~MedNUSVideoViewer()
{
    player->stop();
    delete player;
}

void MedNUSVideoViewer::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space) {
        if(player->state() == QMediaPlayer::PlayingState)
            player->pause();
        else
            player->play();
    }
}
